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


//думаю треба винести це в окремий модуль
//**
int ReceiveStr(SOCKET clSocket, string& key, string& value) {
	char recvbuf[PentagoServer::DEFAULT_BUFLEN];
	int iResult;
	iResult = recv(clSocket, recvbuf, sizeof(size_t), 0);
	if (iResult <= 0) {
		return iResult;
	}
	key.resize(((size_t*) recvbuf)[0]);
	iResult = recv(clSocket, recvbuf, sizeof(size_t), 0);
	if (iResult <= 0) {
		return iResult;
	}
	value.resize(((size_t*) recvbuf)[0]);
	if ( key.length() > 0 ) {
		iResult = recv(clSocket, const_cast<char*>(key.c_str()), key.length(), 0);
		if (iResult <= 0) {
			return iResult;
		}
	}
	if ( value.length() > 0 ) {
		iResult = recv(clSocket, const_cast<char*>(value.c_str()), value.length(), 0);
		if (iResult <= 0) {
			return iResult;
		}
	}
//	cout << endl << endl << key << ":" << value << endl << endl;
	return iResult;
}

int SendStr(string key, string value, SOCKET to) {
	//TODO: добавити можливітсь відправляти дані довжиною більше ніж DEFAULT_BUFLEN
	char sendbuf[PentagoServer::DEFAULT_BUFLEN];
	((size_t*) sendbuf)[0] = key.length();
	((size_t*) sendbuf)[1] = value.length();
	memcpy((char*) (((size_t*) sendbuf) + 2), key.c_str(), key.length());
	memcpy(((char*) (((size_t*) sendbuf) + 2)) + key.length(), value.c_str(), value.length());
	int iResult = send(to, sendbuf, key.length() + value.length() + 2 * sizeof(size_t), 0);
	if (iResult == SOCKET_ERROR) {
		return WSAGetLastError();
	}
	return 0;
}
//**
//

void ProcessClient(PentagoServer*parent, SOCKET clSocket) {
	int iResult;
	string key, value;
	do {
		if ((iResult = ReceiveStr(clSocket, key, value)) > 0) {
			parent->_SendMsgToAll(key, value, clSocket);
			if (key == "Player1Name") {
				parent->Player1Name = value;
			}
			if (key == "Player2Name") {
				parent->Player2Name = value;
			}
			if (key == "GetPlayer1Name") {
				SendStr("Player1Name", parent->Player1Name, clSocket);
			}
			if (key == "GetPlayer2Name") {
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
				closesocket(clSocket);
			}
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

void PentagoServer::_SendMsgToAll(const string& key, const string& value, SOCKET from) {
	clientsMutex.lock();
	for (unsigned int i = 0; i < clients.size(); i++) {
		if (clients[i] != from) {
			SendStr(key, value, (SOCKET) clients[i]);
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
	deletedThreadsMutex.lock();
	vector<SOCKET>::iterator i = clients.begin();
	vector<thread*>::iterator j = clientProcessors.begin();
	for (unsigned int counter = 0;counter < clients.size() ; counter++, j++, i++) {
		if (*i == clSocket) {
			clients.erase(i);
			deletedThreads.push_back(*j);
			clientProcessors.erase(j);
			break;
		}
	}
	deletedThreadsMutex.unlock();
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
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
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

bool PentagoServer::IsGood() {
	return isGood;
}

PentagoServer::~PentagoServer() {
	isGood = false;
	closesocket(_ListenSocket);
	clientsMutex.lock();
	for (vector<SOCKET>::iterator i = clients.begin(); i != clients.end(); i++) {
		closesocket((SOCKET)*i);
	}
	clientsMutex.unlock();
	deletedThreadsMutex.lock();
	for (vector<thread*>::iterator i = deletedThreads.begin(); i != deletedThreads.end(); i++) {
		(*i)->join();
	}
	deletedThreadsMutex.unlock();
	clientProcessorsMutex.lock();
	for (vector<thread*>::iterator i = clientProcessors.begin(); i != clientProcessors.end(); i++) {
		(*i)->join();
	}
	clientProcessorsMutex.unlock();
}

