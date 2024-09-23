#include <iostream>
#include <windows.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 


const int BUF_SIZE = 256;
const wchar_t STR[3] = L"#@";

int main(int argc, char* argv[])
{
	using std::wcout;
	using std::endl;
	setlocale(LC_ALL, ".UTF8");

	if (argc != 3) {
		wcout << L"Arguments error\n";
		return  -1;
	}
	HANDLE hIn, hOut;
	DWORD nIn, nOut;

	size_t size = strlen(argv[1]) + 1;
	wchar_t* inFile = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, inFile, size, argv[1], size - 1);
	/*wchar_t* outFile = new wchar_t[size];
	wcscpy_s(outFile, size, inFile);
	wchar_t* pos = wcsstr(outFile, L".");
	if (pos != NULL)
	{
		int letter = pos - outFile;
		outFile[letter + 1] = L'o';
		outFile[letter + 2] = L'u';
		outFile[letter + 3] = L't';
		outFile[letter + 4] = L'\0';
	}*/

	wchar_t outFile[12] = L"outfile.txt";

	hIn = CreateFile(inFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hIn == INVALID_HANDLE_VALUE)
	{
		wcout << inFile << L" error1: " << GetLastError() << endl;
		//cout << argv[1] << " error1: " << GetLastError() << endl;
		return  2;
	}

	hOut = CreateFile((LPCTSTR)outFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		wcout << outFile << L" error2: " << GetLastError() << endl;
		return 3;
	}

	/*WriteFile(hOut, argv[2], 3, &nOut, NULL);
	WriteFile(hOut, "\n", 2, &nOut, NULL);*/
	int count = 0;
	char buffer[BUF_SIZE] = { 0 };
	while (ReadFile(hIn, buffer, BUF_SIZE, &nIn, NULL) && nIn > 0)
	{
		for (int i = 0; i < nIn; i++) {
			std::cout << buffer[i] << argv[2][0] << endl;
			if (buffer[i] == argv[2][0] && buffer[i + 1] == argv[0][1]) {
				buffer[i] = STR[0];
				buffer[i + 1] = STR[1];
				i++;
				count++;
			}
		}
		WriteFile(hOut, buffer, nIn * 2, &nOut, NULL);
	}

	wcout << "\nChanges: " << count;
	CloseHandle(hIn);
	CloseHandle(hOut);
}