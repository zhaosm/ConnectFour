#include <iostream>
#include "Point.h"
#include "Strategy.h"

using namespace std;

/*
���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������

input:
Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
output:
������ӵ�Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board,
	const int lastX, const int lastY, const int noX, const int noY) {
	/*
	��Ҫ������δ���
	*/
	int x = -1, y = -1;//���ս�������ӵ�浽x,y��
	int** board = new int*[M];
	for (int i = 0; i < M; i++) {
		board[i] = new int[N];
		for (int j = 0; j < N; j++) {
			board[i][j] = _board[i * N + j];
		}
	}

	/*
	�������Լ��Ĳ������������ӵ�,Ҳ���Ǹ�����Ĳ�����ɶ�x,y�ĸ�ֵ
	�ò��ֶԲ���ʹ��û�����ƣ�Ϊ�˷���ʵ�֣�����Զ����Լ��µ��ࡢ.h�ļ���.cpp�ļ�
	*/
	//Add your own code below
	/*
	//a naive example
	for (int i = N-1; i >= 0; i--) {
	if (top[i] > 0) {
	x = top[i] - 1;
	y = i;
	break;
	}
	}
	*/
	Point p = uctSearch(M, N, top, board, lastX, lastY);
	x = p.x, y = p.y;

	/*
	��Ҫ������δ���
	*/
	clearArray(M, N, board);
	return new Point(x, y);
}


/*
getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p) {
	delete p;
	return;
}

/*
���top��board����
*/
void clearArray(int M, int N, int** board) {
	for (int i = 0; i < M; i++) {
		delete[] board[i];
	}
	delete[] board;
}


/*
������Լ��ĸ�������������������Լ����ࡢ����������µ�.h .cpp�ļ�������ʵ������뷨
*/

Point uctSearch(const int M, const int N, const int* top, int** board, int lastX, int lastY) {
	int startTime = clock(), delta = 0;
	TreeNode root(M, N, top, board, lastX, lastY);
	while (clock() - startTime <= TIME_LIMIT) {
		TreeNode *v = treePolicy(&root);
		delta = defaultPolicy(v);
		backUp(v, delta);
	}
	TreeNode *bChild = bestChild(&root, 0);
	return Point(bChild->lastX, bChild->lastY);
}

TreeNode *treePolicy(TreeNode *v) {
	int validPointsNum = 0;
	while (v->status == 4) {
		validPointsNum = v->validPoints.size();
		if (v->shuffledIndexesIndex < validPointsNum - 1) return expand(v);
		else v = bestChild(v, 0.8);
	}
	return v;
}

TreeNode *expand(TreeNode *v) {
	v->shuffledIndexesIndex += 1;
	if (!(v->children)) v->children = new TreeNode*[v->validPoints.size()];
	Point validPoint = v->validPoints[v->shuffledIndexes[v->shuffledIndexesIndex]];
	TreeNode *newChild = new TreeNode(v, validPoint.x, validPoint.y);
	newChild->indexInParentChildren = v->shuffledIndexesIndex;
	v->children[v->shuffledIndexesIndex] = newChild;
	return newChild;
}

TreeNode *bestChild(TreeNode *v, double c) {
	int i = 0, maxIndex = -1;
	double max = INT_MIN, temp = 0;
	TreeNode *child = NULL;
	for (i = 0; i < v->shuffledIndexesIndex + 1; i++) {
		child = v->children[i];
		if (!child) continue;
		temp = (double)child->Qv / (double)child->Nv + c * pow(2 * log((double)v->Nv) / (double)child->Nv, 0.5);
		if (temp > max) {
			maxIndex = i;
			max = temp;
		}
	}
	return (v->children)[maxIndex];
}

int defaultPolicy(TreeNode *v) {
	int i = 0, j = 0, index = -1, k = 0, x = v->lastX, y = v->lastY;
	bool isUserWin = false, isMachineWin = false, tie = false, isUser = v->isUser;
	int **board = new int *[v->M], *top = new int[v->N];
	for (i = 0; i < v->M; i++) {
		board[i] = new int[v->N];
		for (j = 0; j < v->N; j++) {
			board[i][j] = v->board[i][j];
		}
	}
	for (i = 0; i < v->N; i++) top[i] = v->top[i];
	std::vector<Point> validPoints(0);
	while (true) {
		if (!isUser) isUserWin = userWin(x, y, v->M, v->N, board);
		else isMachineWin = machineWin(x, y, v->M, v->N, board);
		tie = isTie(v->N, top);
		if (isUserWin) break;
		else if (isMachineWin) break;
		else if (tie) break;

		validPoints.clear();
		k = 0;
		for (i = v->N - 1; i >= 0; i--) {
			if (top[i] > 0) {
				validPoints.push_back(Point(top[i] - 1, i));
				k++;
			}
		}
		srand(unsigned(time(NULL)));
		index = rand() % k;
		x = validPoints[index].x;
		y = validPoints[index].y;
		board[x][y] = isUser ? 1 : 2;
		top[y] -= 1;
		isUser = !isUser;
	}
	delete[]top;
	clearArray(v->M, v->N, board);
	if (!(v->isUser) && isUserWin) return 1;
	else if (v->isUser && isMachineWin) return 1;
	else if (tie) return 0;
	else return -1;
}

void backUp(TreeNode *v, int delta) {
	while (v) {
		v->Nv = v->Nv + 1;
		v->Qv = v->Qv + delta;
		delta = -delta;
		v = v->parent;
	}
}
