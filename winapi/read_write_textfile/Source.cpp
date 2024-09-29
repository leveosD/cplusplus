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
	wchar_t* outFile = new wchar_t[size];
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

	wchar_t symbs[3];
	mbstowcs_s(&outSize, symbs, 3, argv[2], 2);
	/*WriteFile(hOut, argv[2], 3, &nOut, NULL);
	WriteFile(hOut, "\n", 2, &nOut, NULL);*/
	int count = 0;
	wchar_t wbuffer[BUF_SIZE] = { 0 };
	char buffer[BUF_SIZE] = { 0 };
	while (ReadFile(hIn, buffer, BUF_SIZE, &nIn, NULL) && nIn > 0)
	{
		mbstowcs_s(&outSize, wbuffer, BUF_SIZE, buffer, BUF_SIZE - 1);
		for (int i = 0; i < nIn; i++) {
			if (wbuffer[i] == symbs[0] && wbuffer[i + 1] == symbs[1]) {
				wbuffer[i] = STR[0];
				wbuffer[i + 1] = STR[1];
				i++;
				count++;
			}
		}
		//wcout << wbuffer << L' ' << nIn << endl;
		WriteFile(hOut, wbuffer, nIn*2, &nOut, NULL);
	}

	wcout << "\nChanges: " << count;
	CloseHandle(hIn);
	CloseHandle(hOut);
}