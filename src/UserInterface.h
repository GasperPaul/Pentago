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
	Player::Step GetPlayerStep(const Player* player);
	
	//obviously
	void PaintBoard(Board& board);
	
	//obviously too
	void ShowWinner(winstatus status,Player* players[2]);
	
	//obviously too
	void Show_StepIsNotAllowed();
	
	//�������� ��������� �������
	MenuItem MenuDialog();

	//�������� ������� � ������� ������ �����
	//������ ����� �������� ������������ ����� �� �������� �����
	bool GetHostAddress(Network::RemoteAddress& addr);

	//���� �������, ������� �, ����'������, �����
	void Show_GameBegins();

	void Show_WaitingForOponentsStep();

	//������� � �����, � ���� � �����������, ����
	//!important can't be ""
	std::string InputPlayerName(std::string who);

	//�������� � ������� (����������� � Network)
	void Show_WaitForConnection();

	//��� ����� ��������� �������� � �������, ���� ���� ������� (����� ��������� � Network)
	void _PlayerConnected(const Player* player);

	void Show_PlayerDisconnected(const Player* player);

	void Show_CanNotConnect(const Network::RemoteAddress* addr);

	void Show_CanNotStartServer();

#ifdef DEBUG
	void ShowDebugInfo(const char* info);
#endif
};

#endif /* USER_INTERFACE_H */
