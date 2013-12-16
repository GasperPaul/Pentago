/*
 * PenpagoServer.cpp
 *
 *  Created on: 8 ����. 2013
 *      Author: Youw
 */
#include "PentagoServer.h"

#include <string.h>

#include "CrossThreadMutex.h"

using std::thread;
using std::string;

const std::string PentagoServer::DEFAULT_PORT = "26326";
#include "NetCommunication.h"

void PentagoServer::ProcessClient(PentagoServer*parent, SOCKET clSocket) {
	int iResult;
	string key, value;
	do {
		if ((iResult = ReceiveStr(clSocket, key, value)) > 0) {
			if (key == "PlayerStep") {
				parent->_SendMsgToAll(key, value, clSocket);
			} else if (key == "Player1Name") {
				parent->Player1Name = value;
				parent->_SendMsgToAll(key, value, clSocket);
			} else if (key == "Player2Name") {
				parent->Player2Name = value;
				parent->_SendMsgToAll(key, value, clSocket);
			} else if (key == "GetPlayer1Name") {
				SendStr("Player1Name", parent->Player1Name, clSocket);
			} else if (key == "GetPlayer2Name") {
				SendStr("Player2Name", parent->Player2Name, clSocket);
			}
		} else {
			//if server is running
			if (parent->isGood) {
				//Connection with client lost:
				parent->_RemClientFromList(clSocket);
				string value;
				value.push_back(char(-2));
				value.push_back(char(-2));
				value.push_back(char(-2));
				value.push_back(char(-2));
				parent->_SendMsgToAll("PlayerStep", value, clSocket);
				shutdown(clSocket, SD_BOTH);
				closesocket(clSocket);
			}
		}
	} while (iResult > 0);
}

void PentagoServer::KeepServerOn(PentagoServer*parent) {
	while (parent->isGood) {
		SOCKET ClientSocket = accept(parent->_ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			if (parent->isGood) {
				shutdown(parent->_ListenSocket, SD_BOTH);
				closesocket(parent->_ListenSocket);
			}
			parent->isGood = false;
		}
		parent->_AddClientToList(new thread(ProcessClient, parent, ClientSocket), ClientSocket);
	}
}

void PentagoServer::_SendMsgToAll(const string& key, const string& value, SOCKET from) {
	clientsMutex.lock();
	if (isGood) {
		for (unsigned int i = 0; i < clients.size(); i++) {
			if (clients[i] != from) {
				SendStr(key, value, clients[i]);
			}
		}
	}
	clientsMutex.unlock();
}

void PentagoServer::_AddClientToList(thread* clThread, SOCKET clSocket) {
	clientsMutex.lock();
	clientProcessorsMutex.lock();
	if (isGood) {
		clientProcessors.push_back(clThread);
		clients.push_back(clSocket);
	}
	clientProcessorsMutex.unlock();
	clientsMutex.unlock();
}

void PentagoServer::_RemClientFromList(SOCKET clSocket) {
	clientsMutex.lock();
	clientProcessorsMutex.lock();
	deletedThreadsMutex.lock();
	if (isGood) {
		vector<SOCKET>::iterator i = clients.begin();
		vector<thread*>::iterator j = clientProcessors.begin();
		for (unsigned int counter = 0; counter < clients.size(); counter++, j++, i++) {
			if (*i == clSocket) {
				clients.erase(i);
				deletedThreads.push_back(*j);
				clientProcessors.erase(j);
				break;
			}
		}
	}
	deletedThreadsMutex.unlock();
	clientProcessorsMutex.unlock();
	clientsMutex.unlock();
}

PentagoServer::PentagoServer(string port) {
	isGood = false;
	_ListenSocket = INVALID_SOCKET;
	int iResult;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
#ifdef _WIN32
	// Initialize Winsock
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		//printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}
#endif
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
	if (iResult != 0) {
		//printf("getaddrinfo failed with error: %d\n", iResult);
		//WSACleanup();
		return;
	}

	// Create a SOCKET for waiting for clients
	_ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_ListenSocket == INVALID_SOCKET) {
		//printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		//WSACleanup();
		return;
	}

	// Setup the TCP listening socket
	iResult = bind(_ListenSocket, result->ai_addr, (int) result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		//printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(_ListenSocket);
		//WSACleanup();
		return;
	}

	freeaddrinfo(result);

	iResult = listen(_ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		//printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(_ListenSocket);
		//WSACleanup();
		return;
	}
	isGood = true;
	serv = thread(KeepServerOn, this);
}

bool PentagoServer::IsGood() {
	return isGood;
}

void PentagoServer::CloseServer() {
	isGood = false;
	_SendMsgToAll("ServerClosing", "", -10);
	clientsMutex.lock();
	for (unsigned i = 0; i < clients.size(); i++) {
		shutdown(clients[i], SD_BOTH);
		closesocket(clients[i]);
	}
	clientsMutex.unlock();
	deletedThreadsMutex.lock();
	for (unsigned i = 0; i < deletedThreads.size(); i++) {
		if (deletedThreads[i]->joinable()) {
			deletedThreads[i]->join();
			delete deletedThreads[i];
		}
		deletedThreads.clear();
	}
	deletedThreadsMutex.unlock();
	clientProcessorsMutex.lock();
	for (unsigned i = 0; i < clientProcessors.size(); i++) {
		if (clientProcessors[i]->joinable()) {
			clientProcessors[i]->join();
			delete clientProcessors[i];
		}
		clientProcessors.clear();
	}
	clientProcessorsMutex.unlock();
	shutdown(_ListenSocket, SD_BOTH);
	closesocket(_ListenSocket);
	if (serv.joinable()) {
		serv.join();
	}
}

PentagoServer::~PentagoServer() {
	CloseServer();
}

