/*
 * Game.h
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include "Referee.h"

class Game {
public:
	virtual ~Game();
	static Game* Instance();
	void Run();
	void TempTestReferee();//Temp

private:
	Board board;
	Player* players[2];
	Referee referee;

	static Game* instance;
	Game();
	Game(const Game&);
};

#endif /* GAME_H */
