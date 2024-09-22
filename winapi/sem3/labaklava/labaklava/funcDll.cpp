#include "pch.h"
#include <iostream>
//__declspec(dllexport) void _cdecl ChangeText(HANDLE hIn, HANDLE hOut, const wchar_t* old_old_symbs, const wchar_t* new_old_symbs)
extern "C" __declspec(dllexport) int ChangeText(HANDLE hIn, HANDLE hOut, wchar_t* old_symbs, wchar_t* new_symbs)
{
	const int BUF_SIZE = 256;
	char buffer[BUF_SIZE] = { 0 };
	DWORD nin, nout;
	int count = 0;
	while (ReadFile(hIn, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
	{
		wchar_t str[BUF_SIZE] = { 0 };
		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, buffer, nin, str, BUF_SIZE);
		for (int i = 0; i < nin; i++) {
			if (str[i] == old_symbs[0] && str[i + 1] == old_symbs[1]) {
				str[i] = new_symbs[0];
				str[i + 1] = new_symbs[1];
				i++;
				count++;
			}
		}
		BOOL flag;
		char changedstr[BUF_SIZE] = { 0 };
		WideCharToMultiByte(CP_UTF8, WC_DEFAULTCHAR, str, nin, changedstr, BUF_SIZE, NULL, &flag);
		WriteFile(hOut, changedstr, nin, &nout, NULL);
	}
	return count;
}