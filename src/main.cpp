/*
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#include "Game.h"

int main(){
	Game* game = Game::GetInstance();
	game->Run();
	return 0;
}


