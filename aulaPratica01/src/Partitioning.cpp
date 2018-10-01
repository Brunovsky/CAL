/*
 * Partioning.cpp
 */

#include "Partitioning.h"

int min(int a, int b) {
	return a < b ? a : b;
}

int max(int a, int b) {
	return a > b ? a : b;
}

int s_recursive(int n, int k)
{
	if (n <= 0) return 0;
	if (k <= 0 || k > n) return 0;
	if (k == 1 || k == n) return 1;
	return s_recursive(n - 1, k - 1) + k * s_recursive(n - 1, k);
}

int s_dynamic(int n, int k)
{
	int* s = new int[k + 1];
	int* sNew = new int[k + 1];

	for (int i = 0; i <= k; ++i) {
		s[i] = 0; sNew[i] = 0;
	}

	s[1] = 1;

	for (int i = 2; i <= n; ++i) {
		int start = max(1, i - (n - k));
		int end = min(i, k);

		for (int j = start; j <= end; ++j) {
			sNew[j] = s[j - 1] + j * s[j];
		}

		for (int j = start; j <= end; ++j)
			s[j] = sNew[j];
	}

	int result = s[k];

	delete s;
	delete sNew;

	return result;
}


int b_recursive(int n)
{
	int total = 0;
	for (int k = 1; k <= n; ++k) total += s_recursive(n, k);
	return total;
}

int b_dynamic(int n)
{
	int* s = new int[n + 1];
	int* sNew = new int[n + 1];

	for (int i = 0; i <= n; ++i) {
		s[i] = 0; sNew[i] = 0;
	}

	s[1] = 1;

	for (int i = 2; i <= n; ++i) {
		for (int j = 1; j <= i; ++j) {
			sNew[j] = s[j - 1] + j * s[j];
		}

		for (int j = 1; j <= i; ++j)
			s[j] = sNew[j];
	}

	int result = 0;
	for (int i = 1; i <= n; ++i)
		result += s[i];

	delete s;
	delete sNew;

	return result;
}


