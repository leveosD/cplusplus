#include <iostream>
#include <windows.h> 

const char STR[3] = "#@";
const int STR_SIZE = 128;
const int BUF_SIZE = 256;
//const char PATH[] = "D:\\code\\cplusplus\\winapi\\pipe_project\\x64\\Release\\";
char* path;
char* logfile;
int addLogMessage(const char* text);

int main(int argc, char* argv[])
{
	using namespace std;
	int count = 0;
	path = new char[strlen(argv[0]) + 2];
	strcpy_s(path, strlen(argv[0]) + 1, argv[0]);
	char second[] = "second_process.exe";
	char* p = strstr(path, second);
	if (p != nullptr) {
		*p = '\0';
	}

	logfile = new char[strlen(path) + 12];
	strcpy_s(logfile, strlen(path) + 1, path);
	strcpy_s(logfile + strlen(path), 12, "logfile.log");
	char message[STR_SIZE] = { 0 };
	sprintf_s(message, STR_SIZE, "Path: %s", path);
	addLogMessage(message);
	char* filename;
	char* symbs;
	char result[10] = { 0 };
	char buff[STR_SIZE] = { 0 };
	DWORD cbWritten;
	DWORD cbRead;
	if (!ReadFile(GetStdHandle(STD_INPUT_HANDLE), buff, STR_SIZE, &cbRead, NULL)) {
		char error[12] = "Error: ";
		char code[5];
		_itoa_s(GetLastError(), code, 5, 10);
		strcat_s(error, 12, code);
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), error, 12, &cbWritten, NULL);
		addLogMessage(error);
		return 1;
	}
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\n(Child) Get filename to change file. ", 36, &cbWritten, NULL);
	char* cntx = nullptr;
	sprintf_s(message, STR_SIZE, "Get data: %s", buff);
	filename = strtok_s(buff, " ", &cntx);
	symbs = cntx;
	sprintf_s(message, STR_SIZE, "Filename: %s. Symbs: %s", filename, symbs);
	addLogMessage(message);
	//size_t size = strlen(argv[1]) + 1;
	//wchar_t* inFile = new wchar_t[size];

	HANDLE hIn, hOut;
	//wcout << argv[1];
	int len = strlen(path) + strlen(filename) + 1;
	char* infile = new char[len];
	strcpy_s(infile, strlen(path) + 1, path);
	strcpy_s(infile + strlen(path), strlen(filename) + 1, filename);
	sprintf_s(message, STR_SIZE, "Infile: %s", infile);
	addLogMessage(message);
	//printf("%s\n", filename);
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
	sprintf_s(message, STR_SIZE, "Outfile: %s", outfile);
	addLogMessage(message);
	hOut = CreateFileA(outfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hIn != INVALID_HANDLE_VALUE && hOut != INVALID_HANDLE_VALUE)
	{
		char buffer[BUF_SIZE] = { 0 };
		wchar_t wbuffer[BUF_SIZE] = { 0 };
		DWORD nin, nout;
		size_t outSize;

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
	}
	else 
	{

		sprintf_s(message, STR_SIZE, "Infile or outfile error. Error: %d", GetLastError());
		addLogMessage(message);
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\nINFILE OR OUTFILE ERROR", 24, &cbWritten, NULL);
		delete[] infile;
		delete[] outfile;
		return 0;
	}
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\n(Child) File has been changed. ", 32, &cbWritten, NULL);
	
	delete[] infile;
	delete[] outfile;
	CloseHandle(hIn);
	CloseHandle(hOut);
	return count;
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