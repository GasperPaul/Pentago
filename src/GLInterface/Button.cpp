/*
 *  Created on: 11 груд. 2013
 *      Author: Gasper
 */

#include "Button.h"

Button::Button(Point2D* _verteces, int _tex) : verteces { _verteces[0], _verteces[1] }, texture(_tex) { }

Button::Button(Point2D a, Point2D b, int _tex) : verteces { a, b }, texture(_tex) { }

Button::~Button() { }

Point2D* Button::GetVerteces() {
	return verteces;
}

void Button::Draw() {
	Point3D tmp[2] = { GLutils::UnProject(verteces[0]), GLutils::UnProject(verteces[1]) };
	if (texture) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
			glTexCoord2d(0.0,0.0); glVertex2d(tmp[0].x, tmp[0].y);
			glTexCoord2d(1.0,0.0); glVertex2d(tmp[1].x, tmp[0].y);
			glTexCoord2d(1.0,1.0); glVertex2d(tmp[1].x, tmp[1].y);
			glTexCoord2d(0.0,1.0); glVertex2d(tmp[0].x, tmp[1].y);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	} else {
		glBegin(GL_QUADS);
			glVertex2d(tmp[0].x, tmp[0].y);
			glVertex2d(tmp[1].x, tmp[0].y);
			glVertex2d(tmp[1].x, tmp[1].y);
			glVertex2d(tmp[0].x, tmp[1].y);
		glEnd();
	}

}

void Button::Clicked(Point2D m_coord) {
	if (m_coord.x < verteces[1].x && m_coord.x > verteces[0].x && m_coord.y < verteces[1].y && m_coord.y > verteces[0].y)
		for(auto event : OnClick)
			event(this);
}

Button Button::operator+=(EventCallback e) {
	OnClick.push_back(e);
	return *this;
}
