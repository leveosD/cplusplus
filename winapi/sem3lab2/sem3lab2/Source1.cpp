#include <windows.h>
#include <iostream> 

const int BUFF_SIZE = 256;
const wchar_t STR[3] = L"ab";

CHAR buffer[BUFF_SIZE] = { 0 };

int ChangeText(wchar_t* inFile, wchar_t* outFile, const wchar_t* symbs) 
{
	using std::wcout;
	using std::endl;
	HANDLE hIn, hOut;
	DWORD nIn, nOut;
	hIn = CreateFile(inFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hIn == INVALID_HANDLE_VALUE)
	{
		wcout << inFile << L" error1: " << GetLastError() << endl;
		//cout << argv[1] << " error1: " << GetLastError() << endl;
		return 2;
	}

	hOut = CreateFile((LPCTSTR)outFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		wcout << outFile << L" error2: " << GetLastError() << endl;
		return 3;
	}

	while (ReadFile(hIn, buffer, BUFF_SIZE, &nIn, NULL) && nIn > 0)
	{
		for (int i = 0; i < nIn; i++) {
			wcout << buffer[i];
			if (buffer[i] == symbs[0] && buffer[i + 1] == symbs[1]) {
				buffer[i] = STR[0];
				buffer[i + 1] = STR[1];
				i++;
			}
		}
		WriteFile(hOut, buffer, nIn, &nOut, NULL);
	}

	CloseHandle(hIn);
	CloseHandle(hOut);
}