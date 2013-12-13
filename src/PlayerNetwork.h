
#ifndef PLAYER_NETWORK_H
#define PLAYER_NETWORK_H

#include <string>
#include "Player.h"

class PlayerNetwork: public Player {
public:
	PlayerNetwork(std::string _name = "Player");
	Step MakeStep();
};

#endif /* PLAYER_NETWORK_H */
