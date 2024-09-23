#include "pch.h"
#include <iostream>
//__declspec(dllexport) void _cdecl ChangeText(HANDLE hIn, HANDLE hOut, const wchar_t* old_old_symbs, const wchar_t* new_old_symbs)
extern "C" __declspec(dllexport) int ChangeText(HANDLE hIn, HANDLE hOut, wchar_t* old_symbs, wchar_t* new_symbs)
{
	const int BUF_SIZE = 256;
	char buffer[BUF_SIZE] = { 0 };
	wchar_t wbuffer[BUF_SIZE] = { 0 };
	DWORD nin, nout;
	int count = 0;
	size_t outSize;
	while (ReadFile(hIn, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
	{
		mbstowcs_s(&outSize, wbuffer, BUF_SIZE, buffer, BUF_SIZE - 1);
		for (int i = 0; i < nin; i++) {
			if (wbuffer[i] == old_symbs[0] && wbuffer[i + 1] == old_symbs[1]) {
				wbuffer[i] = new_symbs[0];
				wbuffer[i + 1] = new_symbs[1];
				i++;
				count++;
			}
		}
		WriteFile(hOut, wbuffer, nin * 2, &nout, NULL);
	}
	return count;
}