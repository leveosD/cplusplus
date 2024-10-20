#include <iostream>
#include <windows.h> 
#include <conio.h>

const char STR[3] = "#@";
const int STR_SIZE = 128;
const int BUF_SIZE = 512;


int main(int argc, char* argv[])
{
	using namespace std;
	char* path = new char[strlen(argv[0] + 2)];
	strcpy_s(path, strlen(argv[0]) + 1, argv[0]);
	char second[] = "server.exe";
	char* p = strstr(path, second);
	if (p != nullptr) {
		*p = '\0';
	}
	cout << "Path: " << path << endl;
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
				if (!strcmp(szBuf, "exit"))
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
					else 
					{
						char* cntx = nullptr;
						char* filename = strtok_s(szBuf, " ", &cntx);
						char* symbs = cntx;
						
						HANDLE hIn, hOut;
						//wcout << argv[1];
						int len = strlen(path) + strlen(filename) + 1;
						char* infile = new char[len];
						strcpy_s(infile, strlen(path) + 1, path);
						strcpy_s(infile + strlen(path), strlen(filename) + 1, filename);
						hIn = CreateFileA(infile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

						char* outfile = new char[len];
						strcpy_s(outfile, len, infile);
						char* pos = strstr(outfile, ".txt");
						if (pos != nullptr) {
							int letter = pos - outfile;
							outfile[letter + 1] = 'o';
							outfile[letter + 2] = 'u';
							outfile[letter + 3] = 't';
							outfile[letter + 4] = '\0';
						}
						cout << "Outfile: " << outfile << endl;
						hOut = CreateFileA(outfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						if (hIn != INVALID_HANDLE_VALUE && hOut != INVALID_HANDLE_VALUE)
						{
							char buffer[BUF_SIZE] = { 0 };
							wchar_t wbuffer[BUF_SIZE] = { 0 };
							DWORD nin, nout;
							size_t outSize;
							int count = 0;
							while (ReadFile(hIn, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
							{
								mbstowcs_s(&outSize, wbuffer, BUF_SIZE, buffer, BUF_SIZE - 1);
								for (int j = 0; j < nin; j++) {
									if (buffer[j] == symbs[0] && buffer[j + 1] == symbs[1]) {
										buffer[j] = STR[0];
										buffer[j + 1] = STR[1];
										j++;
										count++;
									}
								}
								WriteFile(hOut, buffer, nin, &nout, NULL);
							}
							CloseHandle(hIn);
							CloseHandle(hOut);
							delete[] infile;
							delete[] outfile;
							cout << "File " << filename << " has been changed. Changes: " << count << endl;
							sprintf_s(message, 80, "Changes: %d.", count);
							WriteFile(hMailslot2, message, strlen(message) + 1, &cbWritten, NULL);
						}
						else {
							WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\nINFILE OR OUTFILE ERROR", 24, &cbWritten, NULL);
							delete[] infile;
							delete[] outfile;
							return 0;
						}
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