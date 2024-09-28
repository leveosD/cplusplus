#include "windows.h"
#include <iostream>

int main(int argc, char* argv[]) {
	using namespace std;
	if (argc != 3) {
		wcout << L"Arguments error\n";
		return  -1;
	}

	HANDLE hin, hout;
	wchar_t infile[10] = L"text.txt";
	hin = CreateFileA(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hin == INVALID_HANDLE_VALUE)
	{
		cout << argv[1] << " not found";
		return 1;
	}
	/*hin = CreateFileW(infile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hin == INVALID_HANDLE_VALUE)
	{
		return 1;
	}*/

	wchar_t outfile[10] = L"file.txt";
	hout = CreateFileW(outfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hout == INVALID_HANDLE_VALUE) {
		return 1;
	}
	const int BUF_SIZE = 256;
	char buffer[BUF_SIZE] = { 0 };
	DWORD nin, nout;
	wchar_t symbs[3] = L"لع"; //символы, которые надо заменить
	/*size_t outSize;
	mbstowcs_s(&outSize, symbs2, 3, argv[2], 2);
	for (int i = 0; i < 3; i++) {
		wcout << (symbs[i] == symbs2[i]) << endl;
	}*/
	/*size_t size = wcslen(argv[2]) + 1;
	wchar_t* symbs2 = new wchar_t[size];
	wcscpy_s(symbs2, size, argv[2]);*/

	//WriteFile(hout, symbs2, 3, &nout, NULL);
	//WriteFile(hout, argv[2], 3, &nout, NULL);
	wchar_t STR[3] = L"бу"; // символы, на которые надо заменить
	int count = 0;
	while (ReadFile(hin, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
	{
		wchar_t str[BUF_SIZE] = { 0 };
		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, buffer, nin, str, BUF_SIZE);
		for (int i = 0; i < nin; i++) {
			if (str[i] == symbs[0] && str[i + 1] == symbs[1]) {
				str[i] = STR[0];
				str[i + 1] = STR[1];
				i++;
				count++;
			}
		}
		BOOL flag;
		char changedstr[BUF_SIZE] = { 0 };
		WideCharToMultiByte(CP_UTF8, WC_DEFAULTCHAR, str, nin, changedstr, BUF_SIZE, NULL, &flag);
		WriteFile(hout, changedstr, nin, &nout, NULL);
	}
	std::cout << count;
	return 0;
}