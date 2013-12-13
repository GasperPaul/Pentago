/*
 * PenpagoServer.h
 *
 *  Created on: 8 ����. 2013
 *      Author: Youw
 */

#ifndef PENTAGOSERVER_H_
#define PENTAGOSERVER_H_

#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include "SocketIncludes.h"

using std::string;
using std::mutex;
using std::vector;
using std::thread;

class PentagoServer {
public:
	static const int DEFAULT_BUFLEN;
	static const string DEFAULT_PORT;
	PentagoServer(string port);
	void RunPentagoServer();
	bool IsGood();

	~PentagoServer();

private:
	vector<SOCKET> clients;
	vector<thread*> clientProcessors;
	vector<thread*> deletedThreads;
	string Player1Name, Player2Name;
	mutex clientsMutex;
	mutex clientProcessorsMutex;
	mutex deletedThreadsMutex;

	bool isGood;

	void _AddClientToList(std::thread* clThread, SOCKET clSocket);
	void _RemClientFromList(SOCKET clSocket);
	void _SendMsgToAll(const string& key, const string& value, SOCKET from);
	SOCKET _ListenSocket;

	friend void KeepServerOn(PentagoServer*parent);
	friend void ProcessClient(PentagoServer*parent, SOCKET clSocket);

	thread serv;
};
#endif /* PENTAGOSERVER_H_ */
