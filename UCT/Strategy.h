/********************************************************
*	Strategy.h : 策略接口文件                           *
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2010.8                                              *
*********************************************************/

#ifndef STRATEGY_H_
#define	STRATEGY_H_

#define TIME_LIMIT 2500

#include "Point.h"
#include "TreeNode.h"

extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board,
	const int lastX, const int lastY, const int noX, const int noY);

extern "C" __declspec(dllexport) void clearPoint(Point* p);

void clearArray(int M, int N, int** board);

/*
添加你自己的辅助函数
*/
Point uctSearch(const int M, const int N, const int* top, int** board, int lastX, int lastY);
TreeNode *treePolicy(TreeNode *v);
TreeNode *expand(TreeNode *v);
TreeNode *bestChild(TreeNode *v, double c);
int defaultPolicy(TreeNode *v);
void backUp(TreeNode *v, int delta);


#endif