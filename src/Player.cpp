/*
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#include "Player.h"
#include <iostream>

short* Player::Step() {
	short x, y, q, r;
	bool invalid = true;

	std::cout << name << ", make your move: " << std::endl
		 << "row | column | quadrant | left=0, right=1 " << std::endl
		 << "End game Ч any value < 0." << std::endl;
	do {
		std::cout << "> ";
		std::cin >> x >> y >> q >> r;
		if (x < 0 || y < 0 || q < 0 || r < 0)
			return new short[4] { -1, -1, -1, -1 };
		invalid = x>=6 && y>=6 && q>=4 && r>=2;
		if (invalid)
			std::cout << "Invalid input, try again." << std::endl;
	} while (invalid);

	return new short[4] { x, y, q, r };
}

std::string Player::Name() {
	return name;
}

Player::Player(std::string _name) : name(_name) { }
Player::~Player() { }

