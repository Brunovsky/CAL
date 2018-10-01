/*
 * Sudoku.cpp
 *
 */

#include "Sudoku.h"

/** Inicia um Sudoku vazio.
 */
Sudoku::Sudoku()
{
	this->initialize();

	countFilled = -1;
}

/**
 * Inicia um Sudoku com um conteúdo inicial.
 * Lança excepção IllegalArgumentException se os valores
 * estiverem fora da gama de 1 a 9 ou se existirem números repetidos
 * por linha, coluna ou bloc 3x3.
 *
 * @param nums matriz com os valores iniciais (0 significa por preencher)
 */
Sudoku::Sudoku(int nums[9][9])
{
	this->initialize();

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (nums[i][j] != 0)
			{
				int n = nums[i][j];
				numbers[i][j] = n;
				lineHasNumber[i][n] = true;
				columnHasNumber[j][n] = true;
				block3x3HasNumber[i / 3][j / 3][n] = true;
				countFilled++;
			}
		}
	}
}

void Sudoku::initialize()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int n = 0; n < 10; n++)
			{
				numbers[i][j] = 0;
				lineHasNumber[i][n] = false;
				columnHasNumber[j][n] = false;
				block3x3HasNumber[i / 3][j / 3][n] = false;
			}
		}
	}

	this->countFilled = 0;
}

/**
 * Obtem o conteúdo actual (só para leitura!).
 */
int** Sudoku::getNumbers()
{
	int** ret = new int*[9];

	for (int i = 0; i < 9; i++)
	{
		ret[i] = new int[9];

		for (int a = 0; a < 9; a++)
			ret[i][a] = numbers[i][a];
	}

	return ret;
}

/**
 * Verifica se o Sudoku já está completamente resolvido
 */
bool Sudoku::isComplete()
{
	return countFilled == 9 * 9;
}



/**
 * Resolve o Sudoku.
 * Retorna indicação de sucesso ou insucesso (sudoku impossível).
 */
bool Sudoku::solve()
{
	if (isComplete()) return true;

	bool possibMIN[10] = {true, true, true, true, true, true, true, true, true, true};
	int numpossibMIN = 10;
	int MIN[2] = {-1, -1};

	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (numbers[i][j] != 0) continue;

			bool possib[10] = { true, true, true, true, true, true, true, true, true, true };
			int numpossib = 9;

			for (int n = 1; n < 10; ++n) {
				bool b = columnHasNumber[j][n]
						|| lineHasNumber[i][n]
						|| block3x3HasNumber[i / 3][j / 3][n];
				if (b) {
					numpossib--;
					possib[n] = false;
				}
			}

			if (numpossib < numpossibMIN) {
				for (int k = 0; k < 10; ++k) possibMIN[k] = possib[k];
				numpossibMIN = numpossib;
				MIN[0] = i; MIN[1] = j;
			}

			if (numpossibMIN == 1) break;
		}

		if (numpossibMIN == 1) break;
	}


	for (int n = 1; n < 10; ++n) {
		if (possibMIN[n]) {
			numbers[MIN[0]][MIN[1]] = n;
			countFilled++;
			columnHasNumber[MIN[1]][n] = true;
			lineHasNumber[MIN[0]][n] = true;
			block3x3HasNumber[MIN[0] / 3][MIN[1] / 3][n] = true;

			bool b = solve();
			if (b) return true;
			else {
				numbers[MIN[0]][MIN[1]] = 0;
				countFilled--;
				columnHasNumber[MIN[1]][n] = false;
				lineHasNumber[MIN[0]][n] = false;
				block3x3HasNumber[MIN[0] / 3][MIN[1] / 3][n] = false;
			}
		}
	}

	return false;
}



/**
 * Imprime o Sudoku.
 */
void Sudoku::print()
{
	for (int i = 0; i < 9; i++)
	{
		if (i == 3 || i == 6) cout << endl;
		for (int a = 0; a < 9; a++) {
			if (a == 3 || a == 6) cout << "  ";
			cout << this->numbers[i][a] << " ";
		}
		cout << endl;
	}

	cout << endl;
}


/*
int main() {
	int nums[9][9] = {
	{ 6, 0, 0,  0, 0, 8,  5, 3, 0 },
	{ 0, 0, 5,  0, 0, 0,  0, 0, 0 },
	{ 0, 1, 8,  3, 5, 0,  0, 4, 0 },

	{ 0, 0, 0,  0, 7, 1,  0, 0, 0 },
	{ 4, 0, 0,  0, 0, 0,  0, 0, 9 },
	{ 0, 0, 0,  4, 2, 0,  0, 0, 0 },

	{ 0, 4, 0,  0, 1, 5,  8, 9, 0 },
	{ 0, 0, 0,  0, 0, 0,  2, 0, 0 },
	{ 0, 8, 3,  6, 0, 0,  0, 0, 1 },
	};

	Sudoku sudoku{nums};
	bool b = sudoku.solve();

	cout << "b = " << b << endl;
	sudoku.print();

	cin.get();
	return 0;
}
*/
