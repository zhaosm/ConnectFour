#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"
#include <time.h>

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
	}*/

	int alpha = INT_MIN, beta = INT_MAX;
	BestChoice bestChoice = alphaBetaSearch(M, N, top, board, alpha, beta, false, lastX, lastY, 0);
	x = bestChoice.x;
	y = bestChoice.y;

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

void RandomArray(vector<int> oldArray, vector<int> &newArray, int l) {
	for (int i = l; i>0; i--) {
		srand(unsigned(time(NULL)));
		int index = rand() % i;
		newArray.push_back(oldArray[index]);
		oldArray.erase(oldArray.begin() + index);
	}
}


BestChoice alphaBetaSearch(const int M, const int N, const int* top, int** board, int alpha, int beta, bool isMin, int lastX, int lastY, int depth) {
	// debug
	// printf("alphaBetaSearch\n");
	int i = 0, j = 0;
	// for debug
	/*int board_show[6][7] = { 0 }, top_show[7] = { 0 };
	for (i = 0; i < M; i++) {
	for (j = 0; j < N; j++) {
	board_show[i][j] = board[i][j];
	}
	}
	for (i = 0; i < N; i++) {
	top_show[i] = top[i];
	}*/

	// for debug
	/*if (board[2][3] == 1) {
	printf("breakpoint\n");
	}*/

	bool isUserWin = false, isMachineWin = false, tie = false;
	if (lastX >= 0 && lastX < M && lastY >= 0 && lastY < N) {
		if (!isMin) isUserWin = userWin(lastX, lastY, M, N, board);
		else isMachineWin = machineWin(lastX, lastY, M, N, board);
		tie = isTie(N, top);
	}
	if (isUserWin) {
		return BestChoice(-1, -1, -10);
	}
	else if (isMachineWin) {
		return BestChoice(-1, -1, 10);
	}
	else if (tie) {
		return BestChoice(-1, -1, 0);
	}
	if (depth > 7) {
		return BestChoice(-1, -1, 0);
	}

	vector<Point> validPoints;
	int k = 0, l = 0, x = -1, y = -1;
	for (i = N - 1; i >= 0; i--) {
		if (top[i] > 0) {
			x = top[i] - 1;
			y = i;
			validPoints.push_back(Point(x, y));
			k++;
		}
	}

	vector<int> indexArray;
	vector<int> shuffledIndexArray;
	for (i = 0; i < k; i++) {
		indexArray.push_back(i);
	}
	RandomArray(indexArray, shuffledIndexArray, k);

	int bestScore = 0, bestScoreIndex = shuffledIndexArray[0];
	int *newTop = new int[N];
	int **newBoard = new int*[M];
	for (i = 0; i < M; i++) {
		newBoard[i] = new int[N];
	}
	if (isMin) {
		bestScore = INT_MAX;
		bestScoreIndex = 0;
	}
	else {
		bestScore = INT_MIN;
		bestScoreIndex = 0;
	}
	Point point(-1, -1);
	for (i = 0; i < k; i++) {
		// reset changed values
		if (point.x != -1 && point.y != -1) {
			newTop[point.y] = top[point.y];
			newBoard[point.x][point.y] = 0;
		}
		else {
			// initialize newTop and newBoard
			for (j = 0; j < N; j++) {
				newTop[j] = top[j];
			}
			for (j = 0; j < M; j++) {
				for (l = 0; l < N; l++) {
					newBoard[j][l] = board[j][l];
				}
			}
		}
		Point point = validPoints[shuffledIndexArray[i]];
		newTop[point.y] = top[point.y] - 1;
		int newBoardValue = isMin ? 1 : 2;
		newBoard[point.x][point.y] = newBoardValue;
		BestChoice bestChoice = alphaBetaSearch(M, N, newTop, newBoard, alpha, beta, !isMin, point.x, point.y, depth + 1);
		int score = bestChoice.score;
		if (isMin) {
			if (score < bestScore) {
				bestScore = score;
				bestScoreIndex = shuffledIndexArray[i];
				beta = score;
			}
		}
		else {
			if (score > bestScore) {
				bestScore = score;
				bestScoreIndex = shuffledIndexArray[i];
				alpha = score;
			}
		}
		if (alpha >= beta) {
			break;
		}
	}

	delete[] newTop;
	clearArray(M, N, newBoard);
	return BestChoice(validPoints[bestScoreIndex].x, validPoints[bestScoreIndex].y, bestScore);
}
