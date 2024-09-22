#include"pch.h"
#include<stdio.h> 
#include<string.h> 
#define EXP  __declspec(dllexport)

int EXP Replacrees(CHAR buffer[], char sym, DWORD nIn);
void EXP FillName(char name[], char filename[]);


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(NULL, "The DLL is loaded", "DLL Skeleton", MB_OK);
		break;
	case DLL_THREAD_ATTACH:
		MessageBox(NULL, "A thread is created in this process", "DLL Skeleton", MB_OK);
		break;
	case DLL_THREAD_DETACH:
		MessageBox(NULL, "A thread is destroyed in this process", "DLL Skeleton", MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(NULL, "The DLL is unloaded", "DLL Skeleton", MB_OK);
		break;
	}
	return TRUE;
}

int EXP Replacrees(CHAR buffer[],char sym, DWORD nIn)
{
	// модифицируем строку в буфере
	short count = 0;
	for (int i = 0; i < nIn; i++)
	{
		if (buffer[i + 1] == '\0')
		{
			buffer[i] = sym;
			count++;
		}
		if (buffer[i + 1] == '\r' and buffer[i] != '\n')
		{
			buffer[i] = sym;
			count++;
		}
	}
	return count;
	//// модифицируем строку в буфере
}
void EXP FillName(char name[], char filename[])
{
	strcpy(filename, name);
	char* pos = strstr(filename, ".");
	if (pos == NULL)
		strcat(filename, ".out");
	else
	{
		short letter = pos - filename;
		filename[letter + 1] = 't';
		filename[letter + 2] = 'a';
		filename[letter + 3] = 'y';
		filename[letter + 4] = 0;
	}
}

