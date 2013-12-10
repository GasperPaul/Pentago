/*
 *  Created on: 10 лист. 2013
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

Player::Step* Player::MakeStep() {
	//virtual method
	return NULL;
}

Player::Player(std::string _name) : name(_name) { }
Player::~Player() { }

