#ifndef TREENODE_H
#define	TREENODE_H

#include <vector>
#include <math.h>
#include "utils.h"
#include "Point.h"
#include "Judge.h"

class TreeNode {
public:
	int **board;
	int *top;
	int M, N;
	bool isUser;
	int Nv, Qv;
	TreeNode *parent;
	TreeNode **children;
	std::vector<Point> validPoints;
	std::vector<int> shuffledIndexes;
	int shuffledIndexesIndex; // current shuffedIndexes index, initial -1
	int indexInParentChildren; // -1 for root node
	int status; // 1 for userWin, 2 for machineWin, 3 for tie, 4 for other
	int lastX, lastY;

public:
	TreeNode(const int M, const int N, const int* top, int* const* board, int lastX, int lastY) {
		int i = 0, j = 0, k = 0;
		this->board = new int *[M];
		for (i = 0; i < M; i++) {
			this->board[i] = new int[N];
			for (j = 0; j < N; j++) {
				this->board[i][j] = board[i][j];
			}
		}
		this->top = new int[N];
		for (i = 0; i < N; i++) this->top[i] = top[i];
		this->M = M;
		this->N = N;
		this->isUser = false;
		this->Nv = 0;
		this->Qv = 0;
		this->parent = NULL;
		this->children = NULL;
		std::vector<Point> validPoints(0);
		for (i = N - 1; i >= 0; i--) {
			if (top[i] > 0) {
				this->validPoints.push_back(Point(top[i] - 1, i));
				k++;
			}
		}
		this->shuffledIndexes = randomArray(k);
		this->shuffledIndexesIndex = -1;
		this->indexInParentChildren = -1;
		bool isUserWin = false, isMachineWin = false, tie = false;
		if (lastX >= 0 && lastX < M && lastY >= 0 && lastY < N) {
			isUserWin = userWin(lastX, lastY, M, N, board);
			tie = isTie(N, top);
		}
		if (isUserWin) this->status = 1;
		else if (isMachineWin) this->status = 2;
		else if (tie) this->status = 3;
		else this->status = 4;
		this->lastX = lastX;
		this->lastY = lastY;
	}

	TreeNode(TreeNode *parent, int x, int y) {
		int i = 0, j = 0, k = 0, xx = 0;
		this->board = new int *[parent->M];
		for (i = 0; i < parent->M; i++) {
			this->board[i] = new int[parent->N];
			for (j = 0; j < parent->N; j++) {
				this->board[i][j] = parent->board[i][j];
			}
		}
		this->board[x][y] = parent->isUser ? 1 : 2;
		this->top = new int[parent->N];
		for (i = 0; i < parent->N; i++) this->top[i] = parent->top[i];
		this->top[y] = parent->top[y] - 1;
		bool isUserWin = false, isMachineWin = false, tie = false;
		this->isUser = !(parent->isUser);
		if (this->isUser) isMachineWin = machineWin(x, y, parent->M, parent->N, this->board);
		else isUserWin = userWin(x, y, parent->M, parent->N, this->board);
		tie = isTie(parent->N, this->top);
		if (isUserWin) this->status = 1;
		else if (isMachineWin) this->status = 2;
		else if (tie) this->status = 3;
		else this->status = 4;
		this->M = parent->M;
		this->N = parent->N;
		this->Nv = 0;
		this->Qv = 0;
		this->parent = parent;
		this->children = NULL;
		std::vector<Point> validPoints(0);
		for (i = parent->N - 1; i >= 0; i--) {
			if (this->top[i] > 0) {
				this->validPoints.push_back(Point(this->top[i] - 1, i));
				k++;
			}
		}
		this->shuffledIndexes = randomArray(k);
		this->shuffledIndexesIndex = -1;
		this->indexInParentChildren = -1;
		this->lastX = x;
		this->lastY = y;
	}

	~TreeNode() {
		int i = 0, j = 0;
		for (i = 0; i < this->shuffledIndexesIndex + 1; i++) {
			if (this->children[i]) delete this->children[i];
		}
		delete[]this->children;
		if (this->indexInParentChildren >= 0 && this->parent) {
			this->parent->children[this->indexInParentChildren] = NULL;
		}
		this->parent = NULL;
		delete[]this->top;
		for (i = 0; i < this->M; i++) {
			delete[]this->board[i];
		}
		delete[]this->board;
	}
};


#endif
