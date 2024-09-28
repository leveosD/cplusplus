#include <iostream>

int main() {
	using namespace std;

	const int SIZE = 5;
	float A[SIZE][SIZE] = {
		{-3.8, 0, 5.3, 4.5, 0.5},
		{ 0.2, -1.3, 0, 8.5, 3.5},
		{-1.1, 1.8, 5.1, -8.2, 0.32},
		{0, -0.3, 0, -1.28, 0.52},
		{-0.3, 0.5, 1.8, -7.3, 5.5}
	};

	//выводим матрицу А и параллельно находим минимальный элемент
	//и строку со столбцом, где он расположен
	cout << "A:\n";
	float min = A[0][0];
	int row, column;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (A[i][j] < min) {
				min = A[i][j];
				row = i;
				column = j;
			}
			cout << A[i][j] << "\t";
		}
		cout << endl;
	}

	float B[SIZE - 1][SIZE - 1];
	float avrg = 0;
	cout << "B:\n";
	int k = 0, l = 0; //индексы матрицы В
	for (int i = 0; i < SIZE; i++) { //перебираем матрицу А
		if (i == row) //пропускаем строку с наименьшим эл-ом
			continue;
		for (int j = 0; j < SIZE; j++) {
			if (j == column) //пропускаем столбец с наименьшим эл-ом
				continue;
			B[k][l] = A[i][j]; //записываем эл-т из А в В
			avrg += B[k][l]; //считаем сумму всех эл-ов
			cout << B[k][l] << "\t";
			l++; //увеличиваем индекс столбца для В
		}
		l = 0; //обнуляем индекс столца для В
		k++; //и увеличиваем индекс строки для В
		cout << endl;
	}

	avrg /= (SIZE - 1) * (SIZE - 1); //считаем среднее
	cout << endl << "Average of B: " << avrg;
}