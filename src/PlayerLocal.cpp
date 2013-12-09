#include "PlayerLocal.h"

#include "Game.h"

Player::Step* PlayerLocal::MakeStep() {
	Game *game = Game::Instance();
	Player::Step*step = game->userInterface.GetPlayerStep(this);
	game->network.SendPlayerStep(step);
	return step;
}

PlayerLocal::PlayerLocal(std::string _name) :
		Player(_name) {
}

