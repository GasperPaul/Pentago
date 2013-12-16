/*
 *  Created on: 11 груд. 2013
 *      Author: Gasper
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <vector>
#include "GameObject.h"

class Button : public GameObject {
public:
	Button(Point2D*, int _tex = 0);
	Button(Point2D, Point2D, int _tex = 0);
	Button operator+=(EventCallback);
	Point2D* GetVerteces();
	virtual void Draw();
	virtual void Clicked(Point2D);
	virtual ~Button();
private:
	std::vector<EventCallback> OnClick;
	Point2D verteces[2];
	int texture;
};

#endif /* BUTTON_H */
