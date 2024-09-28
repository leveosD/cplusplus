//Вариант 7
#include <iostream>
#include <string>

//nрототипы функций
int inp_str(char*, int);
void out_str(const char*, int, int);
int* my_sort(char**, int);
int func(char*);

//константы
static const int STR_LEN = 100;
static const double REDUCING_FACTOR = 1.247;

/*
* Пользователь вводит длину списка строк, а затем сами строки.
* СОзданный список сортируется по убыванию количества чисел в каждой строке. 
*/

int main() {
	using namespace std;
	//ввод максимального размера массива
	cout << "Enter max lenght of the list: ";
	int LIST_LEN;
	cin >> LIST_LEN;
	cin.get();
	
	//ввод значений для массива
	char** arr = new char* [LIST_LEN];
	for (int i = 0; i < LIST_LEN; i++) {
		arr[i] = new char[STR_LEN];
		inp_str(arr[i], STR_LEN);
	}
	
	//вывод массива до и после сортировки
	cout << "Before sort:\n";
	for (int i = 0; i < LIST_LEN; i++)
		out_str(arr[i], STR_LEN, i);

	//сортировка массива и запись результатов в соответствующий массив
	int* results = my_sort(arr, LIST_LEN);
	//вывод отсортированного массива
	cout << "After sort:\n";
	for (int i = 0; i < LIST_LEN; i++)
		out_str(arr[i], STR_LEN, i);
	//вывод количества цифр и второй цифры последней строки
	cout << "Amount of digits: " << results[0] << endl
		<< "The second digit of the last word: " << results[1];
	//удаляем массив с ответами, т.к. он был инициализирован
	delete[] results; // с помощью new[]
}

//функция ввода
int inp_str(char* str, int maxlen) {
	std::cout << "Enter string: ";
	std::cin.get(str, maxlen).get();
	return strlen(str);
}

//функция вывода
void out_str(const char* str, int len, int num) {
	std::cout << num << ": " << str << std::endl;
}

//функция сортировки
int* my_sort(char** arr, int len) {
	int gap = len - 1; //разница между сравнимаемыми элементами изначально наибольшая
	while (gap >= 1) { //разницу должна быть больше 1, чтобы иметь возможность перебирать список
		for (int i = 0; i + gap < len; i++) { //элемент с большим индексом находится в пределах массива
			int count1 = func(arr[i]), count2 = func(arr[i + gap]); //вычисление уол-ва цифр в строках
			if (count1 > count2) { //если в первой строке цифр больше
				//меняем элементы местами
				char* temp = new char;
				temp = arr[i];
				arr[i] = arr[i + gap];
				arr[i + gap] = temp;
			}
		}
		//уменьшаем разрыв между элементами
		gap /= REDUCING_FACTOR;
	}
	//считаем цифры в каждом элементе уже отсортированного массива
	int digits = 0;
	for (int i = 0; i < len; i++) {
		digits += func(arr[i]);
	}
	//нахождение второй цифры последней строки
	int answer = -1;
	int count = 0;
	for (char* c = &arr[len - 1][0]; *c != '\0'; c++) {
		if (isdigit(*c)) {
			if (++count == 2) {
				answer = *c - '0';
				break;
			}
		}
	}
	//создание массива с ответами
	int* results = new int[2];
	results[0] = digits;
	results[1] = answer;
	return results;
}

//функция сравнения
int func(char* s) {
	int count = 0; //переменные-счетчики
	int len = strlen(s);
	for (int i = 0; i < len; i++) { //for до конца строки
		if (isdigit(s[i])) //если символ является числом
			count++;	 //счетчик увеличивается
	}
	return count;
}