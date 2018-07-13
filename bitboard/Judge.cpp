#include "Judge.h"

bool userWin(const int x, const int y, const int M, const int N, int* const* board) {
	//ºáÏò¼ì²â
	int i, j;
	int count = 0;
	for (i = y; i >= 0; i--)
		if (!(board[x][i] == 1))
			break;
	count += (y - i);
	for (i = y; i < N; i++)
		if (!(board[x][i] == 1))
			break;
	count += (i - y - 1);
	if (count >= 4) return true;

	//×İÏò¼ì²â
	count = 0;
	for (i = x; i < M; i++)
		if (!(board[i][y] == 1))
			break;
	count += (i - x);
	if (count >= 4) return true;

	//×óÏÂ-ÓÒÉÏ
	count = 0;
	for (i = x, j = y; i < M && j >= 0; i++, j--)
		if (!(board[i][j] == 1))
			break;
	count += (y - j);
	for (i = x, j = y; i >= 0 && j < N; i--, j++)
		if (!(board[i][j] == 1))
			break;
	count += (j - y - 1);
	if (count >= 4) return true;

	//×óÉÏ-ÓÒÏÂ
	count = 0;
	for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
		if (!(board[i][j] == 1))
			break;
	count += (y - j);
	for (i = x, j = y; i < M && j < N; i++, j++)
		if (!(board[i][j] == 1))
			break;
	count += (j - y - 1);
	if (count >= 4) return true;

	return false;
}

bool machineWin(const int x, const int y, const int M, const int N, int* const* board) {
	//ºáÏò¼ì²â
	int i, j;
	int count = 0;
	for (i = y; i >= 0; i--)
		if (!(board[x][i] == 2))
			break;
	count += (y - i);
	for (i = y; i < N; i++)
		if (!(board[x][i] == 2))
			break;
	count += (i - y - 1);
	if (count >= 4) return true;

	//×İÏò¼ì²â
	count = 0;
	for (i = x; i < M; i++)
		if (!(board[i][y] == 2))
			break;
	count += (i - x);
	if (count >= 4) return true;

	//×óÏÂ-ÓÒÉÏ
	count = 0;
	for (i = x, j = y; i < M && j >= 0; i++, j--)
		if (!(board[i][j] == 2))
			break;
	count += (y - j);
	for (i = x, j = y; i >= 0 && j < N; i--, j++)
		if (!(board[i][j] == 2))
			break;
	count += (j - y - 1);
	if (count >= 4) return true;

	//×óÉÏ-ÓÒÏÂ
	count = 0;
	for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
		if (!(board[i][j] == 2))
			break;
	count += (y - j);
	for (i = x, j = y; i < M && j < N; i++, j++)
		if (!(board[i][j] == 2))
			break;
	count += (j - y - 1);
	if (count >= 4) return true;

	return false;
}

bool isTie(const int N, const int* top) {
	bool tie = true;
	for (int i = 0; i < N; i++)
	{
		if (top[i] > 0)
		{
			tie = false;
			break;
		}
	}
	return tie;
}

bool isWinBitBoard(const uint64_t *ub) {
	// view userBitBoard as a 16 * 12 board, with some margin
	// horizontal
	if ((ub[0] & (ub[0] >> 1) & (ub[0] >> 2) & (ub[0] >> 3)) | (ub[1] & ((ub[1] >> 1) | (ub[0] << (64 - 1))) & ((ub[1] >> 2) | (ub[0] << (64 - 2))) & ((ub[1] >> 3) | (ub[0] << (64 - 3)))) | (ub[2] & ((ub[2] >> 1) | (ub[1] << (64 - 1))) & ((ub[2] >> 2) | (ub[1] << (64 - 2))) & ((ub[2] >> 3) | (ub[1] << (64 - 3))))) return true;
	// vertical
	if ((ub[0] & (ub[0] >> 12) & (ub[0] >> 24) & (ub[0] >> 36)) | (ub[1] & ((ub[1] >> 12) | (ub[0] << (64 - 12))) & ((ub[1] >> 24) | (ub[0] << (64 - 24))) & ((ub[1] >> 36) | (ub[0] << (64 - 36)))) | (ub[2] & ((ub[2] >> 12) | (ub[1] << (64 - 12))) & ((ub[2] >> 24) | (ub[1] << (64 - 24))) & ((ub[2] >> 36) | (ub[1] << (64 - 36))))) return true;
	// right top to left bottom
	if ((ub[0] & (ub[0] >> 11) & (ub[0] >> 22) & (ub[0] >> 33)) | (ub[1] & ((ub[1] >> 11) | (ub[0] << (64 - 11))) & ((ub[1] >> 22) | (ub[0] << (64 - 22))) & ((ub[1] >> 33) | (ub[0] << (64 - 33)))) | (ub[2] & ((ub[2] >> 11) | (ub[1] << (64 - 11))) & ((ub[2] >> 22) | (ub[1] << (64 - 22))) & ((ub[2] >> 33) | (ub[1] << (64 - 33))))) return true;
	// left top to right bottom
	if ((ub[0] & (ub[0] >> 13) & (ub[0] >> 26) & (ub[0] >> 39)) | (ub[1] & ((ub[1] >> 13) | (ub[0] << (64 - 13))) & ((ub[1] >> 26) | (ub[0] << (64 - 26))) & ((ub[1] >> 39) | (ub[0] << (64 - 39)))) | (ub[2] & ((ub[2] >> 13) | (ub[1] << (64 - 13))) & ((ub[2] >> 26) | (ub[1] << (64 - 26))) & ((ub[2] >> 39) | (ub[1] << (64 - 39))))) return true;
	return false;
}

bool isTieBitBoard(const uint64_t *ub, int N) {
	int i = 0, s = 0;
	uint64_t r = 0;
	for (i = 0; i < N; i++) {
		s = (64 * 3 - 1) - i;
		int round = s / 64, remain = s % 64;
		if ((ub[2 - round] & ((uint64_t)1 << remain)) == 0) return false;
	}
	return true;
}