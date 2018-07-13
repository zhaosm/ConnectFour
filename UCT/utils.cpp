#include "utils.h"

std::vector<int> randomArray(int l) {
	std::vector<int> indexArray;
	std::vector<int> shuffledIndexArray;
	int i = 0;
	for (i = 0; i < l; i++) {
		indexArray.push_back(i);
	}
	for (int i = l; i > 0; i--) {
		srand(unsigned(time(NULL)));
		int index = rand() % i;
		shuffledIndexArray.push_back(indexArray[index]);
		indexArray.erase(indexArray.begin() + index);
	}
	return shuffledIndexArray;
}