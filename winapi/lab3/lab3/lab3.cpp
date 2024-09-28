#include <iostream>
#include <windows.h> 

const int MAX_SIZE = 255;

int main(int argc, char* argv[])
{
	using namespace std;
	if (argc < 2) {
		cout << "Lab3 Arguments error\n";
		return 1;
	}
	
	DWORD result, total = 0;
	LPSTARTUPINFOA si = new STARTUPINFOA[127];
	LPPROCESS_INFORMATION pi = new PROCESS_INFORMATION[127];
	HANDLE hThread[127];
	char exeName[MAX_SIZE] = "D:/code/cplusplus/winapi/sem3/labaklava/x64/Debug/test.exe ";
	//char exeName[MAX_SIZE] = "C:/proga/lab4_2/x64/Debug/lab4_2.exe ";
	char* ln;
	for (int i = 1; i < (argc - 1); i+= 2) {
		ln = new char[MAX_SIZE];
		strcpy_s(ln, MAX_SIZE, exeName);
		strcat_s(ln, MAX_SIZE, argv[i]);
		strcat_s(ln, MAX_SIZE, " ");
		strcat_s(ln, MAX_SIZE, argv[i + 1]);
		//size_t size = strlen(ln) + 1;
		//wchar_t* proc = new wchar_t[size];
		//size_t outSize;
		//mbstowcs_s(&outSize, proc, size, ln, size - 1);
		ZeroMemory(&si[i], sizeof(si[i]));
		si[i].cb = sizeof(si);
		ZeroMemory(&pi[i], sizeof(pi[i]));
		cout << ln << endl;
		if (!CreateProcessA(NULL, ln, NULL, NULL, TRUE, NULL, NULL, NULL, &si[i], &pi[i]))
		{
			cout << GetLastError();
			delete[] ln;
			//delete[] proc;
			return 2;
		}
		else 
		{
			hThread[i] = pi[i].hProcess;
			DWORD finish = WaitForSingleObject(hThread[i], INFINITE);
			if (finish == WAIT_OBJECT_0) {
				//printf("Process %Lu is finished\n",pi[i].dwProcessId);
				GetExitCodeProcess(pi[i].hProcess, &result);
				cout << "\nProcess " << pi[i].dwProcessId << " finished by " <<  result << endl;
				total += result;
			}
			else
			{
				CloseHandle(hThread);
			}
		}
	}
	cout << (argc - 1)/2 << " file(s) processed with total spaces " << total;
	delete[] si;
	delete[] pi;
}