#include <iostream>
#include <windows.h> 

HINSTANCE hLib;
typedef void(*ChangeText)(HANDLE, HANDLE, const wchar_t*, const wchar_t*);

const wchar_t STR[3] = L"AB";
const int STR_SIZE = 128;

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

	hLib = LoadLibrary(L"C:/proga/labaklava/x64/Debug/labaklava.dll");
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

	size_t outSize;
	//mbstowcs_s(&outSize, inFile, size, argv[1], size - 1);
	char outFile[20] = "text1.txt\0";
	/*strcpy_s(inFile, strlen(argv[1]) + 1, argv[1]);
	char* pos = strstr(inFile, ".");
	if (pos == NULL)
		strcat_s(inFile, STR_SIZE, ".txt");
	else
	{
		int letter = pos - inFile;
		inFile[letter + 1] = 't';
		inFile[letter + 2] = 'x';
		inFile[letter + 3] = 't';
		inFile[letter + 4] = '\0';
	}*/
	wchar_t* wSymbs = new wchar_t[strlen(symbs) + 1];
	mbstowcs_s(&outSize, wSymbs, strlen(symbs) + 1, symbs, 2);
	
	HANDLE hIn, hOut;
	hIn = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hIn == INVALID_HANDLE_VALUE)
	{
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "ERROR", 5, &cbWritten, NULL);
		FreeLibrary(hLib);
		return 2;
	}

	hOut = CreateFileA(outFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "ERROR", 5, &cbWritten, NULL);
		FreeLibrary(hLib);
		return 3;
	}

	ct(hIn, hOut, wSymbs, STR);
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\n(Child) File has been changed. ", 32, &cbWritten, NULL);
	
	delete[] wSymbs;
	FreeLibrary(hLib);
	CloseHandle(hIn);
	CloseHandle(hOut);
	return 0;
}