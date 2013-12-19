/*
 * Controls.cpp
 *
 *  Created on: 16 dec. 2013
 *      Author: Youw
 */

#include "Controls.h"
#include "GLRenderer.h"
#include "../Game.h"

#include <iostream>

//*externs
GameMode gameMode;
UserInterface::MenuItem menuPressed;
std::vector<Stone*> stones;
std::vector<GameObject*> menuButtons;
std::vector<GameObject*> rotationButtons;
//*

Point3D translate = { 0, 0, 0 };
Point2D oldPos;
tcursor cursor;

namespace Controls {

void onWindowCloseCallBack(GLFWwindow* window)
{
	Game *game = Game::GetInstance();
	game->mustShutdown = true;
	step = {-1, -1, -1, -1};
	_mutex.unlock();
	game->network.CloseConnection();
}

void errorCallback(int error, const char* errorText) {
	std::cout << "Error: " << errorText << std::endl;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch(key){
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	}
}

void WaitingCallback(GLFWwindow* window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch(action) {
		case GLFW_PRESS:
//			glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
//			for(GameObject* obj : menuButtons)
//				obj->Clicked(cursor.position);
			break;
		}
	}
}

void MainMenuCallback(GLFWwindow* window, int button, int action, int mods){
	glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch(action) {
		case GLFW_PRESS:
			glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
			for(GameObject* obj : menuButtons)
				obj->Clicked(cursor.position);
			break;
		}
	}
}

void PlayerStepCallback(GLFWwindow* window, int button, int action, int mods){
	glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch(action) {
		case GLFW_PRESS:
			glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
			for(GameObject* obj : stones)
				obj->Clicked(cursor.position);
			break;
		}
	}
}

void RotationCallback(GLFWwindow* window, int button, int action, int mods){
	glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch(action) {
		case GLFW_PRESS:
			glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
			for(GameObject* obj : rotationButtons)
				obj->Clicked(cursor.position);
			break;
		}
	}
}

void OponentStepCallback(GLFWwindow* window, int button, int action, int mods){
	glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch(action) {
		case GLFW_PRESS:

			break;
		}
	}
}

void mouseMoveCallback(GLFWwindow* window, double x, double y) {
	if(cursor.moving) {
		oldPos = cursor.position;
		glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
		translate.x += (cursor.position.x - oldPos.x) / 100;
		translate.y += (oldPos.y - cursor.position.y) / 100;
	}
}

};
