#include <iostream>
#include <stdio.h>

//прототипы функций
void write();
void read();
void readAll();

const char file_name[] = "f.txt";
const int STUDENTS = 2;
const int NAME_LEN = 31;
const int PASSED = 12;

FILE* file; //файл для записи/чтения
struct Student {
	char surname[NAME_LEN];
	char passed[PASSED];
};
Student students[STUDENTS];

int main()
{
	using std::cout;
	using std::cin;

	//Выбор команды
	cout << "Enter\n1. Write students to file.\n2. Read all students from file.\n"
		<< "3. Read students who passed exam.\nQ to exit.\n";
	char ch;
	ch = cin.get();
	cin.get();
	while (ch != 'q' && ch != 'Q') {
		if (ch == '1')
			write();
		else if (ch == '2')
			readAll();
		else if (ch == '3')
			read();
		cout << "Enter\n1. Write students to file.\n2. Read all students from file.\n"
			<< "3. Read students who passed exam.\nQ to exit.\n";
		ch = cin.get();
		cin.get();
	}
}

//Функция для записи студентов в файл
void write() {
	using std::cout;
	using std::cin;
	fopen_s(&file, file_name, "a"); //открываем файл с возможностью дозаписи
	if (file == NULL)
		return;
	//Вводим 10 студентов
	for (int i = 0; i < STUDENTS; i++) {
		cout << "Enter surname: ";
		cin.getline(students[i].surname, NAME_LEN);
		cout << "Enter passed or not passed: ";
		cin.getline(students[i].passed, PASSED);
		while (strcmp(students[i].passed, "passed") && strcmp(students[i].passed, "not passed")) {
			cout << "Enter passed or not passed: ";
			cin.getline(students[i].passed, PASSED);
		}
		//Записываем в файл фамилию и инф о зачете
		fputs(students[i].surname, file);
		fputs("\n", file); //разделяем фамилию с зачетом для последующего
		fputs(students[i].passed, file); // счиывания через fgets
		fputs("\n", file);
	}
	fclose(file); //закрываем файл

	cout << "Your new students:\n";
	for (int i = 0; i < STUDENTS; i++) {
		cout << students[i].surname << ' ' << students[i].passed << '\n';
	}
}

void read() {
	fopen_s(&file, file_name, "r");
	if (file == NULL)
		return;
	char name[NAME_LEN];
	char exam[PASSED];

	std::cout << "Students who passed exam:\n";
	while (fgets(name, NAME_LEN, file)) { //первая считываемая строка всегда фамилия, поэтому заносим в поле name
		fgets(exam, PASSED, file); //следующая строка это инф о зачете
		exam[strlen(exam) - 1] = '\0'; //убираем символ новой строки, чтобы корректно сравнить
		if (strcmp(exam, "passed") == 0)
			std::cout << name;
	}
	
	fclose(file);
}

void readAll() {
	fopen_s(&file, file_name, "r");
	if (file == NULL)
		return;
	char name[NAME_LEN];
	char exam[PASSED];
	while(fgets(name, NAME_LEN, file)){
		fgets(exam, PASSED, file);
		name[strlen(name) - 1] = '\0';
		std::cout << name << " " << exam;
	}
	fclose(file);
}