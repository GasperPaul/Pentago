/*
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
	Player(std::string _name = "Player");
	virtual ~Player();
	short* Step();
	std::string Name();

private:
	std::string name;
};

#endif /* PLAYER_H */
