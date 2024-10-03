//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <winsock2.h>  // Wincosk2.h должен быть 
// подключен раньше windows.h!
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 666
const int BUF_SIZE = 20 * 1024;
const char STR[3] = "@#";

// макрос для печати количества активных
// пользователей 
#define PRINTNUSERS if(nclients)\
  printf("%d user on-line\n",nclients);\
  else printf("No User on line\n");

  // прототип функции, обслуживающий
  // подключившихся пользователей
DWORD WINAPI dostuff(LPVOID client_socket);

// глобальная переменная – количество
// активных пользователей 
int nclients = 0;

// функция по варианту
int changeFunction(const char* f, const char* s);

int main(int argc, char* argv[])
{
    char buff[1024];    // Буфер для различных нужд

    printf("TCP SERVER DEMO\n");

    // Шаг 1 - Инициализация Библиотеки Сокетов
    // Т.к. возвращенная функцией информация
    // не используется ей передается указатель на
    // рабочий буфер, преобразуемый
    // к указателю  на структуру WSADATA.
    // Такой прием позволяет сэкономить одну
    // переменную, однако, буфер должен быть не менее
    // полкилобайта размером (структура WSADATA
    // занимает 400 байт)
    if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
    {
        // Ошибка!
        printf("Error WSAStartup %d\n",
            WSAGetLastError());
        return -1;
    }

    // Шаг 2 - создание сокета
    SOCKET mysocket;
    // AF_INET     - система адресации
    // SOCK_STREAM  - потоковый сокет (TCP)
    // 0 - по умолчанию выбирается TCP протокол
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        // Ошибка!
        printf("Error socket %d\n", WSAGetLastError());
        WSACleanup();
        // Деиницилизация библиотеки Winsock
        return -1;
    }

    // Шаг 3 связывание сокета с локальным адресом
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;	// задание системы адресации
    local_addr.sin_port = htons(PORT); // задние порта
    local_addr.sin_addr.s_addr = INADDR_ANY;	// сервер принимает подключения
    // на все IP-адреса

// вызываем bind для связывания
    if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
    {
        // обработка ошибка
        printf("Error bind %d\n", WSAGetLastError());
        closesocket(mysocket);  // закрываем сокет!
        WSACleanup();
        return -1;
    }

    // Шаг 4 ожидание подключений
    // размер очереди – 0x100
    if (listen(mysocket, 0x100))
    {
        // Ошибка
        printf("Error listen %d\n", WSAGetLastError());
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }
    printf("Listen...\n");

    // Шаг 5 извлекаем сообщение из очереди
    SOCKET client_socket;    // сокет клиента
    sockaddr_in client_addr;    // адрес клиента
    // (заполняется системой)

// функции accept необходимо передать размер структуры
    int client_addr_size = sizeof(client_addr);

    // цикл извлечения запросов на подключение из очереди
    while ((client_socket = accept(mysocket, (sockaddr*)
        &client_addr, &client_addr_size)))
    {
        nclients++; // увеличиваем счетчик
        // подключившихся клиентов

  // вывод сведений о клиенте
        
        //hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);

        CHAR hostname[1024];
        CHAR b1[1024];
        CHAR b2[1024];
        int dwRetVal = getnameinfo((sockaddr*) & client_addr, client_addr_size, hostname, 1024, b1, 1024, AF_INET);
        printf("+%s [%s] new connect!\n",
            (hostname) ? hostname : "",
            inet_ntop(AF_INET, (sockaddr_in*)& client_addr.sin_addr, b2, 1024));
        PRINTNUSERS

            // Вызов нового потока для обслужвания клиента
            DWORD thID;
        CreateThread(NULL, NULL, dostuff, &client_socket, NULL, &thID);
    }
    return 0;
}

// Эта функция создается в отдельном потоке и
// обсуживает очередного подключившегося клиента
// независимо от остальных
DWORD WINAPI dostuff(LPVOID client_socket)
{
    SOCKET my_sock;
    my_sock = ((SOCKET*)client_socket)[0];
    char buff[BUF_SIZE];
#define str1 "Enter 1 parametr\r\n"
#define str2 "Enter 2 parametr\r\n" 
    // отправляем клиенту сообщение
    //send(my_sock, str1, sizeof(str1), 0);

    int bytes_recv; // размер принятого сообщения
    int changes = 0;
    char res[10];

    // обработка первого параметра
    if ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) // принятие сообщения от клиента
    {
        char* cntx = nullptr;
        char* inFile = strtok_s(buff, " ", &cntx);
        char* symbs = strtok_s(NULL, " ", &cntx);
        changes = changeFunction(inFile, symbs);
        _itoa_s(changes, res, 10, 10);
        send(my_sock, res, strlen(res) + 1, 0); // отправляем клиенту сообщение
    }

    // обработка второго параметра
    //if ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) // принятие сообщения от клиента
    //{
    //    b = atoi(buff); // преобразование второго параметра в int
    //    
    //    send(my_sock, &buff[0], bytes_recv, 0); // отправляем клиенту результат
    //}

    nclients--; // уменьшаем счетчик активных клиентов
    printf("-disconnect\n");
    PRINTNUSERS

        // закрываем сокет
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
        // освобождение мьютекса, иначе он - покинутый!
        return -1;
    }
}