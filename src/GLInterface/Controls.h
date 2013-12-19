/*
 *  Created on: 11 ����. 2013
 *      Author: Gasper
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include <vector>
#include "structs.h"
#include "Stone.h"
#include "../UserInterface.h"

#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"

enum GameMode { MainMenu, PlayerStep, Rotation, OponentStep, Waiting};


extern GameMode gameMode;
extern UserInterface::MenuItem menuPressed;
extern std::vector<Stone*> stones;
extern std::vector<GameObject*> menuButtons;
extern std::vector<GameObject*> rotationButtons;


namespace Controls {

extern void onWindowCloseCallBack(GLFWwindow* window);
extern void errorCallback(int error, const char* errorText);
extern void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
extern void MainMenuCallback(GLFWwindow* window, int button, int action, int mods);
extern void PlayerStepCallback(GLFWwindow* window, int button, int action, int mods);
extern void RotationCallback(GLFWwindow* window, int button, int action, int mods);
extern void OponentStepCallback(GLFWwindow* window, int button, int action, int mods);
extern void mouseMoveCallback(GLFWwindow* window, double x, double y);
extern void WaitingCallback(GLFWwindow* window, int button, int action, int mods);

};

#endif /* CONTROLS_H */
