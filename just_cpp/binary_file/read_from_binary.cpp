//Вторая программа считывает данный из файла и 
#include <iostream>
#include <stdio.h>

int main() {
	using std::cout;
	using std::cin;

	FILE* bin;
	FILE* text;
	fopen_s(&bin, "binary", "rb"); //читаем бинарный файл
	fopen_s(&text, "text.txt", "w"); //записываем в текстовый
	if (bin == NULL || text == NULL)
		return 0;
	int max_arr[100]; //массив с наибольшим количеством подряд идущих чисел(>0)
	int cur_arr[100]; //массив с текущим рядом положительных чисел
	memset(cur_arr, 0, 100 * sizeof(int));
	memset(max_arr, 0, 100 * sizeof(int)); //заполняем оба массива нулями
	int n;
	int i = 0;
	int max_size = 0;
	while (fread(&n, sizeof(n), 1, bin)) { //читаем по числу
		cout << n << std::endl;
		if (n > 0) { //если число >0 записываем в текущий список
			cur_arr[i] = n;
			i++;
		}
		else {
			if (i > max_size) { //если ряд положительных чисел закончился и он больше наибольшего
				max_size = i;
				for (int k = 0; k < max_size; k++)
					max_arr[k] = cur_arr[k]; // то записываем в наибольший текущий массив
			}
			i = 0;
			memset(cur_arr, 0, 100 * sizeof(int)); //просто обнуляем текущий
		}
	}
	//если числа закончились на самом длинном списке,то записываем его
	if (i > max_size) {
		max_size = i;
		for (int k = 0; k < max_size; k++)
			max_arr[k] = cur_arr[k];
	}
	cout << "----------\n";
	//вывод наибольшего массива на экран и в файл
	for (int k = 0; k < max_size; k++) {
		cout << max_arr[k] << std::endl;
		int len = 0; //длина числа в символах
		int x = max_arr[k];
		while (x > 0) {
			x = (int)(x / 10);
			len++;
		}
		char* str = new char[len + 1]; //строка под каждое число
		sprintf_s(str, len + 1, "%d", max_arr[k]); //получаем наше число в символах
		fwrite(str, len, 1, text); // и записываем их в файл
		fputs("\n", text);
	}
	//закрываем оба файла
	fclose(text);
	fclose(bin);
} 