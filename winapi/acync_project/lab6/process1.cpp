#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <conio.h>

const CHAR ABSOLUTE_PATH[50] = "D:/code/cplusplus/winapi/acync_project/x64/Debug/";

HANDLE hSemaphoreSend;
HANDLE hSemaphoreRecv;
HANDLE hSemaphoreTermination;
HANDLE hSemaphores[2];
CHAR lSemaphoreSendName[] =
"$MylSemaphoreSendName$";
CHAR lpSemaphoreRecvName[] =
"$MylSemaphoreRecvName$";
CHAR lpSemaphoreTerminationName[] =
"$MylSemaphoreTerminationName$";

CHAR lpFileShareName[] =
"$MyFileShareName$";
HANDLE hFileMapping;
LPVOID lpFileMap;

const char FILENAME[13] = "new_file.txt";
const size_t BUF_SIZE = 512;
const char STR[3] = "#@";

int main()
{
	DWORD dwRetCode;
	CHAR str[80];
	char* filename;
	DWORD  cbWritten;
	DWORD   total = 0;
	char message[80] = { 0 };
	printf("Mapped and shared file, event sync, server process\n");
	hSemaphoreSend = CreateSemaphoreA(NULL, 0, 1, lSemaphoreSendName);
	hSemaphoreRecv = CreateSemaphoreA(NULL, 0, 1, lpSemaphoreRecvName);
	hSemaphoreTermination = CreateSemaphoreA(NULL, 0, 1, lpSemaphoreTerminationName);
	if (hSemaphoreSend == NULL || hSemaphoreRecv == NULL || hSemaphoreTermination == NULL)
	{
		printf("CreateSemaphore: Error %ld\n",
			GetLastError());
		return 0;
	}
	hSemaphores[0] = hSemaphoreSend;
	hSemaphores[1] = hSemaphoreTermination;

	hFileMapping = CreateFileMappingA(INVALID_HANDLE_VALUE,
		NULL, PAGE_READWRITE, 0, 100, lpFileShareName);
	if (hFileMapping == NULL)
	{
		printf("CreateFileMapping: Error %ld\n",
			GetLastError());
		return 0;
	}
	lpFileMap = MapViewOfFile(hFileMapping,
		FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (lpFileMap == 0)
	{
		printf("MapViewOfFile: Error %ld\n",
			GetLastError());
		return 0;
	}
	while (TRUE)
	{
		dwRetCode = WaitForMultipleObjects(2, hSemaphores, FALSE, INFINITE);
		if (dwRetCode == WAIT_ABANDONED_0 ||
			dwRetCode == WAIT_ABANDONED_0 + 1 ||
			dwRetCode == WAIT_FAILED)// ||
			//dwRetCode == WAIT_OBJECT_0 + 1)
		{
			printf("Waiting error: %d", GetLastError());
			break;
		}
		else
		{
			int c = 0;
			strcpy(str, ((LPSTR)lpFileMap));
			char* cntx = nullptr;
			char* inFile = strtok_s(str, " ", &cntx);
			filename = new char[50 + strlen(inFile)];
			memcpy(filename, ABSOLUTE_PATH, 50);
			memcpy(filename + 49, inFile, strlen(inFile) + 1);
			char* symbs = strtok_s(NULL, " ", &cntx);
			HANDLE hIn, hOut;
			hIn = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
			if (hIn == INVALID_HANDLE_VALUE)
			{
				sprintf(message, "(Server)Can't open %s! Error: %d\n", filename, GetLastError());
				printf("%s", message);
				WriteFile(GetStdHandle(STD_ERROR_HANDLE), message, strlen(message) + 1, &cbWritten, NULL);
				strcpy(((LPSTR)lpFileMap), message);
			}
			int len = strlen(filename) + 1;
			char* filename2 = new char[len];
			strcpy_s(filename2, len, filename);
			char* pos = strchr(filename2, '.');
			if (pos != nullptr) {
				int letter = pos - filename2;
				filename2[letter + 1] = 'o';
				filename2[letter + 2] = 'u';
				filename2[letter + 3] = 't';
				filename2[letter + 4] = '\0';
			}
			hOut = CreateFileA(filename2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hOut == INVALID_HANDLE_VALUE) {
				sprintf(message, "(Server)Can't create %s! Error: %d\n", filename2, GetLastError());
				WriteFile(GetStdHandle(STD_ERROR_HANDLE), message, strlen(message) + 1, &cbWritten, NULL);
				printf("%s\n", message);
				strcpy(((char*)lpFileMap), message);
			}

			char buffer[BUF_SIZE] = { 0 };
			wchar_t wbuffer[BUF_SIZE] = { 0 };
			DWORD nin, nout;
			int count = 0;
			size_t outSize;

			while (ReadFile(hIn, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
			{
				mbstowcs_s(&outSize, wbuffer, BUF_SIZE, buffer, BUF_SIZE - 1);
				for (int i = 0; i < nin; i++) {
					if (wbuffer[i] == symbs[0] && wbuffer[i + 1] == symbs[1]) {
						wbuffer[i] = STR[0];
						wbuffer[i + 1] = STR[1];
						i++;
						count++;
					}
				}
				WriteFile(hOut, wbuffer, nin * 2, &nout, NULL);
			}

			CloseHandle(hIn);
			CloseHandle(hOut);
			sprintf(message, "File %s has been changed. Number of changes: %d\n", str, count);
			WriteFile(GetStdHandle(STD_ERROR_HANDLE), message, strlen(message) + 1, &cbWritten, NULL);

			strcpy(((LPSTR)lpFileMap), message);
			strcpy(str, ((LPSTR)lpFileMap));
			printf("Test: %s\n", str);
			ReleaseSemaphore(hSemaphoreRecv, 1, NULL);
		}
	}

	CloseHandle(hSemaphoreSend);
	CloseHandle(hSemaphoreRecv);
	UnmapViewOfFile(lpFileMap);
	CloseHandle(hFileMapping);

	return 0;
}