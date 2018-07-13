#ifndef BESTCHOICE_H
#define BESTCHOICE_H
#include "Point.h"

class BestChoice {
public:
	int x;
	int y;
	int score;

	BestChoice(int x, int y, int score) {
		this->x = x;
		this->y = y;
		this->score = score;
	}
};

#endif 
