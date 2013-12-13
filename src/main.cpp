/*
 *  Created on: 10 ����. 2013
 *      Author: Gasper
 */

#include "Game.h"

int main(){
	Game* game = Game::Instance();
	game->Run();
	game->~Game();
	return 0;
}


