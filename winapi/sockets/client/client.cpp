// ������ �������� TCP-�������
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "winsock2.h"
#include "windows.h"
#include "ws2tcpip.h"
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8081
#define STR_PORT "8080"
#define SERVERADDR "127.0.0.1"  

int main(int argc, char* argv[])
 {
	 char buff[1024];
     printf("TCP DEMO CLIENT\n");

     // ��� 1 - ������������� ���������� Winsock
     if (WSAStartup(0x202, (WSADATA *)&buff[0]))
     {
         printf("WSAStart error %d\n", WSAGetLastError());
         return -1;
     }

     // ��� 2 - �������� ������
     SOCKET my_sock;
     my_sock = socket(AF_INET, SOCK_STREAM, 0);
     if (my_sock < 0)
     {
         printf("Socket() error %d\n", WSAGetLastError());
         return -1;
     }

     // ��� 3 - ��������� ����������
     // ���������� ��������� sockaddr_in - �������� ������ � ����� �������
     sockaddr_in dest_addr;
     dest_addr.sin_family = AF_INET;
     dest_addr.sin_port = htons(PORT);
     //HOSTENT *hst;

     // ��������� IP ������ � ��������� ����������
     if (inet_pton(AF_INET, SERVERADDR, &dest_addr.sin_addr.s_addr) != INADDR_NONE) {
         
     }
     else
     {
         // ������� �������� IP ����� �� ��������� ����� �������
         addrinfo hints;
         addrinfo* result;
         int dwRetval = getaddrinfo(SERVERADDR, STR_PORT, &hints, &result);
         if (dwRetval == 0) 
         {
             ((unsigned long*)&dest_addr.sin_addr)[0] =
                 ((unsigned long**)result)[0][0];
             printf("else: %d", ((unsigned long**)result)[0][0]);
         }
         else
         {
             printf("Invalid address %s\n", SERVERADDR);
             closesocket(my_sock);
             WSACleanup();
             return -1;
         }
     }
     // ����� ������� ������� - �������� ���������� ����������
     if (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
     {
         printf("Connect error %d\n", WSAGetLastError());
         return -1;
     }

     printf("Connect with %s succed\n \
             Type quit for quit\n\n", SERVERADDR);

     // ��� 4 - ������ � �������� ���������
     int nsize = 0;
     while ((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)
     {
         // ������ ����������� ���� � ����� ������
         buff[nsize] = '\0';

         // ������� �� �����
         printf(buff);

         // ������ ���������������� ���� � ����������
         fgets(&buff[0], sizeof(buff) - 1, stdin);

         // �������� �� "quit"
         if (!strcmp(&buff[0], "quit\n"))
         {
             // ���������� �����
             printf("Exit...");
             closesocket(my_sock);
             WSACleanup();
             return 0;
         }

         // �������� ������ ������� �������
         send(my_sock, &buff[0], strlen(&buff[0]), 0);
     }
     printf("Recv error %d\n", WSAGetLastError());
     closesocket(my_sock);
     WSACleanup();
     return -1;
 }