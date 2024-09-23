#include "windows.h"
#include <iostream>

int main(int argc, char* argv[]) {
	using namespace std;

	if (argc != 3) {
		wcout << L"Arguments error\n";
		return  -1;
	}

	HANDLE hin, hout;

	hin = CreateFileA(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hin == INVALID_HANDLE_VALUE)
	{
		wcout << argv[1] << " not found";
		return 1;
	}

	char outfile[12] = "outfile.txt";
	hout = CreateFileA(outfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hout == INVALID_HANDLE_VALUE) {
		wcout << outfile << " error";
		return 1;
	}
	const int BUF_SIZE = 256;
	char buffer[BUF_SIZE] = { 0 };
	DWORD nin, nout;
	//wchar_t symbs[3] = L"لع"; //символы, которые надо заменить
	size_t outSize;
	wchar_t symbs[5];
	MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, argv[2], 2, symbs, 4);
	////mbstowcs_s(&outSize, symbs, 3, argv[2], 2);
	//cout << argv[2] << endl;
	//cout << symbs[0] << " " << symbs[1] << endl;
	//WriteFile(hout, symbs, 3, &nout, NULL);
	wchar_t STR[3] = L"бу"; // символы, на которые надо заменить
	int count = 0;
	while (ReadFile(hin, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
	{
		wchar_t str[BUF_SIZE] = { 0 };
		MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, buffer, nin, str, BUF_SIZE);
		wcout << sizeof(buffer) << L' ' << sizeof(str) << endl;
		for (int i = 0; i < 256; i++) {
			wchar_t line[7];
			line[0] = buffer[i];
			line[1] = L':';
			line[2] = L' ';
			line[3] = str[i * 2];
			line[4] = str[i * 2 + 1];
			line[5] = '\n';
			line[6] = '\0';
			WriteFile(hout, line, nin, &nout, NULL);
		}
		for (int i = 0; i < nin; i++) {
			wcout << size_t(str[i]) << " " << size_t(symbs[0]) << L' ' << size_t(argv[2][0]) << L' ' << size_t(buffer[i]) << endl;
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
	wcout << count;
	return 0;
}