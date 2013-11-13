/*
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#ifndef BOARD_H
#define BOARD_H

class Board {
public:
	enum RotateDirection {Left, Right};

	Board();
	virtual ~Board();
	bool putStone(short, short, short);
	void Rotate(short, RotateDirection);
	const short operator()(short, short) const;
	const short* operator[](short) const;

private:
	short board[6][6];
	unsigned step;
};

#endif /* BOARD_H */
