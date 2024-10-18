#include "pch.h"
#include <iostream>
//__declspec(dllexport) void _cdecl ChangeText(HANDLE hIn, HANDLE hOut, const wchar_t* old_old_symbs, const wchar_t* new_old_symbs)
extern "C" __declspec(dllexport) int ChangeText(HANDLE hIn, HANDLE hOut, char* old_symbs, char* new_symbs)
{
	const int  BUF_SIZE = 256;
	char buffer[BUF_SIZE] = { 0 };
	wchar_t wbuffer[BUF_SIZE] = { 0 };
	DWORD nin, nout;
	int count = 0;
	size_t outSize;

	while (ReadFile(hIn, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
	{
		mbstowcs_s(&outSize, wbuffer, BUF_SIZE, buffer, BUF_SIZE - 1);
		for (int j = 0; j < nin; j++) {
			if (buffer[j] == old_symbs[0] && buffer[j + 1] == old_symbs[1]) {
				buffer[j] = new_symbs[0];
				buffer[j + 1] = new_symbs[1];
				j++;
				count++;
			}
		}
		WriteFile(hOut, buffer, nin, &nout, NULL);
	}
	return count;
}