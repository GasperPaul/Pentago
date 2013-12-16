/*
 *  Created on: 9 груд. 2013
 *      Author: Gasper
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "GLutils.h"
#include "structs.h"

class GameObject {
public:
	virtual void Draw() = 0;
	virtual void Clicked(Point2D) = 0;
	virtual ~GameObject() { };
};

typedef void (*EventCallback)(GameObject* sender);


#endif /* GAMEOBJECT_H */
