/*
 * labirinth.cpp
 */

#include "Labirinth.h"

#include <iostream>
using namespace std;

#define PAREDE 0
#define LIVRE 1
#define SAIDA 2

Labirinth::Labirinth(int values[10][10])
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			labirinth[i][j] = values[i][j];
	initializeVisited();
}


void Labirinth::initializeVisited()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			visited[i][j] = false;
}




void Labirinth::printLabirinth()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			cout << labirinth[i][j] << " ";

		cout << endl;
	}
}


bool Labirinth::findGoal(int x, int y)
{
	visited[x][y] = true;
	if (labirinth[x][y] == SAIDA) return true;

	// left
	if (x > 0 && labirinth[x - 1][y] != PAREDE && !visited[x - 1][y]) {
		if (findGoal(x - 1, y)) return true;
	}
	// right
	if (x < 9 && labirinth[x + 1][y] != PAREDE && !visited[x + 1][y]) {
		if (findGoal(x + 1, y)) return true;
	}
	// up
	if (y > 0 && labirinth[x][y - 1] != PAREDE && !visited[x][y - 1]) {
		if (findGoal(x, y - 1)) return true;
	}
	// down
	if (y < 9 && labirinth[x][y + 1] != PAREDE && !visited[x][y + 1]) {
		if (findGoal(x, y + 1)) return true;
	}

	return false;
}


