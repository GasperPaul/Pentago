#include "PlayerLocal.h"

#include "Game.h"

Player::Step PlayerLocal::MakeStep() {
	return Game::GetInstance()->userInterface.GetPlayerStep(this);
}

PlayerLocal::PlayerLocal(std::string _name) :
		Player(_name) {
	playerType = 0;
}

