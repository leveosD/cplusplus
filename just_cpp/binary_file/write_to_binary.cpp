//Первая программа, записывает данные в бинарный файл
#include <iostream>
#include <stdio.h>

int main() {
	using std::cout;
	using std::cin;

	FILE* file;
	fopen_s(&file, "binary", "wb"); //открываем бинарный файл с возможностью записи
	if (file == NULL)
		return 0;

	cout << "Enter amount of numbers:\n";
	int max;
	cin >> max;

	for (int i = 0; i < max; i++) {
		cout << "Number: ";
		int n;
		cin >> n;
		fwrite(&n, sizeof(n), 1, file); //записываем числа по очереди в файл
	}

	fclose(file);
}