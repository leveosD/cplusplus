#include <iostream>

int main() {
	using namespace std;

	const int LEN = 10;
	float arr[LEN] = { 1.45, -5.21, 5.33, 20000.1, -7.777, 32.1, 10, -1.969, -0.2222, 1.2 };

	//Число положительных эл-ов
	int count = 0;
	for (int i = 0; i < LEN; i++) {
		if (arr[i] > 0)
			count++;
	}
	cout << "Numbers which more 0: " << count << endl;

	//Сумма эл-ов после 0
	float sum = 0;
	bool zero = false; //проверка на наличие нуля
	for (int i = 0; i < LEN; i++) {
		if (arr[i] == 0) { //если встретился ноль
			sum = 0; //обнуляем сумму
			zero = true; 
		}
		sum += arr[i];
	}
	if (!zero) //если ноль не встретился
		sum = 0; //сумма равна нулю
	cout << "Sum after last 0: " << sum << endl;

	//Сортировка массива
	for (int i = 0; i < LEN / 2 + 1; i++) { //проходим первую половину массива 
		if (arr[i] < 1 && arr[i] > -1) //если модуль числа <1 
			continue; //оставляем его в 1 половине
		for (int j = LEN / 2 + 1; j < LEN; j++) { //если нет, то пробегаем 2 половину
			if ((arr[j] > -1 && arr[j] < 1)) { //в поисках чисел с модулем <1
				float temp = arr[j]; //чтобы поместить их в 1 половину
				arr[j] = arr[i];
				arr[i] = temp;
				break; //если число уже поменяли, то ещё одно искать не нужно
			}
		}
	}

	cout << "Array after sort: ";
	for (int i = 0; i < LEN; i++)
		cout << arr[i] << " ";
}