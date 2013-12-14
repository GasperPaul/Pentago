#include "PlayerLocal.h"

#include "Game.h"

Player::Step PlayerLocal::MakeStep() {
	return Game::Instance()->userInterface.GetPlayerStep(this);
}

PlayerLocal::PlayerLocal(std::string _name) :
		Player(_name) {
	playerType = 0;
}

