/*
 *  Created on: 9 груд. 2013
 *      Author: Gasper
 */

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct Point3D{
	double x;
	double y;
	double z;
} Point3D;

typedef struct Point2D {
	double x;
	double y;
} Point2D;

typedef struct cursor {
	Point2D position;
	bool moving;
}  tcursor;

#endif /* STRUCTS_H */
