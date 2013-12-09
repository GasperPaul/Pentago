/*
 * Board.h
 *
 *  Created on: 10 ����. 2013
 *      Author: Gasper
 */

#ifndef BOARD_H
#define BOARD_H

class Board {
public:
	enum RotateDirection { Left, Right };

	Board();
	virtual ~Board();
	bool putStone(short, short, short);
	void Rotate(short, RotateDirection);
	
	//TODO: ����� ������� ���� � ��� const
	short& operator()(short, short);
	short* operator[](short);
	
private:
	short board[6][6];
	unsigned stepNum;
	
};

#endif /* BOARD_H */
