/*
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using std::string;

#include "Board.h"

class Player {
public:
	struct Step {
		short i,j;
		short quarter;
		Board::RotateDirection direction;
	};
	Player(std::string _name = "Player");
	virtual ~Player();
	virtual Step* MakeStep();
	string Name() const;

private:
	string name;
};

#endif /* PLAYER_H */
