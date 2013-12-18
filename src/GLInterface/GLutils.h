/*
 *  Created on: 9 ����. 2013
 *      Author: Gasper
 */

#ifndef GLUTILS_H
#define GLUTILS_H

#include "GL/glu.h"
#include <fstream>
#include "structs.h"

class GLutils {
public:

	static Point3D UnProject(Point2D cursorPosition) {
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);

		winX = (float)cursorPosition.x;
		winY = (float)viewport[3] - (float)cursorPosition.y;
		glReadPixels(cursorPosition.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		return { posX, posY, posZ };
	}

	static GLuint LoadTexture(const char* filename, int width, int height, int wrap) {
		std::ifstream file;
		file.open(filename, std::ios::binary);
		char* data = new char[width * height * 3];
		file.read(data, width * height * 3);
		file.close();

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		delete[] data;
		return texture;
	}
};

#endif /* GLUTILS_H */
