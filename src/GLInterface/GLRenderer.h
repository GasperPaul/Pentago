/*
 *  Created on: 16 груд. 2013
 *      Author: Gasper
 */

#ifndef GLRENDERER_H
#define GLRENDERER_H

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

//#include "Button.h"
//#include "Stone.h"
#include "Controls.h"
//#include "GLutils.h"
#include "../CrossThreadMutex.h"


typedef void (*DrawingFunction)(void);

//class GLRenderer {
//private:
	struct Step{
		short i,j;
		short quarter;
		char direction;
	};

//public:
	extern int GLmain(CrossThreadMutex*);
	extern void SetMode(GameMode);
	extern void RefreshStones();
	extern CrossThreadMutex _mutex;
	extern Step step;
//};

#endif /* GLRENDERER_H */
