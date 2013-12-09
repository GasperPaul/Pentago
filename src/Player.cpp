/*
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#include "Player.h"

std::string Player::GetName() const{
	return name;
}

void Player::SetName(const string name) {
	this->name = name;
}

Player::Step* Player::MakeStep() {
	return NULL;
}

Player::Player(std::string _name) : name(_name) { }
Player::~Player() { }

