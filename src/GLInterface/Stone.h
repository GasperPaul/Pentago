/*
 *  Created on: 9 груд. 2013
 *      Author: Gasper
 */

#ifndef STONE_H
#define STONE_H

#include <vector>
#include <cmath>
#include "GameObject.h"

class Stone : public GameObject {
public:
	enum Type { Black=-1, Empty=0, White=1, Selected=2};

	Stone(Point2D, double);
	Stone(double, double, double);
	virtual ~Stone();

	virtual void Draw();
	virtual void Clicked(Point2D);
	Stone operator+=(EventCallback click);
	Point2D GetCenter();
	double GetRadius();
	void IsSet(Type);
private:
	Point2D center;
	double radius;
	std::vector<EventCallback> OnClick;
	Type isSet = Empty;
};

#endif /* STONE_H */
