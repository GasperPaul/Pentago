#include "PlayerLocal.h"

#include "Game.h"

Player::Step* PlayerLocal::MakeStep() {
	Game *game = Game::Instance();
	return game->Instance()->userInterface.GetPlayerStep(this);
}

PlayerLocal::PlayerLocal(std::string _name) :
		Player(_name) {
}

