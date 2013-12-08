#ifndef NETWORK_H
#define NETWORK_H

#include "Player.h"
#include <thread>

class Network {
public:
	struct Settings {
		char* addr;
		char* port;
	};

	Network();

	//wait until player made it's step
	Player::Step* GetPlayerStep();
	
	//try send step to network player
	//if return 0, it's good
	//must free step
	int SendPlayerStep(Player::Step* step);
	
	//connect and start thread of keeping connection if success
	int Connect(Settings& settings);
private:
	Settings settings;

	//thread; parent: this
	friend void KeepConnection(Network*parent);
};

#endif /* NETWORK_H */
