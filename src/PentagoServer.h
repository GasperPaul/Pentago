/*
 * PenpagoServer.h
 *
 *  Created on: 8 груд. 2013
 *      Author: Youw
 */

#ifndef PENTAGOSERVER_H_
#define PENTAGOSERVER_H_

#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include <winsock2.h>

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
	bool Good();

	~PentagoServer();

private:
	vector<SOCKET> clients;
	vector<thread*> clientProcessors;
	mutex clientsMutex;
	mutex clientProcessorsMutex;
	bool isGood;

	void _AddClientToList(std::thread* clThread, SOCKET clSocket);
	void _RemClientFromList(SOCKET clSocket);
	void _SendMsgToAll(const string& key, const string& value, SOCKET from);
	SOCKET _ListenSocket;

	friend void KeepServerOn(PentagoServer*parent);
	friend void ProcessClient(PentagoServer*parent, SOCKET clSocket);

	thread *serv;
};
#endif /* PENTAGOSERVER_H_ */
