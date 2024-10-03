//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <winsock2.h>  // Wincosk2.h ������ ���� 
// ��������� ������ windows.h!
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 666
const int BUF_SIZE = 20 * 1024;
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

// ������� �� ��������
int changeFunction(const char* f, const char* s);

int main(int argc, char* argv[])
{
    char buff[1024];    // ����� ��� ��������� ����

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
        // �������������� ��������

  // ����� �������� � �������
        
        //hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);

        CHAR hostname[1024];
        CHAR b1[1024];
        CHAR b2[1024];
        int dwRetVal = getnameinfo((sockaddr*) & client_addr, client_addr_size, hostname, 1024, b1, 1024, AF_INET);
        printf("+%s [%s] new connect!\n",
            (hostname) ? hostname : "",
            inet_ntop(AF_INET, (sockaddr_in*)& client_addr.sin_addr, b2, 1024));
        PRINTNUSERS

            // ����� ������ ������ ��� ����������� �������
            DWORD thID;
        CreateThread(NULL, NULL, dostuff, &client_socket, NULL, &thID);
    }
    return 0;
}

// ��� ������� ��������� � ��������� ������ �
// ���������� ���������� ��������������� �������
// ���������� �� ���������
DWORD WINAPI dostuff(LPVOID client_socket)
{
    SOCKET my_sock;
    my_sock = ((SOCKET*)client_socket)[0];
    char buff[BUF_SIZE];
#define str1 "Enter 1 parametr\r\n"
#define str2 "Enter 2 parametr\r\n" 
    // ���������� ������� ���������
    //send(my_sock, str1, sizeof(str1), 0);

    int bytes_recv; // ������ ��������� ���������
    int changes = 0;
    char res[10];

    // ��������� ������� ���������
    if ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) // �������� ��������� �� �������
    {
        char* cntx = nullptr;
        char* inFile = strtok_s(buff, " ", &cntx);
        char* symbs = strtok_s(NULL, " ", &cntx);
        changes = changeFunction(inFile, symbs);
        _itoa_s(changes, res, 10, 10);
        send(my_sock, res, strlen(res) + 1, 0); // ���������� ������� ���������
    }

    // ��������� ������� ���������
    //if ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) // �������� ��������� �� �������
    //{
    //    b = atoi(buff); // �������������� ������� ��������� � int
    //    
    //    send(my_sock, &buff[0], bytes_recv, 0); // ���������� ������� ���������
    //}

    nclients--; // ��������� ������� �������� ��������
    printf("-disconnect\n");
    PRINTNUSERS

        // ��������� �����
        closesocket(my_sock);
    return 0;
}

int changeFunction(const char* f, const char* s)
{
    HANDLE hIn, hOut;
    hIn = CreateFileA(f, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    int len = strlen(f) + 1;
    char* outfile = new char[len];
    strcpy_s(outfile, len, f);
    char* pos = strchr(outfile, '.');
    if (pos != nullptr) {
        int letter = pos - outfile;
        outfile[letter + 1] = 'o';
        outfile[letter + 2] = 'u';
        outfile[letter + 3] = 't';
        outfile[letter + 4] = '\0';
    }
    hOut = CreateFileA(outfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hIn != INVALID_HANDLE_VALUE && hOut != INVALID_HANDLE_VALUE)
    {
        char buffer[BUF_SIZE] = { 0 };
        wchar_t wbuffer[BUF_SIZE] = { 0 };
        DWORD nin, nout;
        int count = 0;
        size_t outSize;

        while (ReadFile(hIn, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
        {
            mbstowcs_s(&outSize, wbuffer, BUF_SIZE, buffer, BUF_SIZE - 1);
            for (int j = 0; j < nin; j++) {
                if (wbuffer[j] == s[0] && wbuffer[j + 1] == s[1]) {
                    wbuffer[j] = STR[0];
                    wbuffer[j + 1] = STR[1];
                    j++;
                    count++;
                }
            }
            WriteFile(hOut, wbuffer, nin * 2, &nout, NULL);
        }

        CloseHandle(hIn);
        CloseHandle(hOut);
        printf("(ThreadId: %lu) Successfully editing. Changes: %d\n", GetCurrentThreadId(), count);
        return count;
    }
    else
    {
        printf("(ThreadId: %lu), Can't open file %s!\n", GetCurrentThreadId(), f);
        // ������������ ��������, ����� �� - ���������!
        return -1;
    }
}