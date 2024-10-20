#include <windows.h>
#include <stdio.h>
#include <iostream>

const size_t NAME_SIZE = 256;
const size_t BUF_SIZE = 512;

int main(int argc, char* argv[])
{
	using namespace std;
	HANDLE hMailslot1, hMailslot2;
	LPSTR  lpszReadMailslotName = (LPSTR)"\\\\.\\mailslot\\$Channel2$";
	char   szMailslotName[NAME_SIZE];
	char   szBuf[BUF_SIZE];
	BOOL   fReturnCode;
	DWORD  cbMessages;
	DWORD  cbMsgNumber;
	DWORD  cbWritten;
	DWORD  cbRead;
	cout << "Mailslot client demo\n";
	cout << "Syntax: mslotclient [servername]\n";
	if (argc > 1)
		sprintf_s(szMailslotName, NAME_SIZE, "\\\\%s\\mailslot\\$Channel1$", argv[1]);
	else
		strcpy_s(szMailslotName, NAME_SIZE, "\\\\.\\mailslot\\$Channel1$");
	hMailslot1 = CreateFileA(szMailslotName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hMailslot1 == INVALID_HANDLE_VALUE)
	{
		cout << "Error: " << GetLastError() << endl;
		return 0;
	}
	cout << "Connected. Type 'exit' to terminate\n";
	hMailslot2 = CreateMailslotA(lpszReadMailslotName, 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (hMailslot2 == INVALID_HANDLE_VALUE)
	{
		cout << "Error: " << GetLastError() << endl;
		return 0;
	}
	cout << "Mailslot for reply created\n";
	while (1)
	{
		cout << "Enter your data: ";
		cin.getline(szBuf, NAME_SIZE, '\n');
		if (!WriteFile(hMailslot1, szBuf, strlen(szBuf) + 1, &cbWritten, NULL))
			break;
		if (!strcmp(szBuf, "exit"))
			break;
		cout << "Waiting for reply...\n";
		fReturnCode = GetMailslotInfo(hMailslot2, NULL, &cbMessages, &cbMsgNumber, NULL);
		if (!fReturnCode)
		{
			cout << "Error: " << GetLastError() << endl;
			break;
		}
		//cout << cbMsgNumber << endl;
		if (ReadFile(hMailslot2, szBuf, 512, &cbRead, NULL)) {
			cout << szBuf << endl;
		}
	}
	cout << "Mailslot client demo has been ended.\n";
	CloseHandle(hMailslot1);
	CloseHandle(hMailslot2);
	return 0;
}