#ifndef NETWORK_H
#define NETWORK_H

#include "Player.h"

#include <thread>
#include <string>
#include <mutex>

#include <winsock2.h>

using std::string;
using std::thread;
using std::mutex;

class Network {
public:

	struct RemoteAddress {
		string addr;
		string port;
	};

	Network();

	//wait until player made it's step
	Player::Step* GetPlayerStep();
	
	//try send step to network player
	//if return 0, it's good
	//must free step
	int SendPlayerStep(Player::Step* step);
	
	//connect and start thread of keeping connection if success
	//return 0 if success
	int Connect(const RemoteAddress* settings, Player* player[2], char PlayerNum);

	//якщо ми хостуємо, почекати товариша
	bool WaitForConnection();

private:
	int SendStr(string key, string value);
	RemoteAddress settings;
	SOCKET HostSocket;

	Player::Step ReceivedStep;
	bool StepFromPlayerReceived;

	mutex WaitMutexForConnection, StepFromPlayerReceivedMutex;
	thread *keepConnectionThread;
	bool otherPlayerIsConnected;
	//for thread; parent: this
	friend void _KeepConnection(Network*parent);
};

#endif /* NETWORK_H */
