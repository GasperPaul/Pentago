/*
 *  Created on: 11 груд. 2013
 *      Author: Gasper
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include <GLFW/glfw3.h>
#include <iostream>

#include "Stone.h"

enum GameMode { MainMenu, PlayerStep, Rotation, OponentStep };
static GameMode gameMode;

static Point3D translate = { 0, 0, 0 };
static Point2D oldPos;
static tcursor cursor;
static std::vector<Stone*> stones;
static std::vector<GameObject*> menuButtons;
static std::vector<GameObject*> rotationButtons;

class Controls {
public:

static void errorCallback(int error, const char* errorText) {
	std::cout << "Error: " << errorText << std::endl;
}

static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch(key){
	case GLFW_KEY_ESCAPE:
		if (gameMode == MainMenu) glfwSetWindowShouldClose(window, 1);
		else gameMode = MainMenu;
		break;
	}
}

static void MainMenuCallback(GLFWwindow* window, int button, int action, int mods){
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

static void PlayerStepCallback(GLFWwindow* window, int button, int action, int mods){
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

static void RotationCallback(GLFWwindow* window, int button, int action, int mods){
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

static void OponentStepCallback(GLFWwindow* window, int button, int action, int mods){
	glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch(action) {
		case GLFW_PRESS:

			break;
		}
	}
}

static void mouseMoveCallback(GLFWwindow* window, double x, double y) {
	if(cursor.moving) {
		oldPos = cursor.position;
		glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
		translate.x += (cursor.position.x - oldPos.x) / 100;
		translate.y += (oldPos.y - cursor.position.y) / 100;
	}
}

};

#endif /* CONTROLS_H */
