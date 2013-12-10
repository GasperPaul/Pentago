#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>

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
	MenuItem MenuDialog();

	//показати менюшку з вибором адреси хоста
	//колись можна написати автоматичний пошук по локальній мережі
	bool GetHostAddress(Network::RemoteAddress* addr);

	//куча анімації, графіки і, обов'язково, вибух
	void Show_GameBegins();

	void Show_WaitingForOponentsStep();

	//можливо з клави, а може з налаштувань, тощо
	//!important can't be ""
	std::string InputPlayerName(std::string who);

	//віконечко з бігунком (викликається з Network)
	void _WaitForConnection();

	//тут треба закривати віконечко з бігунком, якщо воно відкрите (також викликати з Network)
	void _PlayerConnected(const Player* player);

	void Show_PlayerDisconnected(const Player* player);
};

#endif /* USER_INTERFACE_H */
