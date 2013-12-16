/*
 *  Created on: 16 груд. 2013
 *      Author: Gasper
 */

#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "Controls.h"

struct Step {
	short i, j;
	short quarter;
	char direction;
};

#include "../CrossThreadMutex.h"
extern CrossThreadMutex _mutex;
extern Step step;



typedef void (*DrawingFunction)(void);

extern int GLmain(CrossThreadMutex*);
extern void SetMode(GameMode);
extern void RefreshStones();

#endif /* GLRENDERER_H */
