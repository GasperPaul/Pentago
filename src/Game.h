/*
 * Game.h
 *
 *  Created on: 10 ����. 2013
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

	//�������, ����
	void Run();

//	void TempTestReferee();//Temp
	UserInterface userInterface;
	Network network;

private:
	//� ��� ��� ���� ��� ���, �� ���� �� ����� ����� � ����
	void PlayGame();

	Board board;
	Player* players[2];
	Referee referee;

	static Game* instance;
	Game();
	Game(const Game&);
};

#endif /* GAME_H */
