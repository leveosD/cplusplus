#include <iostream>
#include <windows.h> 

HINSTANCE hLib;
typedef int(*ChangeText)(HANDLE, HANDLE, wchar_t*, wchar_t*);
wchar_t STR[3] = L"#@";

int main(int argc, char* argv[])
{
	using std::wcout;
	using std::endl;
	if (argc != 3) {
		wcout << L"Arguments error\n";
		return 1;
	}
	hLib = LoadLibrary(L"dll_project.dll");
	if (hLib == NULL) {
		wcout << L"Error " << GetLastError();
		return 2;
	}
	
	ChangeText ct = (ChangeText)GetProcAddress(hLib, "ChangeText");
	if (ct == NULL) {
		wcout << L"Error " << GetLastError();
		return 3;
	}

	size_t size = strlen(argv[1]) + 1;
	wchar_t* inFile = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, inFile, size, argv[1], size - 1);
	wchar_t* outFile = new wchar_t[size];
	//wchar_t outFile[20] = L"text1.txt";
	wcscpy_s(outFile, size, inFile);
	wchar_t* pos = wcsstr(outFile, L".");
	if (pos != NULL)
	{
		int letter = pos - outFile;
		outFile[letter + 1] = L'o';
		outFile[letter + 2] = L'u';
		outFile[letter + 3] = L't';
		outFile[letter + 4] = L'\0';
	}

	wchar_t* symbs = new wchar_t[3];
	mbstowcs_s(&outSize, symbs, 3, argv[2], 2);


	HANDLE hIn, hOut;
	hIn = CreateFileW(inFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hIn == INVALID_HANDLE_VALUE)
	{
		wcout << "In File Error: " << inFile << " " << GetLastError() << endl;
		return 4;
	}

	hOut = CreateFileW(outFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		wcout << L"Error: " << GetLastError();
		return 5;
	}

	wcout << "Changes: " << ct(hIn, hOut, symbs, STR);

	delete[] inFile;
	FreeLibrary(hLib);
	CloseHandle(hIn);
	CloseHandle(hOut);
}