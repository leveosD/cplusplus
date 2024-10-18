#include <iostream>
#include <windows.h> 

HINSTANCE hLib;
typedef void(*ChangeText)(HANDLE, HANDLE, const char*, const char*);

const char STR[3] = "#@";
const int STR_SIZE = 128;
const char PATH[] = "D:/code/cplusplus/winapi/pipe_project/x64/Debug/";

int main(int argc, char* argv[])
{
	using namespace std;
	char* filename;
	char* symbs;
	char message[STR_SIZE] = { 0 };
	char result[10] = { 0 };
	DWORD cbWritten;
	DWORD cbRead;
	if (!ReadFile(GetStdHandle(STD_INPUT_HANDLE), message, STR_SIZE, &cbRead, NULL)) {
		char error[12] = "Error: ";
		char code[5];
		_itoa_s(GetLastError(), code, 5, 10);
		strcat_s(error, 12, code);
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), error, 12, &cbWritten, NULL);
		return 1;
	}
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\n(Child) Get filename to change file. ", 36, &cbWritten, NULL);
	char* cntx = nullptr;
	filename = strtok_s(message, " ", &cntx);
	symbs = strtok_s(NULL, " ", &cntx);
	//путь к dll, в которой находится функций, изменяющая текст. её можно найти в этом же репозитории в папке dll_project
	hLib = LoadLibrary(L"D:/code/cplusplus/winapi/dll_project/x64/Debug/dll_project.dll");
	if (hLib == NULL) {
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "ERROR_MOD_NOT_FOUND", 20, &cbWritten, NULL);
		return 2;
	}

	ChangeText ct = (ChangeText)GetProcAddress(hLib, "ChangeText");
	if (ct == NULL) {
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "ERROR", 5, &cbWritten, NULL);
		FreeLibrary(hLib);
		return 3;
	}
	//size_t size = strlen(argv[1]) + 1;
	//wchar_t* inFile = new wchar_t[size];

	HANDLE hIn, hOut;
	char* outFile;
	//wcout << argv[1];
	hIn = CreateFileA(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hIn == INVALID_HANDLE_VALUE) {
		//addLogMessage("Original infile can't be opened");
		size_t size = strlen(argv[1]) + strlen(PATH) + 1;
		char* inFile = new char[size];
		strcpy_s(inFile, strlen(PATH) + 1, PATH);
		strcat_s(inFile, size, argv[1]);
		outFile = new char[size];
		strcpy_s(outFile, size, inFile);
		hIn = CreateFileA(inFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
		if (hIn == INVALID_HANDLE_VALUE)
		{
			WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "INFILE", 6, &cbWritten, NULL);
			//addLogMessage("Can't open in file with PAth");
			//addLogMessage(inFile);
			delete[] inFile;
			return 4;
		}
		delete[] inFile;
	}
	else {
		//addLogMessage("Original infile was opened");
		//addLogMessage(argv[1]);
		outFile = new char[strlen(argv[1]) + 1];
		strcpy_s(outFile, strlen(argv[1]) + 1, argv[1]);
	}
	char* pos = strchr(outFile, '.');
	if (pos != NULL)
	{
		int letter = pos - outFile;
		outFile[letter + 1] = L'o';
		outFile[letter + 2] = L'u';
		outFile[letter + 3] = L't';
		outFile[letter + 4] = L'\0';
	}
	//addLogMessage(outFile);
	hOut = CreateFileA(outFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "OUTFILE", 7, &cbWritten, NULL);
		//addLogMessage("Can't open out file");
		//addLogMessage(outFile);
		return 5;
	}

	ct(hIn, hOut, argv[2], STR);
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\n(Child) File has been changed. ", 32, &cbWritten, NULL);
	
	FreeLibrary(hLib);
	CloseHandle(hIn);
	CloseHandle(hOut);
	return 0;
}