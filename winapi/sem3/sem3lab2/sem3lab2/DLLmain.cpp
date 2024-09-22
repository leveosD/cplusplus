#include <windows.h>

__declspec(dllexport) int __cdecl ChangeText(wchar_t*, wchar_t*);
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(NULL, L"The DLL is loaded", L"DLL Skeleton", MB_OK);
		break;
	case DLL_THREAD_ATTACH:
		MessageBox(NULL, L"A thread is created in this process", L"DLL Skeleton", MB_OK);
		break;
	case DLL_THREAD_DETACH:
		MessageBox(NULL, L"A thread is destroyed in this process", L"DLL Skeleton", MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(NULL, L"The DLL is unloaded", L"DLL Skeleton", MB_OK);
		break;
	}
	return TRUE;
}
