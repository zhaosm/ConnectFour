/********************************************************
*	Strategy.h : 策略接口文件                           *
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2010.8                                              *
*********************************************************/

#ifndef STRATEGY_H_
#define	STRATEGY_H_

#include "Point.h"
#include "BestChoice.h"
#include <vector>

extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board,
	const int lastX, const int lastY, const int noX, const int noY);

extern "C" __declspec(dllexport) void clearPoint(Point* p);

void clearArray(int M, int N, int** board);

/*
添加你自己的辅助函数
*/
void randomArray(std::vector<int> oldArray, std::vector<int> &newArray, int l);
uint64_t *boardToBitBoard(int *const *board, int M, int N, int mark);
uint64_t *topToBitBoard(const int *top, int M, int N);
int **bitBoardToBoard(const uint64_t *bitBoard, int M, int N); // for debug
BestChoice alphaBetaSearch(const int M, const int N, const int *top, const uint64_t *userBitBoard, const uint64_t *machineBitBoard, int alpha, int beta, bool isMin, int lastX, int lastY, int depth);

#endif