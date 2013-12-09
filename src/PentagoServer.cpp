/*
 * PenpagoServer.cpp
 *
 *  Created on: 8 груд. 2013
 *      Author: Youw
 */

#include "PentagoServer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <string.h>

using std::thread;
using std::string;

const std::string PentagoServer::DEFAULT_PORT = "26326";
const int PentagoServer::DEFAULT_BUFLEN = 1024;

int ReceiveStr(SOCKET clSocket, string& key, string& value) {
	char recvbuf[PentagoServer::DEFAULT_BUFLEN];

	int iResult = recv(clSocket, recvbuf, sizeof(size_t), 0);
	if (iResult <= 0) {
		return iResult;
	}
	key.resize(((size_t*) recvbuf)[0]);
	iResult = recv(clSocket, recvbuf, sizeof(size_t), 0);
	if (iResult <= 0) {
		return iResult;
	}
	value.resize(((size_t*) recvbuf)[0]);
	iResult = recv(clSocket, const_cast<char*>(key.c_str()), key.length(), 0);
	if (iResult <= 0) {
		return iResult;
	}
	iResult = recv(clSocket, const_cast<char*>(value.c_str()), value.length(), 0);
	if (iResult <= 0) {
		return iResult;
	}
	return iResult;
}

void ProcessClient(PentagoServer*parent, SOCKET clSocket) {
	int iResult;
	string key, value;
	do {
		if ((iResult = ReceiveStr(clSocket, key, value)) > 0) {
			parent->_SendMsgToAll(key,value,clSocket);
		} else {
			parent->_RemClientFromList(clSocket);
			closesocket(clSocket);
			//	std::cout << "Player disconnected." << std::endl;
			//	return WSAGetLastError();
		}
	} while (iResult > 0);
}

void KeepServerOn(PentagoServer*parent) {
	while (true) {
		SOCKET ClientSocket = accept(parent->_ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			//printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(parent->_ListenSocket);
			//WSACleanup();
			break;
		}
		thread *thr = new thread;
		parent->_AddClientToList(thr, ClientSocket);
		*thr = std::thread(ProcessClient, parent, ClientSocket);
	}
}

int SendStr(string key, string value, SOCKET to) {
	//TODO: добавити можливітсь відправляти дані довжиною більше ніж DEFAULT_BUFLEN
	char sendbuf[PentagoServer::DEFAULT_BUFLEN];
	((size_t*) sendbuf)[0] = key.length();
	((size_t*) sendbuf)[1] = value.length();
	strcpy((char*) (((size_t*) sendbuf) + 2), key.c_str());
	strcpy(((char*) (((size_t*) sendbuf) + 2)) + key.length(), value.c_str());
	int iResult = send(to, sendbuf, key.length() + value.length() + 2 * sizeof(size_t), 0);
	if (iResult == SOCKET_ERROR) {
		return WSAGetLastError();
	}
	return 0;
}

void PentagoServer::_SendMsgToAll(const string& key, const string& value, SOCKET from) {
	clientsMutex.lock();
	for (unsigned int i = 0; i < clients.size(); i++) {
		if (clients[i] != from) {
			SendStr(key, value, (SOCKET)clients[i]);
		}
	}
	clientsMutex.unlock();
}

void PentagoServer::_AddClientToList(thread* clThread, SOCKET clSocket) {
	clientsMutex.lock();
	clientProcessorsMutex.lock();
	clientProcessors.push_back(clThread);
	clients.push_back(clSocket);
	clientProcessorsMutex.unlock();
	clientsMutex.unlock();
}

void PentagoServer::_RemClientFromList(SOCKET clSocket) {
	clientsMutex.lock();
	clientProcessorsMutex.lock();

	//TODO: complete it later

//	clientProcessors.push_back(clThread);
//	clients.push_back(clSocket);
	clientProcessorsMutex.unlock();
	clientsMutex.unlock();
}

PentagoServer::PentagoServer(string port) {
	isGood = false;
	_ListenSocket = INVALID_SOCKET;
	WSADATA wsaData;
	int iResult;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		//printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
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

	// Create a SOCKET for connecting to server
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
	serv = new thread(KeepServerOn, this);
}

bool PentagoServer::Good() {
	return isGood;
}

PentagoServer::~PentagoServer() {
	closesocket(_ListenSocket);
	for (unsigned int i = 0; i < clients.size(); i++) {
		closesocket((SOCKET) clients[i]);
	}
}

