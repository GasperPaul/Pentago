/*
 *  Created on: 16 груд. 2013
 *      Author: Gasper
 */

#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "Controls.h"

typedef void (*DrawingFunction)(void);

struct Step {
	short i, j;
	short quarter;
	char direction;
};

extern CrossThreadMutex _mutex;
extern Step step;
extern int GLmain(CrossThreadMutex*);
extern void SetMode(GameMode);
extern void RefreshStones();

#endif /* GLRENDERER_H */
