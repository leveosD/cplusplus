#include <iostream>
#include <windows.h> 
#include <conio.h>

HINSTANCE hLib;
typedef void(*ChangeText)(HANDLE, HANDLE, const wchar_t*, const wchar_t*);

const char STR[3] = "AB";
const int STR_SIZE = 128;
const int BUF_SIZE = 512;
const char FILENAME[13] = "new_file.txt";

int main(int argc, char* argv[])
{
	using namespace std;
	BOOL   fReturnCode;
	DWORD  cbMessages;
	DWORD  cbMsgNumber;
	HANDLE hMailslot1;
	HANDLE hMailslot2 = { 0 };
	LPSTR  lpszReadMailslotName = (LPSTR)"\\\\.\\mailslot\\$Channel1$";
	LPSTR  lpszWriteMailslotName = (LPSTR)"\\\\*\\mailslot\\$Channel2$";
	char   szBuf[BUF_SIZE];
	DWORD  cbRead;
	DWORD  cbWritten;
	DWORD   total = 0;
	char message[80] = { 0 };
	FILE* hdl;
	cout << "Mailslot server demo\n";
	hMailslot1 = CreateMailslotA(lpszReadMailslotName, 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (hMailslot1 == INVALID_HANDLE_VALUE)
	{
		cout << "Error: " << GetLastError() << endl;
		return 0;
	}
	cout << "Mailslot created\n";
	while (1)
	{
		fReturnCode = GetMailslotInfo(hMailslot1, NULL, &cbMessages, &cbMsgNumber, NULL);
		if (!fReturnCode)
		{
			cout << "Error: " << GetLastError() << endl;
			break;
		}
		if (cbMsgNumber != 0)
		{
			if (ReadFile(hMailslot1, szBuf, BUF_SIZE, &cbRead, NULL))
			{
				cout << "Content: " << szBuf << endl;
				if (strcmp(szBuf, "exit") == 0)
					break;
				else
				{
					hMailslot2 = CreateFileA(lpszWriteMailslotName, GENERIC_WRITE,
						FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
					if (hMailslot2 == INVALID_HANDLE_VALUE)
					{
						cout << "Error: " << GetLastError() << endl;
						break;
					}
					else {
						char* cntx = nullptr;
						char* inFile = strtok_s(szBuf, " ", &cntx);
						char* symbs = strtok_s(NULL, " ", &cntx);
						HANDLE hIn, hOut;
						hIn = CreateFileA(inFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
						if (hIn == INVALID_HANDLE_VALUE)
						{
							cout << "Error: " << GetLastError() << endl;
							break;
						}

						hOut = CreateFileA(FILENAME, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						if (hOut == INVALID_HANDLE_VALUE) {
							cout << "Error: " << GetLastError() << endl;
							break;
						}
						CHAR buffer[BUF_SIZE] = { 0 };
						DWORD nIn, nOut;
						int count = 0;
						while (ReadFile(hIn, buffer, BUF_SIZE, &nIn, NULL) && nIn > 0)
						{
							wchar_t str[BUF_SIZE] = { 0 };
							MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, buffer, nIn, str, BUF_SIZE);
							for (int i = 0; i < nIn; i++) {
								if (str[i] == symbs[0] && str[i + 1] == symbs[1]) {
									str[i] = STR[0];
									str[i + 1] = STR[1];
									i++;
									count++;
								}
							}
							BOOL flag;
							char changedstr[BUF_SIZE] = { 0 };
							WideCharToMultiByte(CP_UTF8, WC_DEFAULTCHAR, str, nIn, changedstr, BUF_SIZE, NULL, &flag);
							WriteFile(hOut, changedstr, nIn, &nOut, NULL);
						}
						CloseHandle(hIn);
						CloseHandle(hOut);
						cout << "File " << inFile << " has been changed.\n";
					}
					
				}
			}
			else
			{
				cout << "Error: " << GetLastError() << endl;
				break;
			}
		}
		Sleep(500);

	}
	cout << "Mailslot server demo has been ended";
	CloseHandle(hMailslot1);
	CloseHandle(hMailslot2);
	return 0;
}