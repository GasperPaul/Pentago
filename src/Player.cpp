/*
 *  Created on: 10 ����. 2013
 *      Author: Gasper
 */
#include "Player.h"

std::string Player::GetName() const{
//	const_cast<mutex*>(&PlayerNameAccessMutex)->lock();
	string name = this->name;
//	const_cast<mutex*>(&PlayerNameAccessMutex)->unlock();
	return name;
}

void Player::SetName(const string name) {
//	PlayerNameAccessMutex.lock();
	this->name = name;
//	PlayerNameAccessMutex.unlock();
}

Player::Step Player::MakeStep() {
	//virtual method
	return Player::Step();
}

int Player::GetPlayerType() {
	return playerType;
}

Player::Player(std::string _name) : name(_name) {
	playerType = -1;
}
Player::~Player() { }

