#include <iostream>
#include <string>   //библиотека для работа со строками
#include <set>
#include <map>

int main()
{
    using namespace std;    //использование пространства имен std
    
    //запись двух строк с клавиатуры (с текстовыми подсказками)
    cout << "Enter the first string: "; // текстовая подсказка
    string str1;        //создание переменной типа string
    getline(cin, str1); //присваивание переменной введенного значения
    cout << "Enter the second string: ";
    string str2;
    getline(cin, str2);

    //вывод длин строк
    int len1 = 0, len2 = 0; //переменные для хранения длин строк
    cout << "The first string's lenght: ";
    for (char x : str1) //цикл for, проходящий все элементы строки
        len1++; //увеличение счетчика за каждый символ
    cout << len1 << endl;
    cout << "The second string's lenght: ";
    for (char x : str2)
        len2++;
    cout << len2 << endl;

    //конкатенация строк с удалением пробелов
    string str3 = str1 + str2; //объядиняем введенные строки
    while (str3.find(' ') != string::npos) //новая итерация начинается, если в строке есть пробелы
        str3.erase(str3.find(' '), 1); //удаление пробела
    cout << str3 << endl;

    //замена во всех парах одинаковых символов второй элемент на пробел
    int i = 0, count = 0;   //переменные счетчики для перебора строки и количества замен
    set<char> symbs; //создание set для отслеживания пар
    do {
        if (!symbs.contains(str3[i]))   //если в symbs нет текущего символа,
            symbs.insert(str3[i]);      //добавляем его в symbs
        else {                          //если в symbs есть текущий символ
            symbs.erase(str3[i]);       //удаляем его, т.к. он является вторым в нынешней паре
            str3.replace(i, 1, " ");    //заменяем 2 элемент на пробел
            if (++count == 4)           //если счетчик достигает 4,
                break;                  //цикл завершается
        }
    } while (++i < str3.length());      //цикл продолжается,до конца строки

    string str3_copy = str3; // создание копии переменной str3 для последубщего применения оператора switch

    map<char, char> chars = { {'A', '0'}, {'M', '1'}, {'P', '2'}, {'R', '3'} }; //создание словаря для хранения символов,
                                                          //которые нужно заменить, и символов, на которые нужно заменить
    i = 0; //обнуляем переменную-счетчик
    do {
        if (chars.contains(str3[i])) { // если текущий символ есть в словаре
            str3[i] = chars[str3[i]]; // заменяем его на необходимый
        }
    } while (++i < str3.length()); // цикл продолжается до конца строки
    cout << "Result: " << str3 << endl; // вывод результата

    for (int k = 0; k < str3_copy.length(); k++) { //цикл, перебирaющий str3_copy
        switch (str3_copy[k]) { // сравнение с k-ым символом строки
        case 'A':
            str3_copy[k] = '0';
            break;
        case 'M':
            str3_copy[k] = '1';
            break;
        case 'P':
            str3_copy[k] = '2';
            break;
        case 'R':
            str3_copy[k] = '3';
            break;
        }
    }
    cout << "Switch: " << str3_copy << endl; // вывод конечной строки
}