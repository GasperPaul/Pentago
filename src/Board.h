/*
 * Board.h
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#ifndef BOARD_H
#define BOARD_H
#include<iostream>

class Board {
public:
	enum RotateDirection { Left, Right };

	Board();
	virtual ~Board();
	bool putStone(short, short, short);
	void Rotate(short, RotateDirection);
	void DisplayTemp();
	short& operator()(short, short);
	short* operator[](short);

private:
	short board[6][6];
	unsigned step;
};

#endif /* BOARD_H */
