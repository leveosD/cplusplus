#include <iostream>
#include <fstream>
#include <string>

//main с аргументами программной строки
int main(int argc, char* argv[])
{
    using namespace std;
    //4.1
    //инициализация и открытия переменных для записи в файл и его чтения
    ifstream in(argv[1]);
    ofstream out("text2.txt");
    //записываем аргементы программной строки в переменные
    char SYMB = *argv[2];
    string str;
    //если файлы успешно открыты
    if (out.is_open() && in.is_open()) {
        while (in >> str) { //читаем строку
            if (str.find(SYMB) == -1) { //если в ней нет заданного символа
                out << str << char(in.get()); //записываем в файл строку
                //и символ разделитель
            }
        }
    }
    //закрываем открытые файлы
    in.close();
    out.close();

    //4.2
    //открываем нужные файлы
    in.open(argv[1]);
    int COUNT = *argv[3] - '0';
    out.open("text3.txt");
    if (out.is_open() && in.is_open()) {
        char ch; //переменная для посимвольной обработки
        while (in.get(ch)) { //пока ввод корректен
            out << ch; //записываем символ в файл
            if (ch == ' ' && COUNT-- > 0) //если этот символ пробел
            //и количество вставок не превышает максимум
                out << '.'; //довавляем точку
        }
    }
    in.close();
    out.close();
}
