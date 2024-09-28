#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <conio.h>

HANDLE hEventSend;
HANDLE hEventRecv;
HANDLE hEventTermination;
HANDLE hEvents[2];
CHAR lpEventSendName[] =
"$MylEventSendName$";
CHAR lpEventRecvName[] =
"$MylEventRecvName$";
CHAR lpEventTerminationName[] =
"$MyEventTerminationName$";
CHAR lpFileShareName[] =
"$MyFileShareName$";
HANDLE hFileMapping;
LPVOID lpFileMap;

const char FILENAME[13] = "new_file.txt";
const size_t BUF_SIZE = 512;
const char STR[3] = "AB";

int main()
{
	DWORD dwRetCode;
	CHAR str[80];
	//FILE* hdl;
	DWORD  cbWritten;
	DWORD   total = 0;
	char message[80] = { 0 };
	printf("Mapped and shared file, event sync, server process\n");
	hEventSend = CreateEventA(NULL, FALSE, FALSE, lpEventSendName);
	hEventRecv = CreateEventA(NULL, FALSE, FALSE, lpEventRecvName);
	if (hEventSend == NULL || hEventRecv == NULL)
	{
		fprintf(stdout, "CreateEvent: Error %ld\n",
			GetLastError());
		return 0;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		printf("\nApplication EVENT already started\n"
			"Press any key to exit...");
		return 0;
	}
	hEventTermination = CreateEventA(NULL,
		FALSE, FALSE, lpEventTerminationName);
	if (hEventTermination == NULL)
	{
		fprintf(stdout, "CreateEvent (Termination): Error %ld\n",
			GetLastError());
		return 0;
	}
	hFileMapping = CreateFileMappingA(INVALID_HANDLE_VALUE,
		NULL, PAGE_READWRITE, 0, 100, lpFileShareName);
	if (hFileMapping == NULL)
	{
		fprintf(stdout, "CreateFileMapping: Error %ld\n",
			GetLastError());
		return 0;
	}
	lpFileMap = MapViewOfFile(hFileMapping,
		FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (lpFileMap == 0)
	{
		fprintf(stdout, "MapViewOfFile: Error %ld\n",
			GetLastError());
		return 0;
	}
	hEvents[0] = hEventTermination;
	hEvents[1] = hEventSend;
	while (TRUE)
	{
		dwRetCode = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
		if (dwRetCode == WAIT_ABANDONED_0 ||
			dwRetCode == WAIT_ABANDONED_0 + 1 ||
			dwRetCode == WAIT_OBJECT_0 ||
			dwRetCode == WAIT_FAILED)
			break;
		else
		{
			puts(((LPSTR)lpFileMap));
			strcpy(str, ((LPSTR)lpFileMap));
			char* cntx = nullptr;
			char* inFile = strtok_s(str, " ", &cntx);
			char* symbs = strtok_s(NULL, " ", &cntx);
			HANDLE hIn, hOut;
			hIn = CreateFileA(inFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
			if (hIn == INVALID_HANDLE_VALUE)
			{
				sprintf(message, "(Server)Can't open %s! Error: %d", inFile, GetLastError());
				WriteFile(GetStdHandle(STD_ERROR_HANDLE), message, strlen(message) + 1, &cbWritten, NULL);
				printf("\n");
				strcpy(((LPSTR)lpFileMap), message);
			}

			hOut = CreateFileA(FILENAME, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hOut == INVALID_HANDLE_VALUE) {
				sprintf(message, "(Server)Can't create %s! Error: %d", FILENAME, GetLastError());
				WriteFile(GetStdHandle(STD_ERROR_HANDLE), message, strlen(message) + 1, &cbWritten, NULL);
				printf("\n");
				strcpy(((LPSTR)lpFileMap), message);
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

			sprintf(message, "File %s has been changed. Number of changes: %d\n", str, count);
			WriteFile(GetStdHandle(STD_ERROR_HANDLE), message, strlen(message) + 1, &cbWritten, NULL);
			printf("\n");
			strcpy(((LPSTR)lpFileMap), message);
			CloseHandle(hIn);
			CloseHandle(hOut);
			SetEvent(hEventRecv);
		}
	}

	CloseHandle(hEventSend);
	CloseHandle(hEventRecv);
	CloseHandle(hEventTermination);
	UnmapViewOfFile(lpFileMap);
	CloseHandle(hFileMapping);

	return 0;
}