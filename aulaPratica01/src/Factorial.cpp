/*
 * Factorial.cpp
 */

#include "Factorial.h"

// O(n) temporal
// O(n) espacial
int factorialRecurs(int n)
{
	if (n < 0) return -1;
	else if (n == 0) return 1;
	else return factorialRecurs(n - 1) * n;
}

// O(n) temporal
// O(1) espacial
int factorialDinam(int n)
{
	if (n < 0) return -1;
	int fact = n;
	for (int i = 1; i < n; ++i)
		fact *= i;
	return fact;
}
