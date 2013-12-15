#if defined(_PENTAGO_OPENGL_) && defined(__MainUserInterfaceFile)


#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "PentagoServer.h"


#include <thread>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "GLInterface/Stone.h"
#include "GLInterface/Controls.h"
#include "GLInterface/GLMain.h"
#include "GLInterface/EventList.h"
#include "CrossThreadMutex.h"

using std::thread;

UserInterface::UserInterface(const Board * board) {
	if ((_board = board) != NULL) {
	CrossThreadMutex mutex = CrossThreadMutex(true);
	GLthread = new thread(GLMain, &mutex);
	mutex.wait();
	} else {
		GLthread = NULL;
	}
}

UserInterface::~UserInterface() {
	if (GLthread && GLthread->joinable())
		GLthread->join();
}

Player::Step UserInterface::GetPlayerStep(const Player* player) {
	return Player::Step();
}

void UserInterface::PaintBoard() {
	
}

void UserInterface::ShowWinner(winstatus status, Player* players[2]) {

}

void UserInterface::Show_StepIsNotAllowed() {

}

void UserInterface::Show_GameBegins() {

}

UserInterface::MenuItem UserInterface::MenuDialog() {
	return UserInterface::LocalGame;
}

std::string UserInterface::InputPlayerName(const std::string& who) {
	return "Player";
}

void UserInterface::Show_WaitForConnection() {

}

void UserInterface::_PlayerConnected(const Player* player) {

}

bool UserInterface::GetHostAddress(Network::RemoteAddress* addr) {
	addr->addr="localhost";
	addr->port=PentagoServer::DEFAULT_PORT;
	return true;
}

void UserInterface::Show_WaitingForOponentsStep() {

}

void UserInterface::Show_PlayerDisconnected(const Player* player) {

}


void UserInterface::Show_CanNotStartServer() {

}

void UserInterface::Show_CanNotConnect(const Network::RemoteAddress* addr) {

}
#endif // _PENTAGO_OPENGL_
