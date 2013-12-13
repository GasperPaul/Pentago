#include "Network.h"
#include "Player.h"
#include "PentagoServer.h"
#include "Game.h"
#include <cstring>

extern int ReceiveStr(SOCKET clSocket, string& key, string& value);
extern int SendStr(string key, string value, SOCKET to);

bool Network::IsConnected() const {
	return HostSocket != SOCKET_ERROR;
}

void _KeepConnection(Network*parent) {
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
				Game *game = Game::Instance();
				game->SetPlayerName(Game::Player1, value);
				game->userInterface._PlayerConnected(game->GetPlayer(Game::Player1));
				parent->PlayerIsConnectedWutex.unlock();
			} else if (key == "Player2Name") {
				Game *game = Game::Instance();
				game->SetPlayerName(Game::Player2, value);
				game->userInterface._PlayerConnected(game->GetPlayer(Game::Player2));
				parent->PlayerIsConnectedWutex.unlock();
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
			parent->HostSocket = SOCKET_ERROR;
		}
	} while (iResult > 0);
}

Network::Network() {
	HostSocket = INVALID_SOCKET;
	StepFromPlayerIsReceivedMutex = CrossThreadMutex(true); //locked: no step received yet
	PlayerIsConnectedWutex = CrossThreadMutex(true);// locked: no player in the same game yet;
}

Player::Step Network::GetPlayerStep() {
	Game::Instance()->userInterface.Show_WaitingForOponentsStep();

	StepFromPlayerIsReceivedMutex.wait();

	Player::Step result;
	ReceivedStepMutex.lock();
	result = ReceivedStep;
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
		//	printf("WSAStartup failed with error: %d\n", iResult);
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
//		printf("getaddrinfo failed with error: %d\n", iResult);
//		WSACleanup();
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
//			printf("socket failed with error: %ld\n", WSAGetLastError());
//			WSACleanup();
#ifdef _WIN32
			return WSAGetLastError();
#elif __linux__
			return -1;
#endif
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
//		printf("Unable to connect to server!\n");
//		WSACleanup();
		return 1;
	}

	if ((PlayerNum == (char) Game::Player1) || (PlayerNum == (char) Game::PlayerBoth)) {
		iResult = SendStr("Player1Name", player[Game::Player1]->GetName());
		if (iResult != 0) {
			return iResult;
		}
	}
	if ((PlayerNum == (char) Game::Player2) || (PlayerNum == (char) Game::PlayerBoth)) {
		iResult = SendStr("Player2Name", player[Game::Player2]->GetName());
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
	Game::Instance()->userInterface.Show_WaitForConnection();
	//TODO: no logic: repair later
	PlayerIsConnectedWutex.wait();
	return true;
}
