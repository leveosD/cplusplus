#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

HANDLE hSemaphoreSend;
HANDLE hSemaphoreRecv;
HANDLE hSemaphoreTermination;
CHAR lpSemaphoreSendName[] =
"$MylSemaphoreSendName$";
CHAR lpSemaphoreRecvName[] =
"$MylSemaphoreRecvName$";
CHAR lpSemaphoreTerminationName[] =
"$MylSemaphoreTerminationName$";
CHAR lpFileShareName[] =
"$MyFileShareName$";
HANDLE hFileMapping;
LPVOID lpFileMap;

int main()
{
	CHAR request[80];
	CHAR response[80];
	DWORD dwRetCode;
	printf("Mapped and shared file, Semaphore sync, client process"
		"\nEnter  <Exit> to terminate...\n");

	hSemaphoreSend = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, lpSemaphoreSendName);
	hSemaphoreRecv = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, lpSemaphoreRecvName);
	hSemaphoreTermination = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, lpSemaphoreTerminationName);
	if (hSemaphoreSend == NULL || hSemaphoreRecv == NULL || hSemaphoreTermination == NULL)
	{
		printf("OpenSemaphore: Error %ld\n", GetLastError());
		return 0;
	}

	hFileMapping = OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, lpFileShareName);

	if (hFileMapping == NULL)
	{
		printf("OpenFileMapping: Error %ld\n", GetLastError());
		return 0;
	}

	lpFileMap = MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

	if (lpFileMap == 0)
	{
		printf("MapViewOfFile: Error %ld\n", GetLastError());
		return 0;
	}

	while (TRUE)
	{
		gets_s(request);
		if (!strcmp(request, "exit") || !strcmp(request, "Exit") || !strcmp(request, "EXIT"))
			break;
		strcpy((char*)lpFileMap, request);
		ReleaseSemaphore(hSemaphoreSend, 1, NULL);
		dwRetCode = WaitForSingleObject(hSemaphoreRecv, INFINITE);
		if (dwRetCode == WAIT_OBJECT_0) 
		{
			//puts(((char*)lpFileMap));
			strcpy(response, ((char*)lpFileMap));
			printf("Response: %s", response);
			
		}
		else if (dwRetCode == WAIT_ABANDONED || dwRetCode == WAIT_FAILED)
		{
			printf("Error waiting responce!\n)");
			break;
		}
	}

	ReleaseSemaphore(hSemaphoreTermination, 1, NULL);
	//printf("%d", res);
	CloseHandle(hSemaphoreSend);
	CloseHandle(hSemaphoreRecv);
	CloseHandle(hSemaphoreTermination);
	UnmapViewOfFile(lpFileMap);
	CloseHandle(hFileMapping);
	return 0;
}
