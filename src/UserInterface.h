#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>

#include "Referee.h"
#include "Board.h"
#include "Player.h"
#include "Network.h"

class UserInterface {
public:
	enum MenuItem {
		ExitGame = 0, LocalGame = 1, StartHost = 2, ConnectToServer = 3
	};

	UserInterface(const Board *board = NULL);
	~UserInterface();

	//wait until player made it's step
	Player::Step GetPlayerStep(const Player* player);

	//obviously
	void PaintBoard();

	//obviously too
	void ShowWinner(winstatus status);

	//obviously too
	void Show_StepIsNotAllowed();

	//�������� ��������� �������
	MenuItem MenuDialog();

	//�������� ������� � ������� ������ �����
	//������ ����� �������� ������������ ����� �� �������� �����
	bool GetHostAddress(Network::RemoteAddress* addr);

	//���� �������, ������� �, ����'������, �����
	void Show_GameBegins();

	void Show_WaitingForOponentsStep();

	//������� � �����, � ���� � �����������, ����
	//can't return ""
	std::string InputPlayerName(const std::string& who);

	//�������� � ������� (����������� � Network)
	void Show_WaitForConnection();

	//��� ����� ��������� �������� � �������, ���� ���� ������� (����� ��������� � Network)
	void _PlayerConnected(const Player* player);

	void Show_PlayerDisconnected(const Player* player);

	void Show_CanNotConnect(const Network::RemoteAddress* addr);

	void Show_CanNotStartServer();

#ifdef _PENTAGO_OPENGL_
private:
	std::thread *GLthread;
#endif
private:
	const Board * _board;
#ifdef DEBUG
public:
	void ShowDebugInfo(const char* info);
#endif
};

#endif /* USER_INTERFACE_H */
