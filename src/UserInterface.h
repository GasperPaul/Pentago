#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "Board.h"
#include "Referee.h"
#include "Player.h"
#include "Network.h"

class UserInterface {
public:
	enum MenuItem {ExitGame=0,LocalGame=1,StartHost=2,ConnectToServer=3};

	UserInterface();

	//wait until player made it's step
	Player::Step* GetPlayerStep(const Player* player);
	
	//obviously
	void PaintBoard(Board& board);
	
	//obviously too
	void ShowWinner(winstatus status,Player players[2]);
	
	//obviously too
	void Show_StepIsNotAllowed();
	
	//показати початкову менюшку
	MenuItem ShowMenu();

	//показати менюшку з вибором адреси хоста
	//колись можна написати автоматичний пошук по локальній мережі
	Network::Settings* GetHostAddress();

	//куча анімації, графіки і, обов'язково, вибух
	void Show_GameBegin();
};

#endif /* USER_INTERFACE_H */
