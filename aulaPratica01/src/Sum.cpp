/*
 * Sum.cpp
 */

#include "Sum.h"
#include <chrono>

string calcSum(int* sequence, int size)
{
	string result;

	for (int m = 1; m <= size; ++m) {
		int max = size - m;
		int bestSum, bestI;

		for (int i = 0; i <= max; ++i) {
			int iSum = 0;

			for (int j = 0; j < m; ++j) {
				iSum += sequence[i + j];
			}

			if (bestSum > iSum || i == 0) {
				bestSum = iSum;
				bestI = i;
			}
		}

		result += to_string(bestSum) + "," + to_string(bestI) + ";";
	}

	return result;
}

