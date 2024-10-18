#include <iostream>
#include <windows.h> 

HINSTANCE hLib;
typedef int(*ChangeText)(HANDLE, HANDLE, char*, char*);
char STR[3] = "#@";
const char PATH[] = "D:/code/cplusplus/winapi/dll_project/x64/Debug/";

int addLogMessage(const char* text);

int main(int argc, char* argv[])
{
	using std::wcout;
	using std::endl;
	if (argc != 3) {
		addLogMessage("Arguments error");
		wcout << L"Arguments error\n";
		return 1;
	}
	hLib = LoadLibrary(L"dll_project.dll");
	if (hLib == NULL) {
		wcout << L"Error " << GetLastError();
		addLogMessage("Can't load dll");
		return 2;
	}
	
	ChangeText ct = (ChangeText)GetProcAddress(hLib, "ChangeText");
	if (ct == NULL) {
		wcout << L"Error " << GetLastError();
		addLogMessage("Can't load function");
		return 3;
	}
	addLogMessage(argv[0]);
	addLogMessage(argv[1]);
	addLogMessage(argv[2]);
	HANDLE hIn, hOut;
	char* outFile;
	//wcout << argv[1];
	hIn = CreateFileA(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hIn == INVALID_HANDLE_VALUE) {
		addLogMessage("Original infile can't be opened");
		size_t size = strlen(argv[1]) + strlen(PATH) + 1;
		char* inFile = new char[size];
		strcpy_s(inFile, strlen(PATH) + 1, PATH);
		strcat_s(inFile, size, argv[1]);
		outFile = new char[size];
		strcpy_s(outFile, size, inFile);
		hIn = CreateFileA(inFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
		if (hIn == INVALID_HANDLE_VALUE)
		{
			wcout << "In File Error: " << inFile << " " << GetLastError() << endl;
			addLogMessage("Can't open in file with PAth");
			addLogMessage(inFile);
			delete[] inFile;
			return 4;
		}
		else {
			addLogMessage("Infile with PATH was opened");
			addLogMessage(inFile);
		}
		delete[] inFile;
	}
	else {
		addLogMessage("Original infile was opened");
		addLogMessage(argv[1]);
		outFile = new char[strlen(argv[1]) + 1];
		strcpy_s(outFile, strlen(argv[1]) + 1, argv[1]);
	}
	char* pos = strchr(outFile, '.');
	if (pos != NULL)
	{
		int letter = pos - outFile;
		outFile[letter + 1] = L'o';
		outFile[letter + 2] = L'u';
		outFile[letter + 3] = L't';
		outFile[letter + 4] = L'\0';
	}
	addLogMessage(outFile);
	hOut = CreateFileA(outFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		wcout << L"Error: " << GetLastError();
		addLogMessage("Can't open out file");
		addLogMessage(outFile);
		return 5;
	}
	else {
		addLogMessage("Outfile was opened");
		addLogMessage(outFile);
	}
	
	addLogMessage("Start func...");
	wcout << "Changes: " << ct(hIn, hOut, argv[2], STR);
	addLogMessage("Func finished.");

	delete[] outFile;
	FreeLibrary(hLib);
	CloseHandle(hIn);
	CloseHandle(hOut);
}

int addLogMessage(const char* text)
{
	DWORD res, Sz;
	HANDLE hFile;
	char buf[256];
	hFile = CreateFileA("D:/code/cplusplus/winapi/create_process/logfile.log", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
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