#include <stdio.h>
#include <string.h>
#include <winsock2.h>  // Wincosk2.h ������ ���� 
// ��������� ������ windows.h!
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8081
const int BUF_SIZE = 1024;
const char STR[3] = "@#";

// ������ ��� ������ ���������� ��������
// ������������� 
#define PRINTNUSERS if(nclients)\
  printf("%d user on-line\n",nclients);\
  else printf("No User on line\n");

  // �������� �������, �������������
  // �������������� �������������
DWORD WINAPI dostuff(LPVOID client_socket);

// ���������� ���������� � ����������
// �������� ������������� 
int nclients = 0;
char* path;

// ������� �� ��������
int changeFunction(const char* f, const char* s);

int main(int argc, char* argv[])
{
    char buff[1024];    // ����� ��� ��������� ����
    path = new char[strlen(argv[0] + 2)];
    strcpy_s(path, strlen(argv[0]) + 1, argv[0]);
    char second[] = "server.exe";
    char* p = strstr(path, second);
    if (p != nullptr) {
        *p = '\0';
    }
    printf("TCP SERVER DEMO\n");

    // ��� 1 - ������������� ���������� �������
    // �.�. ������������ �������� ����������
    // �� ������������ �� ���������� ��������� ��
    // ������� �����, �������������
    // � ���������  �� ��������� WSADATA.
    // ����� ����� ��������� ���������� ����
    // ����������, ������, ����� ������ ���� �� �����
    // ������������ �������� (��������� WSADATA
    // �������� 400 ����)
    if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
    {
        // ������!
        printf("Error WSAStartup %d\n",
            WSAGetLastError());
        return -1;
    }

    // ��� 2 - �������� ������
    SOCKET mysocket;
    // AF_INET     - ������� ���������
    // SOCK_STREAM  - ��������� ����� (TCP)
    // 0 - �� ��������� ���������� TCP ��������
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        // ������!
        printf("Error socket %d\n", WSAGetLastError());
        WSACleanup();
        // �������������� ���������� Winsock
        return -1;
    }

    // ��� 3 ���������� ������ � ��������� �������
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;	// ������� ������� ���������
    local_addr.sin_port = htons(PORT); // ������ �����
    local_addr.sin_addr.s_addr = INADDR_ANY;	// ������ ��������� �����������
    // �� ��� IP-������

    // �������� bind ��� ����������
    if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
    {
        // ��������� ������
        printf("Error bind %d\n", WSAGetLastError());
        closesocket(mysocket);  // ��������� �����!
        WSACleanup();
        return -1;
    }

    // ��� 4 �������� �����������
    // ������ ������� � 0x100
    if (listen(mysocket, 0x100))
    {
        // ������
        printf("Error listen %d\n", WSAGetLastError());
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }
    printf("Listen...\n");

    // ��� 5 ��������� ��������� �� �������
    SOCKET client_socket;    // ����� �������
    sockaddr_in client_addr;    // ����� �������
    // (����������� ��������)

    // ������� accept ���������� �������� ������ ���������
    int client_addr_size = sizeof(client_addr);

    // ���� ���������� �������� �� ����������� �� �������
    while ((client_socket = accept(mysocket, (sockaddr*)
        &client_addr, &client_addr_size)))
    {
        nclients++; // ����������� �������

        CHAR hostname[1024];
        CHAR b[1024];
        int dwRetVal = getnameinfo((sockaddr*) & client_addr, client_addr_size, hostname, 1024, b, 1024, AF_INET);
        printf("+%s [%s] new connect!\n",
            (hostname) ? hostname : "",
            inet_ntop(AF_INET, (sockaddr_in*)& client_addr.sin_addr, b, 1024));
        PRINTNUSERS

            // ����� ������ ������ ��� ����������� �������
            DWORD thID;
        CreateThread(NULL, NULL, dostuff, &client_socket, NULL, &thID);
    }
    printf("Exit...");
    delete[] path;
    return 0;
}

// ��� ������� ��������� � ��������� ������ �
// ���������� ���������� ��������������� �������
// ���������� �� ���������
DWORD WINAPI dostuff(LPVOID client_socket)
{
    SOCKET my_sock;
    my_sock = ((SOCKET*)client_socket)[0];
    char buff[BUF_SIZE] = { 0 };
    char req[74] = "Enter file's name and symbols which have to be changed: ";
    // ���������� ������� ���������
    send(my_sock, req, strlen(req) + 1, 0);

    int bytes_recv; // ������ ��������� ���������
    int changes = 0;

    // ��������� ������� ���������
    while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) && bytes_recv != SOCKET_ERROR) // �������� ��������� �� �������
    {
        char* cntx = nullptr;
        char* inFile = strtok_s(buff, " ", &cntx);
        char* symbs = cntx;
        changes = changeFunction(inFile, symbs);
        ZeroMemory(buff, sizeof(buff));
        sprintf_s(buff, BUF_SIZE, "%d symbols were changed in the file.\n%s", changes, req);
        send(my_sock, buff, strlen(buff) + 1, 0); // ���������� ������� ���������
    }

    nclients--; // ��������� ������� �������� ��������
    printf("-disconnect\n");
    PRINTNUSERS

    // ��������� �����
    closesocket(my_sock);
    return 0;
}

int changeFunction(const char* f, const char* s)
{
    int len = strlen(path) + strlen(f) + 1;
    char* fullpath = new char[len];
    strcpy_s(fullpath, strlen(path) + 1, path);
    strcpy_s(fullpath + strlen(path), strlen(f) + 1, f);
    FILE* infile;
    errno_t err = fopen_s(&infile, fullpath, "r");
    if (err != 0)
    {    
        printf("(ThreadId: %lu), Can't open file %s. Error: %d!\n", GetCurrentThreadId(), fullpath, GetLastError());
        delete[] fullpath;
        return -1;
    }
    
    char* fullpath2 = new char[len];
    strcpy_s(fullpath2, len, fullpath);
    char* pos = strstr(fullpath2, ".txt");
    if (pos != nullptr) {
        int letter = pos - fullpath2;
        fullpath2[letter + 1] = 'o';
        fullpath2[letter + 2] = 'u';
        fullpath2[letter + 3] = 't';
        fullpath2[letter + 4] = '\0';
    }
    FILE* outfile;
    err = fopen_s(&outfile, fullpath2, "w");
    if(err == 0)
    {
        char buffer[BUF_SIZE] = { 0 };
        int count = 0;
        size_t n;

        while(!feof(infile))
        {
            n = fread(buffer, sizeof(char), BUF_SIZE, infile);
            for (int j = 0; j < n; j++) {
                if (buffer[j] == s[0] && buffer[j + 1] == s[1]) {
                    buffer[j] = STR[0];
                    buffer[j + 1] = STR[1];
                    j++;
                    count++;
                }
            }
            fwrite(buffer, sizeof(char), strlen(buffer), outfile);
        }
        
        fclose(infile);
        fclose(outfile);
        printf("(ThreadId: %lu) Successfully editing. Changes: %d\n", GetCurrentThreadId(), count);
        delete[] fullpath;
        delete[] fullpath2;
        return count;
    }
    else
    {
        fclose(infile);
        printf("(ThreadId: %lu), Can't create file %s. Error: %d!\n", GetCurrentThreadId(), fullpath, GetLastError());
        delete[] fullpath;
        delete[] fullpath2;
        return -1;
    }
}