
#ifndef PLAYER_LOCAL_H
#define PLAYER_LOCAL_H

#include <string>
#include "Player.h"

class PlayerLocal: public Player {
public:
	PlayerLocal(std::string _name = "Player");
	Step* MakeStep();
};

#endif /* PLAYER_LOCAL_H */
