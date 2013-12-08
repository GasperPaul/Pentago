/*
 * Game.h
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "PlayerLocal.h"
#include "PlayerNetwork.h"
#include "Referee.h"
#include "Network.h"
#include "UserInterface.h"

class Game {
public:
	virtual ~Game();
	static Game* Instance();

	//менюшка, тощо
	void Run();

//	void TempTestReferee();//Temp
	UserInterface userInterface;
	Network network;

private:
	//а тут вже буде вся гра, аж поки не вийде назад в меню
	void PlayGame();

	Board board;
	Player* players[2];
	Referee referee;

	static Game* instance;
	Game();
	Game(const Game&);
};

#endif /* GAME_H */
