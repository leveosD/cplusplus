#include <iostream>
#include <string>

//определение структуры
static struct Sportsman {
    std::string surname;
    std::string sport;
    int medals;
    int prize;
};

//прототип функции вывода массива
void write_list(Sportsman**, int);

int main()
{
    using namespace std;

    //инициализация списка структур. размер массива определяется вводом пользователя
    int LIST_SIZE;
    do {
        cout << "Enter amount of sportsmans: ";
        cin >> LIST_SIZE;
        if (cin.fail()) {
            cin.clear();
            while (cin.get() != '\n');
        }
        else
            break;
    } while (true);
    //cin.get();
    Sportsman** list = new Sportsman * [LIST_SIZE];

    //ввод значений базы данных
    for (int i = 0; i < LIST_SIZE; i++) {
        list[i] = new Sportsman;
        do
        {
            cin.get();
            cout << "Surname: ";
            getline(cin, list[i]->surname);
        }
        while (list[i]->surname == "");
        do
        {
            cout << "Sport: ";
            getline(cin, list[i]->sport);
        }
        while (list[i]->sport == "");
        
        do {
            cout << "Medals: ";
            cin >> list[i]->medals;
            if (cin.fail()) {
                cin.clear();
                while (cin.get() != '\n');
            }
            else
                break;
        } while (true);

        do {
            cout << "Prize: ";
            cin >> list[i]->prize;
            if (cin.fail()) {
                cin.clear();
                while (cin.get() != '\n');
            }
            else
                break;
        } while (true);
        cout << endl;
    }

    write_list(list, LIST_SIZE); //вывод введеных значений

    //разделение массива на части: с медалями и без
    int j = LIST_SIZE - 1; //переменная последнего ненулевого или непроверенного элемента базы
    for (int i = 0; i < j; i++) { //пока в первой части ненулевые элементы, а во второй нулевые
        if (list[i]->medals == 0 && list[j]->medals != 0) { //проверяем на нулевые значения 1 и 2 элементы
            Sportsman* temp = list[i]; //в случае прохождения условия
            list[i] = list[j]; //меняем элементы местами
            list[j] = temp; //отправляем ноль в конец
            j--;
        }
        else if (list[i]->medals == 0 && list[j]->medals == 0) { //если оба элемента равны 0
            i--; //переменная счетчик с начала базы остается на своей позиции
            j--; //а дальняя приближается, оставляя ноль позади
        }
    }
    j--; 
    //сортировка перемешиванием
    int k = 0;
    while (k < j) {
        //проходим с начала до конца первой половины массива
        //в процессе меняя необходимые элементы местами
        for (int i = 0; i < j - 1; i++) {
            if (list[i]->surname > list[i + 1]->surname) {
                Sportsman* temp = list[i];
                list[i] = list[i + 1];
                list[i + 1] = temp;
            }
        }
        j--; //самый большой элемент остается в конце
        //проходим от конца до начала массива первой половины списка
        //в процессе меняя необходимые элементы местами
        for (int i = j - 1; i > k; i--) {
            if (list[i - 1]->surname > list[i]->surname) {
                Sportsman* temp = list[i];
                list[i] = list[i - 1];
                list[i - 1] = temp;
            }
        }
        k++; //оставляем наимешьший элемент в начале
    }

    write_list(list, LIST_SIZE); //выводим результат

    //освобождаем выделенную память
    for (int i = 0; i < LIST_SIZE; i++) {
        delete list[i];
    }
    delete[] list;
}

//функция вывода массива
void write_list(Sportsman** list, int len) {
    for (int i = 0; i < len; i++) {
        std::cout << i + 1 << ": " << list[i]->surname << ", " << list[i]->sport << ", " << list[i]->medals << ", " << list[i]->prize << std::endl;
    }
    std::cout << std::endl;
}