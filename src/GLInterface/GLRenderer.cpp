#include "GLRenderer.h"
#include "../Game.h"
#include "Button.h"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

CrossThreadMutex _mutex;
Step step;
DrawingFunction Redraw;
GLFWwindow* window;
std::vector<int> texture;

void RefreshStones() {
	Game * game = Game::GetInstance();
	for (auto i : stones) {
		int x = (i->GetCenter().x - 120) / 80;
		int y = (i->GetCenter().y - 40) / 80;

		i->IsSet(Stone::Type(sgn(game->board[x][y])));
	}
}

void MainMenuDraw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0); glVertex2d(-2.f, -2.f);
		glTexCoord2d(1.0, 0.0); glVertex2d(2.f, -2.f);
		glTexCoord2d(1.0, 1.0); glVertex2d(2.f, 2.f);
		glTexCoord2d(0.0, 1.0); glVertex2d(-2.f, 2.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	for (auto i : menuButtons)
		i->Draw();
}

void GameRender() {
	glEnable(GL_TEXTURE_2D);
	// background
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0); glVertex2d(-2.f, -2.f);
		glTexCoord2d(1.0, 0.0); glVertex2d(2.f, -2.f);
		glTexCoord2d(1.0, 1.0); glVertex2d(2.f, 2.f);
		glTexCoord2d(0.0, 1.0); glVertex2d(-2.f, 2.f);
	glEnd();
	// board
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0); glVertex2d(-1.f, -1.f);
		glTexCoord2d(1.0, 0.0); glVertex2d(1.f, -1.f);
		glTexCoord2d(1.0, 1.0); glVertex2d(1.f, 1.f);
		glTexCoord2d(0.0, 1.0); glVertex2d(-1.f, 1.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	for (auto i : stones)
		i->Draw();

	if (gameMode == Rotation)
		for (auto btn : rotationButtons)
			btn->Draw();
}

void SetMode(GameMode _mode) {
	gameMode = _mode;
	switch (_mode) {
	case MainMenu:
		Redraw = MainMenuDraw;
		glfwSetMouseButtonCallback(window, Controls::MainMenuCallback);
		break;
	case PlayerStep:
		Redraw = GameRender;
		glfwSetMouseButtonCallback(window, Controls::PlayerStepCallback);
		break;
	case Rotation:
		Redraw = GameRender;
		glfwSetMouseButtonCallback(window, Controls::RotationCallback);
		break;
	case OponentStep:
		Redraw = GameRender;
		glfwSetMouseButtonCallback(window, Controls::WaitingCallback);
		break;
	case Waiting:
		Redraw = MainMenuDraw;
		glfwSetMouseButtonCallback(window, Controls::WaitingCallback);
		break;
	}
}

void stoneClick(GameObject* sender) {
	Game * game = Game::GetInstance();
	int i = (static_cast<Stone*>(sender)->GetCenter().x - 120) / 80;
	int j = (static_cast<Stone*>(sender)->GetCenter().y - 40) / 80;
	if (!game->board[i][j]) {
		step.i = i;
		step.j = j;
		static_cast<Stone*>(sender)->IsSet(Stone::Type::Selected);
		SetMode(Rotation);
	}
}

void rotateClick(GameObject* sender) {
	Point2D t = static_cast<Button*>(sender)->GetVerteces()[0];

	step.quarter = t.x == 0 ? (t.y < 100 ? 1 : 0) : (t.y < 100 ? 2 : 3);
	step.direction =
			t.y == 0 || t.y == 370 ?
					((step.quarter == 2 || step.quarter == 3) ? 1 : 0) :
					((step.quarter == 2 || step.quarter == 3) ? 0 : 1);

	SetMode(OponentStep);
	_mutex.unlock();
}

void InitializeInterface() {
	// loading textures
	texture.push_back(GLutils::LoadTexture("assets/background.raw", 1024, 1024, true));
	texture.push_back(GLutils::LoadTexture("assets/board.raw", 1024, 1024, true));
	texture.push_back(GLutils::LoadTexture("assets/L1.raw", 256, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/R1.raw", 256, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/L2.raw", 256, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/R2.raw", 256, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/new.raw", 1024, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/host.raw", 1024, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/join.raw", 1024, 256, true));	
	texture.push_back(GLutils::LoadTexture("assets/exit.raw", 1024, 256, true));

	// stones
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++) {
			Stone* stn = new Stone( { double(120 + i * 80), double(40 + j * 80) }, 30);
			*stn += stoneClick;
			stones.push_back(stn);
		}

	// main menu
	Button* menuBtn[] = { 
		new Button( { 200, 120 }, { 456, 184 }, texture[6]), 
		new Button( { 200, 200 }, { 456, 264 }, texture[7]),
		new Button( { 200, 280 }, { 456, 344 }, texture[8]), 
		new Button( { 200, 360 }, { 456, 424 }, texture[9])
	};
	*menuBtn[0] += [](GameObject*) {
		menuPressed = UserInterface::MenuItem::LocalGame;
		SetMode(Waiting);
		_mutex.unlock();
	}; // LocalGame
	*menuBtn[1] += [](GameObject*) {
		menuPressed = UserInterface::MenuItem::StartHost;
		SetMode(Waiting);
		_mutex.unlock();
	}; //Start host
	*menuBtn[2] += [](GameObject*) {
		menuPressed = UserInterface::MenuItem::ConnectToServer;
		SetMode(Waiting);
		_mutex.unlock();
	}; // NewGame; //Connect to host
	*menuBtn[3] += [](GameObject*) {
		glfwSetWindowShouldClose(window, true);
		menuPressed = UserInterface::MenuItem::ExitGame;
		_mutex.unlock();
	}; // Exit;
	for (auto i : menuBtn)
		menuButtons.push_back(i);

	// rotation buttons
	Button* rotBtn[] = { 
		new Button( { 0, 0 }, { 50, 50 }, texture[3]), 
		new Button( { 0, 60 }, { 50,	110 }, texture[2]), 
		new Button( { 0, 370 }, { 50, 420 }, texture[3]), 
		new Button( { 0, 430 }, { 50, 480 }, texture[2]), 
		new Button( { 590, 0 }, { 640, 50 }, texture[4]),
		new Button( { 590, 60 }, { 640, 110 }, texture[5]), 
		new Button( { 590, 370 }, { 640, 420 }, texture[4]), 
		new Button( { 590, 430 }, { 640, 480 }, texture[5]) 
	};

	for (auto i : rotBtn) {
		*i += rotateClick;
		rotationButtons.push_back(i);
	}

	SetMode(MainMenu);
}

int GLmain(CrossThreadMutex* _mutex) {
	glfwSetErrorCallback(Controls::errorCallback);

	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_RESIZABLE, false);
	window = glfwCreateWindow(640, 480, "Pentago", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwSetWindowCloseCallback(window, Controls::onWindowCloseCallBack);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, Controls::keyboardCallback);
	glfwSetMouseButtonCallback(window, Controls::MainMenuCallback);
	glfwSetCursorPosCallback(window, Controls::mouseMoveCallback);

	InitializeInterface();
	_mutex->unlock();
	
	//do not need mor for now
	glfwSwapInterval(2);

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glColor3f(1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		Redraw();

		/* End render */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	for (GameObject* obj : stones) delete obj;
	for (GameObject* obj : menuButtons) delete obj;
	for (GameObject* obj : rotationButtons) delete obj;
	return 0;
}
