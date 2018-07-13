#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"
#include <time.h>
#include <math.h>

using namespace std;

/*
策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误

input:
为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
_board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
board[x][y]表示第x行、第y列的点(从0开始计)
board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
noX, noY : 棋盘上的不可落子点(注:其实这里给出的top已经替你处理了不可落子点，也就是说如果某一步
所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
output:
你的落子点Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board,
	const int lastX, const int lastY, const int noX, const int noY) {
	/*
	不要更改这段代码
	*/
	int x = -1, y = -1;//最终将你的落子点存到x,y中
	int** board = new int*[M];
	for (int i = 0; i < M; i++) {
		board[i] = new int[N];
		for (int j = 0; j < N; j++) {
			board[i][j] = _board[i * N + j];
		}
	}

	/*
	根据你自己的策略来返回落子点,也就是根据你的策略完成对x,y的赋值
	该部分对参数使用没有限制，为了方便实现，你可以定义自己新的类、.h文件、.cpp文件
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
	不要更改这段代码
	*/
	clearArray(M, N, board);
	return new Point(x, y);
}


/*
getPoint函数返回的Point指针是在本dll模块中声明的，为避免产生堆错误，应在外部调用本dll中的
函数来释放空间，而不应该在外部直接delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p) {
	delete p;
	return;
}

/*
清除top和board数组
*/
void clearArray(int M, int N, int** board) {
	for (int i = 0; i < M; i++) {
		delete[] board[i];
	}
	delete[] board;
}


/*
添加你自己的辅助函数，你可以声明自己的类、函数，添加新的.h .cpp文件来辅助实现你的想法
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
