#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"
#include <time.h>
#include <math.h>

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
	uint64_t *userBitBoard = boardToBitBoard(board, M, N, 1), *machineBitBoard = boardToBitBoard(board, M, N, 2);
	BestChoice bestChoice = alphaBetaSearch(M, N, top, userBitBoard, machineBitBoard, alpha, beta, false, lastX, lastY, 0);
	x = bestChoice.x;
	y = bestChoice.y;

	delete[]userBitBoard;
	delete[]machineBitBoard;

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

void randomArray(vector<int> oldArray, vector<int> &newArray, int l) {
	for (int i = l; i>0; i--) {
		srand(unsigned(time(NULL)));
		int index = rand() % i;
		newArray.push_back(oldArray[index]);
		oldArray.erase(oldArray.begin() + index);
	}
}


uint64_t *boardToBitBoard(int *const *board, int M, int N, int mark) {
	uint64_t *bitBoard = new uint64_t[3];
	int i = 0, j = 0, s = 0;
	for (i = 0; i < 3; i++) {
		bitBoard[i] = 0;
	}
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j] != mark) continue;
			s = (64 * 3 - 1) - (i * 12 + j);
			uint64_t newBits = 1;
			int round = s / 64, remain = s % 64;
			bitBoard[2 - round] = bitBoard[2 - round] | (newBits << remain);
		}
	}
	return bitBoard;
}

uint64_t* topToBitBoard(const int *top, int M, int N) {
	uint64_t *bitBoard = new uint64_t[3];
	int i = 0, s = 0;
	for (i = 0; i < 3; i++) {
		bitBoard[i] = 0;
	}
	for (i = 0; i < N; i++) {
		if (top[i] == M) continue;
		s = (64 * 3 - 1) - (12 * top[i] + i);
		uint64_t newBits = 1;
		int round = s / 64, remain = s % 64;
		bitBoard[2 - round] = bitBoard[2 - round] | (newBits << remain);
	}
	return bitBoard;
}

int **bitBoardToBoard(const uint64_t *bitBoard, int M, int N) {
	int **board = new int *[M], i = 0, j = 0, k = 0, round = 0, remain = 0, v = 0;
	for (i = 0; i < M; i++) {
		board[i] = new int[N];
		for (j = 0; j < N; j++) {
			board[i][j] = 0;
		}
	}
	j = 0, k = 0;
	for (i = 0; i < 64 * 3; i++) {
		j = i / 12;
		k = i % 12;
		if (j >= M || k >= N) continue;
		round = i / 64;
		remain = i % 64;
		v = (bitBoard[round] << remain) >> 63;
		if (v == 1) {
			board[j][k] = 1;
		}
	}
	return board;
}

BestChoice alphaBetaSearch(const int M, const int N, const int *top, const uint64_t *userBitBoard, const uint64_t *machineBitBoard, int alpha, int beta, bool isMin, int lastX, int lastY, int depth) {
	// debug
	// printf("alphaBetaSearch\n");
	int i = 0, j = 0;
	// for debug
	/*int **userBoard = bitBoardToBoard(userBitBoard, M, N);
	int **machineBoard = bitBoardToBoard(machineBitBoard, M, N);
	int userBoardShow[9][10] = { 0 }, machineBoardShow[9][10] = { 0 }, topShow[10] = { 0 };
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			userBoardShow[i][j] = userBoard[i][j];
			machineBoardShow[i][j] = machineBoard[i][j];
		}
	}
	for (i = 0; i < N; i++) {
		topShow[i] = top[i];
	}*/

	// for debug
	/*if (board[2][3] == 1) {
	printf("breakpoint\n");
	}*/

	bool isUserWin = false, isMachineWin = false, tie = false;
	if (lastX >= 0 && lastX < M && lastY >= 0 && lastY < N) {
		if (isMin) isMachineWin = isWinBitBoard(machineBitBoard);
		else isUserWin = isWinBitBoard(userBitBoard);
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
	int k = 0, l = 0, x = -1, y = -1, s = 0;
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
	randomArray(indexArray, shuffledIndexArray, k);
	// for debug
	// shuffledIndexArray = indexArray;

	int bestScore = 0, bestScoreIndex = shuffledIndexArray[0];
	int *newTop = new int[N];
	// int **newBoard = new int*[M];
	uint64_t *newBoard = new uint64_t[3];
	if (isMin) {
		bestScore = INT_MAX;
		bestScoreIndex = 0;
	}
	else {
		bestScore = INT_MIN;
		bestScoreIndex = 0;
	}
	Point point(-1, -1);
	uint64_t former = 0, round = 0, remain = 0;
	for (i = 0; i < k; i++) {
		// reset changed on newTop and newBoard
		if (point.x != -1 && point.y != -1) {
			newTop[point.y] = top[point.y];
			newBoard[2 - round] = former;
		}
		else {
			// initialize newTop and newBoard
			for (j = 0; j < N; j++) {
				newTop[j] = top[j];
			}
			if (isMin) {
				for (j = 0; j < 3; j++) {
					newBoard[j] = userBitBoard[j];
				}
			}
			else {
				for (j = 0; j < 3; j++) {
					newBoard[j] = machineBitBoard[j];
				}
			}
		}
		point = validPoints[shuffledIndexArray[i]];
		newTop[point.y] = top[point.y] - 1;

		// int newBoardValue = isMin ? 1 : 2;
		s = (64 * 3 - 1) - (point.x * 12 + point.y);
		round = s / 64;
		remain = s % 64;
		former = newBoard[2 - round];
		newBoard[2 - round] = newBoard[2 - round] | ((uint64_t)1 << remain);
		BestChoice bestChoice = alphaBetaSearch(M, N, newTop, isMin ? newBoard : userBitBoard, isMin ? machineBitBoard : newBoard, alpha, beta, !isMin, point.x, point.y, depth + 1);
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
	delete[] newBoard;
	return BestChoice(validPoints[bestScoreIndex].x, validPoints[bestScoreIndex].y, bestScore);
}
