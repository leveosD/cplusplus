#include <iostream>
#include <windows.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

HINSTANCE hLib;
int ChangeText(wchar_t*, wchar_t*);

int main(int argc, char* argv[])
{
	using std::wcout;
	using std::endl;
	if (argc != 3) {
		wcout << L"Arguments error\n";
		return  -1;
	}

	size_t size = strlen(argv[1]) + 1;
	wchar_t* inFile = new wchar_t[size];

	size_t outSize;
	mbstowcs_s(&outSize, inFile, size, argv[1], size - 1);
	wchar_t outFile[20] = L"text1.txt\0";
	/*strcpy_s(inFile, strlen(argv[1]) + 1, argv[1]);
	char* pos = strstr(inFile, ".");
	if (pos == NULL)
		strcat_s(inFile, 80, ".txt");
	else
	{
		int letter = pos - inFile;
		inFile[letter + 1] = 't';
		inFile[letter + 2] = 'x';
		inFile[letter + 3] = 't';
		inFile[letter + 4] = '\0';
	}
	//cout << argv[0] << endl;
	//cout << inFile << " " << (LPCWSTR)inFile << " " << &inFile << endl;
	//std::wcout << inFile << L" " << (LPCWSTR)inFile << L" " << &inFile << endl;*/
	
}