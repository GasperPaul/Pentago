#ifndef NETWORK_H
#define NETWORK_H

#include "Player.h"

#include <thread>
#include <string>
#include <mutex>

#include "SocketIncludes.h"
#include "CrossThreadMutex.h"

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
	virtual ~Network();

	//wait until player made it's step
	Player::Step GetPlayerStep();
	
	//try send step to network player
	//if return 0, it's good
	int SendPlayerStep(Player::Step* step);
	
	//connect and start thread of keeping connection if success
	//return 0 if success
	int Connect(const RemoteAddress* settings, Player* player[2], char PlayerNum);

	//���� �� �������, �������� ��������
	bool WaitForConnection();
	bool IsConnected() const;

	void CloseConnection();

private:
	int SendStr(string key, string value);
	RemoteAddress settings;
	SOCKET HostSocket;

	mutex ReceivedStepMutex;
	Player::Step ReceivedStep;
	CrossThreadMutex 	StepFromPlayerIsReceivedMutex,
						PlayerIsConnectedWutex;

	thread keepConnectionThread;
	//for thread; parent: this
	static void _KeepConnection(Network*parent);
};

#endif /* NETWORK_H */
