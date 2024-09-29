#include <iostream>
#include "windows.h"
#include "windows.h"

const int STR_SIZE = 128;

//аргументы командной строки: text.txt ag
int main(int argc, char* argv[])
{
	using namespace std;
	DWORD cbWritten, cbRead;
	HANDLE hReadPipe1, hWritePipe1, hReadPipe2, hWritePipe2;
	char filename[STR_SIZE] = "D:/code/cplusplus/winapi/pipe_project/x64/Debug/second_process.exe", result[STR_SIZE] = { 0 };
	char* arguments = new char[STR_SIZE];
	strcpy_s(arguments, STR_SIZE, argv[1]);
	strcat_s(arguments, STR_SIZE, " ");
	strcat_s(arguments, STR_SIZE, argv[2]);
	SECURITY_ATTRIBUTES PipeSA = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	LPSTARTUPINFOA si = new STARTUPINFOA;
	LPPROCESS_INFORMATION pi = new PROCESS_INFORMATION;
	//PROCESS_INFORMATION pi;
	//STARTUPINFO si;
	if (argc != 3)
	{
		cout << "Arguments error\n";
		return 1;
	}
	CreatePipe(&hReadPipe1, &hWritePipe1, &PipeSA, STR_SIZE);
	CreatePipe(&hReadPipe2, &hWritePipe2, &PipeSA, STR_SIZE);
	ZeroMemory(si, sizeof(*si));
	si->hStdInput = hReadPipe1;
	si->hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si->hStdOutput = hWritePipe2;
	si->dwFlags = STARTF_USESTDHANDLES;
	si->cb = sizeof(si);
	ZeroMemory(pi, sizeof(*pi));
	if (!CreateProcessA(filename, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, si, pi))
	{
		cout << "CreateProcess failed.  Error: " << GetLastError();
		return 2;
	}
	WriteFile(hWritePipe1, arguments, strlen(arguments) + 1, &cbWritten, NULL);
	cout << "(Parent)Bytes written: " << (int)cbWritten << endl;
	delete[] arguments;
	cout << endl;

	if(!ReadFile(hReadPipe2, result, STR_SIZE, &cbRead, NULL)) {
		cout << "Read file error. Error: " << GetLastError() << endl;
		return 3;
	}
	cout << "(Parent)Bytes read: " << cbRead << ". Message: " <<  result << endl;

	char result2[STR_SIZE] = { 0 };
	if (!ReadFile(hReadPipe2, result2, STR_SIZE, &cbRead, NULL)) {
		cout << "Read file error. Error: " << GetLastError() << endl;
		return 3;
	}
	cout << "(Parent)Bytes read: " << cbRead << ". Message: " << result2 << endl;

	WaitForSingleObject(pi->hProcess, INFINITE);
	CloseHandle(hReadPipe2);
	CloseHandle(hWritePipe1);
	CloseHandle(pi->hProcess);
	return 0;
}