#define _CRT_SECURE_NO_WARNINGS
#include   <windows.h> 
#include   <stdio.h> 
#include   <stdlib.h> 
#include   <string.h> 
#define BUF_SIZE 256

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Недостаточно аргументов\n");
        return  -1;
    }
    HANDLE hIn, hOut;
    DWORD nIn, nOut;
    CHAR buffer[BUF_SIZE] = { 0 };

    char filename[80] = { 0 };

    strcpy(filename, argv[1]);
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
        printf("output file name:%s\n", filename);
    }

    hIn = CreateFile((LPTSTR)argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hIn == INVALID_HANDLE_VALUE)
    {
        printf("Невозможно открыть входной файл. Ошибка: %x\n", GetLastError());
        return  2;
    }

    hOut = CreateFile((LPTSTR)filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hOut == INVALID_HANDLE_VALUE) {
        printf("Невозможно открыть выходной файл. Ошибка: %x\n", GetLastError());
        return 3;
    }
    short count = 0;
    while (ReadFile(hIn, buffer, BUF_SIZE, &nIn, NULL) && nIn > 0)
    {
        char sym = argv[2][0];
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
        printf("\n%d", count);
        WriteFile(hOut, buffer, nIn, &nOut, NULL);

    }
    CloseHandle(hIn);
    CloseHandle(hOut);
}