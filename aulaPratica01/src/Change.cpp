/*
 * Change.cpp
 */

#include "Change.h"

string calcChange(int m, int numCoins, int *coinValues)
{
    if (numCoins == 0) return "NULL";

    int* nextCoinToAdd = new int[m + 1];

    int* minCoins = new int[m + 1];
    int* nextIterMinCoins = new int[m + 1];

    for (int k = 0; k <= m; ++k) {
		nextCoinToAdd[k] = -1;
		minCoins[k] = -1;
	}

    nextCoinToAdd[0] = 0;
    minCoins[0] = 0;

    for (int i = 0; i < numCoins; ++i) {
    	nextIterMinCoins[0] = 0;

		for (int k = 1; k <= m; ++k) {
			// minCoins[k] = 1 + min(minCoins[k - c0], minCoins[k - c1], ...)
			int minCoinsK = -1;

			for (int c = 0; c <= i; ++c) {
				int kDiff = k - coinValues[c];
				if (kDiff < 0) break;

				int candidate = 1 + nextIterMinCoins[kDiff];

				// Bad candidate, newMinCoins[kDiff] == -1
				if (candidate == 0) continue;

				// If candidate beats min, replace min.
				if ((minCoinsK == -1) || (minCoinsK > candidate)) {
					minCoinsK = candidate;
				}
			}

			nextIterMinCoins[k] = minCoinsK;

			if (nextIterMinCoins[k] != minCoins[k])
				nextCoinToAdd[k] = coinValues[i];
		}

		for (int k = 0; k <= m; ++k) {
			minCoins[k] = nextIterMinCoins[k];
			nextIterMinCoins[k] = 0;
		}
    }

    string changeString;
    while (m > 0) {
    	if (nextCoinToAdd[m] > 0) {
			changeString += to_string(nextCoinToAdd[m]) + ";";
			m -= nextCoinToAdd[m];
    	}
    	else {
    		changeString = "-";
    		break;
    	}
    }

    delete nextCoinToAdd;
    delete minCoins;
    delete nextIterMinCoins;

    return changeString;
}

