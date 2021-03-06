#pragma once

#include "resource.h"

struct Square {
	int left;
	int top;
	int right;
	int bottom;
	COLORREF color;
};

BOOL squaresOverlap(Square * first, Square * second);

BOOL squareIntersectsWithSomeOthers(Square newSquare);

Square generateRandomSquare();

int clickedSquareIndex(LPARAM params);

BOOL squareContainsPoint(Square * square,int x, int y);
