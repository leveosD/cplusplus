#include <iostream>
#include "windows.h"
#include "windows.h"

const int STR_SIZE = 128;
//аргументы командной строки: text.txt ag
int addLogMessage(const char* text);
char* logfile;

int main(int argc, char* argv[])
{
	using namespace std;
	DWORD cbWritten, cbRead;
	HANDLE hReadPipe1, hWritePipe1, hReadPipe2, hWritePipe2;
	//char filename[] = "D:\\code\\cplusplus\\winapi\\pipe_project\\x64\\Release\\second_process.exe";
	char* path = new char[strlen(argv[0]) + 1];
	strcpy_s(path, strlen(argv[0]) + 1, argv[0]);
	char second[] = "second_process.exe";
	char* p = strstr(path, "first");
	if(p != nullptr)
		*p = '\0';
	char* filename = new char[strlen(argv[0]) + 2];
	strcpy_s(filename, strlen(argv[0]) + 3, path);
	strcat_s(filename, strlen(argv[0]) + 3, second);

	logfile = new char[strlen(path) + 12];
	strcpy_s(logfile, strlen(path) + 1, path);
	strcpy_s(logfile + strlen(path), 12, "logfile.log");
	addLogMessage(filename);
	addLogMessage(logfile);
	char result[STR_SIZE] = { 0 };
	char* arguments = new char[STR_SIZE];
	strcpy_s(arguments, STR_SIZE, argv[1]);
	strcat_s(arguments, STR_SIZE, " ");
	strcat_s(arguments, STR_SIZE, argv[2]);
	addLogMessage(arguments);
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
	int p1 = CreatePipe(&hReadPipe1, &hWritePipe1, &PipeSA, STR_SIZE);
	int p2 = CreatePipe(&hReadPipe2, &hWritePipe2, &PipeSA, STR_SIZE);
	if (p1 == 0 || p2 == 0)
	{
		addLogMessage("createPipe error");
		cout << "(first)CreatePipeError. Error: " << GetLastError() << endl;
		return 1;
	}
	ZeroMemory(si, sizeof(*si));
	si->hStdInput = hReadPipe1;
	si->hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si->hStdOutput = hWritePipe2;
	si->dwFlags = STARTF_USESTDHANDLES;
	si->cb = sizeof(si);
	ZeroMemory(pi, sizeof(*pi));
	int code = CreateProcessA(filename, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, si, pi);
	if (GetLastError() == 183) {
		cout << "Terminate procces" << endl;
		TerminateProcess(pi->hProcess, 0);
		code = CreateProcessA(filename, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, si, pi);
	}
	if (code == 0) {
		cout << "Error: " << GetLastError() << "Code: " << code << endl;
		cout << "CreateProcess failed. Error: " << GetLastError();
		CloseHandle(hReadPipe2);
		CloseHandle(hWritePipe1);
		return 2;
	}
	WriteFile(hWritePipe1, arguments, strlen(arguments) + 1, &cbWritten, NULL);
	cout << "(Parent)Bytes written: " << (int)cbWritten << endl;
	delete[] arguments;

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
	DWORD res;
	//LPDWORD res;

	WaitForSingleObject(pi->hProcess, INFINITE);
	GetExitCodeProcess(pi->hProcess, &res);
	cout << "(Parent)" << res << " changes were done." << endl;
	
	CloseHandle(hReadPipe2);
	CloseHandle(hWritePipe1);
	CloseHandle(pi->hProcess);
	delete[] filename;
	return 0;
}

int addLogMessage(const char* text)
{
	DWORD res, Sz;
	HANDLE hFile;
	char buf[256];
	hFile = CreateFileA(logfile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
	if (!hFile) return (-1);
	else
	{
		GetFileSize(hFile, &Sz);
		SetFilePointer(hFile, 0, NULL, FILE_END);
		sprintf_s(buf, "%s\r\n", text);
		WriteFile(hFile, buf, strlen(buf), &res, NULL);
		CloseHandle(hFile);
		return (int)res;
	}
}