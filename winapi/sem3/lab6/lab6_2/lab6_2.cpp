#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

HANDLE hEventSend;
HANDLE hEventRecv;
HANDLE hEventTermination;
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

int main()
{
	CHAR str[80];
	DWORD dwRetCode;
	printf("Mapped and shared file, event sync, client process\n"
		"\nEnter  <Exit> to terminate...\n");

	hEventSend = OpenEventA(EVENT_ALL_ACCESS, FALSE, lpEventSendName);
	hEventRecv = OpenEventA(EVENT_ALL_ACCESS, FALSE, lpEventRecvName);
	if (hEventSend == NULL || hEventRecv == NULL)
	{
		fprintf(stdout, "OpenEvent: Error %ld\n", GetLastError());
		return 0;
	}

	hEventTermination = OpenEventA(EVENT_ALL_ACCESS, FALSE, lpEventTerminationName);
	if (hEventTermination == NULL)
	{
		fprintf(stdout, "OpenEvent (Termination): Error %ld\n", GetLastError());
		return 0;
	}

	hFileMapping = OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, lpFileShareName);

	if (hFileMapping == NULL)
	{
		fprintf(stdout, "OpenFileMapping: Error %ld\n", GetLastError());
		return 0;
	}

	lpFileMap = MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

	if (lpFileMap == 0)
	{
		fprintf(stdout, "MapViewOfFile: Error %ld\n", GetLastError());
		return 0;
	}

	while (TRUE)
	{
		gets_s(str);
		if (!strcmp(str, "exit") || !strcmp(str, "Exit") || !strcmp(str, "EXIT"))
			break;
		strcpy((char*)lpFileMap, str);
		SetEvent(hEventSend);
		dwRetCode = WaitForSingleObject(hEventRecv, INFINITE);
		if (dwRetCode == WAIT_OBJECT_0) puts(((LPSTR)lpFileMap));
		if (dwRetCode == WAIT_ABANDONED_0 || dwRetCode == WAIT_FAILED)
		{
			printf("\nError waiting responce!\n)");
			break;
		}
	}

	SetEvent(hEventTermination);
	CloseHandle(hEventSend);
	CloseHandle(hEventRecv);
	CloseHandle(hEventTermination);
	UnmapViewOfFile(lpFileMap);
	CloseHandle(hFileMapping);

	return 0;
}

//#include <iostream>
//#include <windows.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//const int BUF_SIZE = 256;
//
//int main(int argc, char* argv[])
//{
//    using std::wcout;
//    using std::endl;
//    setlocale(LC_ALL, ".UTF8");
//
//    if (argc != 3) {
//        wcout << L"Arguments error\n";
//        return -1;
//    }
//    HANDLE hIn, hOut;
//    DWORD nIn, nOut;
//
//    size_t size = strlen(argv[1]) + 1;
//    wchar_t* inFile = new wchar_t[size];
//    size_t outSize;
//    mbstowcs_s(&outSize, inFile, size, argv[1], size - 1);
//    wchar_t outFile[12] = L"outfile.txt";
//
//    hIn = CreateFile(inFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
//    if (hIn == INVALID_HANDLE_VALUE)
//    {
//        wcout << inFile << L" error1: " << GetLastError() << endl;
//        return 2;
//    }
//
//    hOut = CreateFile((LPCTSTR)outFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//    if (hOut == INVALID_HANDLE_VALUE) {
//        wcout << outFile << L" error2: " << GetLastError() << endl;
//        return 3;
//    }
//
//    wchar_t symbs[3];
//    mbstowcs_s(&outSize, symbs, 3, argv[2], 2);
//    WriteFile(hOut, argv[2], 3, &nOut, NULL);
//    WriteFile(hOut, "\n", 2, &nOut, NULL);
//    int count = 0;
//    wchar_t wbuffer[BUF_SIZE] = { 0 };
//    char buffer[BUF_SIZE] = { 0 };
//    int counter = 0;
//    while (ReadFile(hIn, buffer, BUF_SIZE, &nIn, NULL) && nIn > 0)
//    {
//        mbstowcs_s(&outSize, wbuffer, BUF_SIZE, buffer, BUF_SIZE - 1);
//        for (int i = 0; i < nIn; i++) {
//            if (counter % 3 == 2) {
//                wbuffer[i] = L' ';
//            }
//            counter++;
//        }
//        WriteFile(hOut, wbuffer, nIn, &nOut, NULL);
//    }
//
//    wcout << "\nChanges: " << count;
//    CloseHandle(hIn);
//    CloseHandle(hOut);
//}