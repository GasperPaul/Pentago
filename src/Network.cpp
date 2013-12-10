#include "Network.h"
#include "Player.h"
#include "PentagoServer.h"
#include "Game.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

extern int ReceiveStr(SOCKET clSocket, string& key, string& value);
extern int SendStr(string key, string value, SOCKET to);

bool Network::IsConnected() const {
	return HostSocket != SOCKET_ERROR;
}

//debug
/*
#include <iostream>
using std::cout;
using std::endl;
*/

void _KeepConnection(Network*parent, bool LockWaitMutexForConnection) {
	int iResult;
	bool isWaitMutexForConnectionLocked = LockWaitMutexForConnection;
	if (LockWaitMutexForConnection) {
		parent->WaitMutexForConnection.lock();
	}
	string key, value;
	do {
		if ((iResult = ReceiveStr(parent->HostSocket, key, value)) > 0) {
			if (key == "PlayerStep") {
				parent->StepFromPlayerReceivedMutex.lock();
				parent->ReceivedStep.i = short(value[0]);
				parent->ReceivedStep.j = short(value[1]);
				parent->ReceivedStep.quarter = short(value[2]);
				parent->ReceivedStep.direction = Board::RotateDirection(value[3]);
				parent->StepFromPlayerReceived = true;
				parent->StepFromPlayerReceivedMutex.unlock();
			} else if (key == "Player1Name") {
				Game *game = Game::Instance();
				game->SetPlayerName(Game::Player1, value);
				game->userInterface._PlayerConnected(game->GetPlayer(Game::Player1));
				//TODO: debug
//				cout << "Come: " << key << ":" << value << endl;
				;
				if (!parent->otherPlayerIsConnected) {
					parent->otherPlayerIsConnected = true;
					if (isWaitMutexForConnectionLocked) {
						parent->WaitMutexForConnection.unlock();
						isWaitMutexForConnectionLocked = false;
					}
				}
			} else if (key == "Player2Name") {
				Game *game = Game::Instance();
				game->SetPlayerName(Game::Player2, value);
				game->userInterface._PlayerConnected(game->GetPlayer(Game::Player2));
				//TODO: debug
//				cout << "Come: " << key << ":" << value << endl;
				if (!parent->otherPlayerIsConnected) {
					parent->otherPlayerIsConnected = true;
					if (isWaitMutexForConnectionLocked) {
						parent->WaitMutexForConnection.unlock();
						isWaitMutexForConnectionLocked = false;
					}
				}

			}
		} else {
			//Connection with server lost:
			parent->StepFromPlayerReceivedMutex.lock();
			parent->ReceivedStep.i = -3;
			parent->ReceivedStep.j = -3;
			parent->ReceivedStep.quarter = -3;
			parent->ReceivedStep.direction = Board::RotateDirection(-3);
			parent->StepFromPlayerReceived = true;
			parent->StepFromPlayerReceivedMutex.unlock();
			closesocket(parent->HostSocket);
			parent->HostSocket = SOCKET_ERROR;
		}
	} while (iResult > 0);
}

Network::Network() {
	HostSocket = INVALID_SOCKET;
	otherPlayerIsConnected = false;
	keepConnectionThread = NULL;
	StepFromPlayerReceived = false;
}

Player::Step* Network::GetPlayerStep() {
	Game::Instance()->userInterface.Show_WaitingForOponentsStep();
	while (!StepFromPlayerReceived) {
		std::this_thread::yield();
		Sleep(1000);
	}
	Player::Step* result = new Player::Step;
	StepFromPlayerReceivedMutex.lock();
	*result = ReceivedStep;
	StepFromPlayerReceived = false;
	StepFromPlayerReceivedMutex.unlock();
	return result;
}

int Network::SendPlayerStep(Player::Step* step) {
	string stepValue;
	stepValue.push_back((char) step->i);
	stepValue.push_back((char) step->j);
	stepValue.push_back((char) step->quarter);
	stepValue.push_back((char) step->direction);
	return SendStr("PlayerStep", stepValue);
}

int Network::Connect(const RemoteAddress* settings, Player* player[2], char PlayerNum) {
	StepFromPlayerReceived = false;
	this->settings = *settings;
	otherPlayerIsConnected = false;
	WSADATA wsaData;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		//	printf("WSAStartup failed with error: %d\n", iResult);
		return iResult;
	}
	ZeroMemory( &hints, sizeof(hints) );
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
			return WSAGetLastError();
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
	if (PlayerNum == (char) Game::Player1 || PlayerNum == (char) Game::PlayerBoth) {
		iResult = SendStr("Player1Name", player[Game::Player1]->GetName());
		if (iResult != 0) {
			return iResult;
		}
	}
	if (PlayerNum == (char) Game::Player2 || PlayerNum == (char) Game::PlayerBoth) {
		iResult = SendStr("Player2Name", player[Game::Player2]->GetName());
		if (iResult != 0) {
			return iResult;
		}
	}
	if (keepConnectionThread && keepConnectionThread->joinable()) {
		keepConnectionThread->join();
	}
	delete keepConnectionThread;
	keepConnectionThread = new thread;
	bool LockWaitMutexForConnection;
	if (PlayerNum == (char) Game::Player1) {
		LockWaitMutexForConnection = true;
		otherPlayerIsConnected = false;
	} else {
		otherPlayerIsConnected = true;
		LockWaitMutexForConnection = false;
	}
	*keepConnectionThread = thread(_KeepConnection, this, LockWaitMutexForConnection);
	if (PlayerNum == (char) Game::Player2) {
		SendStr("GetPlayer1Name", "");
		Game *game = Game::Instance();
		while (game->GetPlayer(Game::Player1)->GetName() == "") {
			std::this_thread::yield();
			Sleep(100);
		}
	}
	Sleep(500);
	return iResult;
}

int Network::SendStr(string key, string value) {
	return ::SendStr(key, value, HostSocket);
}

void ThreadMutexWait(mutex*m) {
	m->lock();
	m->unlock();
}

bool Network::WaitForConnection() {
	Game *game = Game::Instance();
	game->userInterface._WaitForConnection();
	if (!otherPlayerIsConnected) {
		thread waitThread = thread(ThreadMutexWait, &WaitMutexForConnection);
		waitThread.join();
	}
	return true;
}
