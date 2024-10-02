// Программа из конспекта "Системное программное обеспечение"
// Версия для Windows
// стр. ?? 
// Приложение MTHREADS
// многопоточное приложение, синхронизация мьютексами
#include   <windows.h> 
#include   <stdio.h> 
#include   <string.h> 
#define THREADS 3
// глобальный мьютекс
HANDLE hMutex;
const DWORD BUF_SIZE = 256;
const char STR[3] = "@#";
const char PATH[43] = "D:/code/cplusplus/winapi/mutex/x64/Debug/";
// стркутура для обмена данными
struct MYDATA
{
	int num;
	char file[80];
	char symbs[3];
	int changes;
} data[THREADS];

// функция потока
DWORD WINAPI threadfunc(LPVOID);

int main(int argc, char* argv[])
{
	// дескрипторы потоков
	HANDLE hThreads[THREADS];
	if (argc < 3)
	{
		printf("Usage: mthreads.exe filename ...");
		exit(-1);
	}
	// создание мьютекса
	hMutex = CreateMutexA(NULL, FALSE, NULL);
	if (hMutex == NULL) {
		printf("Can't open mutex. Error: %d", GetLastError());
		exit(-1);
	}
	for (int i = 1; i < argc; i++)
	{
		if (i % 2 == 1) {
			data[(i - 1) / 2].num = (i - 1) / 2;
			strcpy_s(data[(i - 1) / 2].file, 42, PATH);
			strcpy_s(data[(i - 1) / 2].file + 41, strlen(argv[i]) + 1, argv[i]);
			data[(i - 1) / 2].changes = 0;
		}
		else {
			strcpy_s(data[(i - 1) / 2].symbs, 3, argv[i]);
			hThreads[(i-1)/2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadfunc, (LPVOID)&data[(i - 1) / 2].num, 0, NULL);
			if (hThreads[i] == NULL)
			{
				printf("Cant create thread %d!\n", (i - 1) / 2);
				exit(-1);
			}
			else printf("(Main)Create thread %d!\n", (i - 1) / 2);
		}
	}
	/*for (int i = 0; i < 3; i++) {
		printf("Data %d. File: %s. Symbs: %s. Changes: %d\n", data[i].num, data[i].file, data[i].symbs, data[i].changes);
	}*/
	// освобождение мьютекса, чтобы отработали потоки
	ReleaseMutex(hMutex);
	// удаляем мьютекс
	// закрываем дескрипторы потоков
	for (int i = 0; i < THREADS; i ++)
	{
		WaitForSingleObject(hThreads[i], INFINITE);
		printf("(Main)In file %s was done %d changes\n", data[i].file, data[i].changes);
		if (hThreads[i] != INVALID_HANDLE_VALUE)
			CloseHandle(hThreads[i]);
	}
	CloseHandle(hMutex);
}
DWORD WINAPI threadfunc(LPVOID num)
{
	//MYDATA data = *((MYDATA *)src);
	int i =  *((int *)num); //data.num;
	HANDLE hIn, hOut;
	printf("Thread #%d wait mutex...\n", i);
	// захват мьютекса
	WaitForSingleObject(hMutex, INFINITE);
	hIn = CreateFileA(data[i].file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	int len = strlen(data[i].file) + 1;
	char* outfile = new char[len];
	strcpy_s(outfile, len, data[i].file);
	char* pos = strchr(outfile, '.');
	if (pos != nullptr) {
		int letter = pos - outfile;
		outfile[letter + 1] = 'o';
		outfile[letter + 2] = 'u';
		outfile[letter + 3] = 't';
		outfile[letter + 4] = '\0';
	}
	hOut = CreateFileA(outfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hIn !=  INVALID_HANDLE_VALUE && hOut != INVALID_HANDLE_VALUE) 
	{
		char buffer[BUF_SIZE] = { 0 };
		wchar_t wbuffer[BUF_SIZE] = { 0 };
		DWORD nin, nout;
		int count = 0;
		size_t outSize;

		while (ReadFile(hIn, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
		{
			mbstowcs_s(&outSize, wbuffer, BUF_SIZE, buffer, BUF_SIZE - 1);
			for (int j = 0; j < nin; j++) {
				if (wbuffer[j] == data[i].symbs[0] && wbuffer[j + 1] == data[i].symbs[1]) {
					wbuffer[j] = STR[0];
					wbuffer[j + 1] = STR[1];
					j++;
					count++;
				}
			}
			WriteFile(hOut, wbuffer, nin * 2, &nout, NULL);
		}

		CloseHandle(hIn);
		CloseHandle(hOut);
		
		data[i].changes = count;
		printf("(ThreadId: %lu) Successfully editing. Changes: %d\n", GetCurrentThreadId(), data[i].changes);
		ReleaseMutex(hMutex);
		return count;
	}
	else 
	{
		printf("(ThreadId: %lu), Can't open file %s!\n", GetCurrentThreadId(), data[i].file);
		// освобождение мьютекса, иначе он - покинутый!
		ReleaseMutex(hMutex);
		ExitThread(-i);
	}
}