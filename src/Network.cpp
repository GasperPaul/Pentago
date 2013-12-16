#include "Network.h"
#include "Player.h"
#include "PentagoServer.h"
#include "Game.h"
#include <cstring>

#include "NetCommunication.h"

bool Network::IsConnected() const {
	return HostSocket != SOCKET_ERROR;
}

void Network::_KeepConnection(Network*parent) {
	int iResult;
	string key, value;
	do {
		if ((iResult = ReceiveStr(parent->HostSocket, key, value)) > 0) {
			if (key == "PlayerStep") {
				parent->ReceivedStepMutex.lock();
				parent->ReceivedStep.i = short(char(value[0]));
				parent->ReceivedStep.j = short(char(value[1]));
				parent->ReceivedStep.quarter = short(char(value[2]));
				parent->ReceivedStep.direction = Board::RotateDirection(char(value[3]));
				parent->StepFromPlayerIsReceivedMutex.unlock();
				parent->ReceivedStepMutex.unlock();
			} else if (key == "Player1Name") {
				Game *game = Game::GetInstance();
				game->SetPlayerName(Game::Player1, value);
				game->userInterface._PlayerConnected(game->GetPlayer(Game::Player1));
				parent->PlayerIsConnectedWutex.unlock();
			} else if (key == "Player2Name") {
				Game *game = Game::GetInstance();
				game->SetPlayerName(Game::Player2, value);
				game->userInterface._PlayerConnected(game->GetPlayer(Game::Player2));
				parent->PlayerIsConnectedWutex.unlock();
			} else if (key == "ServerClosing") {
				parent->ReceivedStepMutex.lock();
				parent->ReceivedStep.i = -4;
				parent->ReceivedStep.j = -4;
				parent->ReceivedStep.quarter = -4;
				parent->ReceivedStep.direction = Board::RotateDirection(-4);
				parent->StepFromPlayerIsReceivedMutex.unlock();
				parent->ReceivedStepMutex.unlock();
				closesocket(parent->HostSocket);
				iResult = -1;
			}
		} else {
			//Connection with server lost:
			parent->ReceivedStepMutex.lock();
			parent->ReceivedStep.i = -3;
			parent->ReceivedStep.j = -3;
			parent->ReceivedStep.quarter = -3;
			parent->ReceivedStep.direction = Board::RotateDirection(-3);
			parent->StepFromPlayerIsReceivedMutex.unlock();
			parent->ReceivedStepMutex.unlock();
			closesocket(parent->HostSocket);
			parent->HostSocket = INVALID_SOCKET;
		}
	} while (iResult > 0);
}

Network::Network() {
	HostSocket = INVALID_SOCKET;
	StepFromPlayerIsReceivedMutex = CrossThreadMutex(true); //locked: no step received yet
	PlayerIsConnectedWutex = CrossThreadMutex(true); // locked: no player in the same game yet;
}

Player::Step Network::GetPlayerStep() {
	Game::GetInstance()->userInterface.Show_WaitingForOponentsStep();
	Player::Step result;

	StepFromPlayerIsReceivedMutex.wait();
	ReceivedStepMutex.lock();
	result = ReceivedStep;
	StepFromPlayerIsReceivedMutex.try_lock();
	ReceivedStepMutex.unlock();
	return result;
}

int Network::SendPlayerStep(Player::Step* step) {
	string stepValue;
	stepValue.resize(4);
	stepValue[0] = (char) step->i;
	stepValue[1] = (char) step->j;
	stepValue[2] = (char) step->quarter;
	stepValue[3] = (char) step->direction;
	return SendStr("PlayerStep", stepValue);
}

int Network::Connect(const RemoteAddress* settings, Player* player[2], char PlayerNum) {
	PlayerIsConnectedWutex.try_lock();
	StepFromPlayerIsReceivedMutex.try_lock();

	this->settings = *settings;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	int iResult;

#ifdef _WIN32
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		return iResult;
	}
#endif
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(settings->addr.c_str(), settings->port.c_str(), &hints, &result);
	if (iResult != 0) {
		return iResult;
	}

	if (HostSocket != INVALID_SOCKET) {
		closesocket(HostSocket);
		HostSocket = INVALID_SOCKET;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		HostSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (HostSocket == INVALID_SOCKET) {
			return -1;
		}

		// Connect to server.
		iResult = connect(HostSocket, ptr->ai_addr, (int) ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(HostSocket);
			HostSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (HostSocket == INVALID_SOCKET) {
		return -1;
	}

	if ((PlayerNum == (char) Game::Player2) || (PlayerNum == (char) Game::PlayerBoth)) {
		iResult = SendStr("Player2Name", player[Game::Player2]->GetName());
		if (iResult != 0) {
			return iResult;
		}
	}
	if ((PlayerNum == (char) Game::Player1) || (PlayerNum == (char) Game::PlayerBoth)) {
		iResult = SendStr("Player1Name", player[Game::Player1]->GetName());
		if (iResult != 0) {
			return iResult;
		}
	}
	if (keepConnectionThread.joinable()) {
		keepConnectionThread.join();
	}
	keepConnectionThread = thread(_KeepConnection, this);
	if (PlayerNum == (char) Game::Player2) {
		SendStr("GetPlayer1Name", "NULL");
		PlayerIsConnectedWutex.try_lock();
		PlayerIsConnectedWutex.wait();
	}
	return 0;
}

int Network::SendStr(string key, string value) {
	return ::SendStr(key, value, HostSocket);
}

bool Network::WaitForConnection() {
	Game::GetInstance()->userInterface.Show_WaitForConnection();
	//TODO: no logic: repair later
	PlayerIsConnectedWutex.wait();
	return true;
}

void Network::CloseConnection() {
	shutdown(HostSocket, SD_BOTH);
	closesocket(HostSocket);
	if (keepConnectionThread.joinable()) {
		keepConnectionThread.join();
	}
}

Network::~Network() {
	CloseConnection();
}
