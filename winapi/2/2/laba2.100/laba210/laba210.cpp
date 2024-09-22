
#include   <windows.h> 
#include   <stdio.h> 
#include   <stdlib.h> 
#include   <string.h> 

#define BUF_SIZE 1024



int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Arguments error\n");
		return  -1;
	}

	HANDLE hIn, hOut;
	HINSTANCE hlib;
	DWORD nIn, nOut;
	CHAR buffer[BUF_SIZE] = { 0 };

	hlib = LoadLibrary("laba2.100.dll");

	if (hlib == NULL)
	{
		printf("Cannot load library");
		exit(-1);
	}
	typedef int(*re)(CHAR[],char, DWORD);
	typedef void(*fe)(char*, char[]);
	re Replacree;
	fe FillNamee;
	Replacree = (re)GetProcAddress(hlib, "Replacrees");
	FillNamee = (fe)GetProcAddress(hlib, "FillName");
	if (Replacree == NULL) {
		printf("Replacrees function not found");
		exit(-2);
	}
	if (FillNamee == NULL) {
		printf("FillName function not found");
		exit(-2);
	}
	char filename[80] = { 0 };
	FillNamee(argv[1], filename);
	printf("%s\n", filename);
	
	hIn = CreateFile((LPTSTR)argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hIn == INVALID_HANDLE_VALUE)
	{
		printf("Can not open in file: %x\n", GetLastError());
		return  2;
	}

	hOut = CreateFile((LPTSTR)filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		printf("can not open out file: %x\n", GetLastError());
		return 3;
	}
	while (ReadFile(hIn, buffer, BUF_SIZE, &nIn, NULL) && nIn > 0)
	{
		int replace = Replacree(buffer, argv[2][0], nIn);
		printf("%d", replace);
		WriteFile(hOut, buffer, nIn, &nOut, NULL);// записываем строку после модификации
	}
	FreeLibrary(hlib);
	CloseHandle(hIn);
	CloseHandle(hOut);
}