#include "PlayerLocal.h"

#include "Game.h"

Player::Step PlayerNetwork::MakeStep(){
	return Game::Instance()->network.GetPlayerStep();
}

PlayerNetwork::PlayerNetwork(std::string _name) :
		Player(_name) {
	playerType = 1;
}

