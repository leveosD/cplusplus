#include <ws2tcpip.h>
#include "Header.h"
#include <AclAPI.h> // for SetSecurityInfo
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8081
const int BUF_SIZE = 1024;
const char STR[3] = "@#";
const char PATH[44] = "D:/code/cplusplus/winapi/service/x64/Debug/";
const int M_SIZE = 80;

int nclients = 0;
SOCKET mysocket;
HANDLE* hThreads;

extern int addLogMessage(const char* text);
int ServiceStart();
void ServiceStop();
int changeFunction(const char*, const char*);
DWORD WINAPI dostuff(LPVOID);

int Server()
{
    SOCKET client_socket;    // сокет клиента
    sockaddr_in client_addr;    // адрес клиента
    // (заполняется системой)
    char message[80] = { 0 };
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
        int dwRetVal = getnameinfo((sockaddr*)&client_addr, client_addr_size, hostname, 1024, b1, 1024, AF_INET);
        sprintf_s(message, M_SIZE, "+%s [%s] new connect!\n",
            (hostname) ? hostname : "",
            inet_ntop(AF_INET, (sockaddr_in*)&client_addr.sin_addr, b2, 1024));
        addLogMessage(message);
        //PRINTNUSERS

            // Вызов нового потока для обслужвания клиента
        DWORD thID;
        CreateThread(NULL, NULL, dostuff, &client_socket, NULL, &thID);
    }
    printf("It's jush finished_/\\('_')/\\_ ");
    // Закрываем идентификаторы объектов-событий  в ServiceStop()
    return 0;
}

int ServiceStart()
{
    char buff[1024];    // Буфер для различных нужд
    char message[80] = { 0 };
    addLogMessage("TCP SERVER DEMO\n");
    if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
    {
        // Ошибка!
        sprintf_s(message, M_SIZE, "Error WSAStartup %d\n",
            WSAGetLastError());
        addLogMessage(message);
        return -1;
    }

    // AF_INET     - система адресации
    // SOCK_STREAM  - потоковый сокет (TCP)
    // 0 - по умолчанию выбирается TCP протокол
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        // Ошибка!
        sprintf_s(message, M_SIZE, "Error socket %d\n", WSAGetLastError());
        addLogMessage(message);
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
        sprintf_s(message, M_SIZE, "Error bind %d\n", WSAGetLastError());
        addLogMessage(message);
        closesocket(mysocket);  // закрываем сокет!
        WSACleanup();
        return -1;
    }

    // Шаг 4 ожидание подключений
    // размер очереди – 0x100
    if (listen(mysocket, 0x100))
    {
        // Ошибка
        sprintf_s(message, M_SIZE, "Error listen %d\n", WSAGetLastError());
        addLogMessage(message);
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }
    printf("Listen...\n");
    return 0;
}

void ServiceStop()
{
	addLogMessage("All Kernel objects closed!");
}

DWORD WINAPI dostuff(LPVOID client_socket)
{
    char message[80] = { 0 };
    SOCKET my_sock;
    my_sock = ((SOCKET*)client_socket)[0];
    char buff[BUF_SIZE];
    char req[74] = "Enter file's name and symbols which have to be changed: ";
    // отправляем клиенту сообщение
    send(my_sock, req, strlen(req) + 1, 0);

    int bytes_recv; // размер принятого сообщения
    int changes = 0;

    // обработка первого параметра
    while ((bytes_recv = recv(my_sock, &buff[0], strlen(buff), 0)) && bytes_recv != SOCKET_ERROR) // принятие сообщения от клиента
    {
        char* cntx = nullptr;
        char* inFile = strtok_s(buff, " ", &cntx);
        char* symbs = cntx;
        changes = changeFunction(inFile, symbs);
        ZeroMemory(buff, sizeof(buff));
        sprintf_s(buff, BUF_SIZE, "%d symbols were changed in the file.\n%s", changes, req);
        send(my_sock, buff, strlen(buff) + 1, 0); // отправляем клиенту сообщение
    }

    nclients--; // уменьшаем счетчик активных клиентов
    sprintf_s(message, M_SIZE, "-disconnect\n");
    addLogMessage(message);
        // закрываем сокет
        closesocket(my_sock);
    return 0;
}

int changeFunction(const char* f, const char* s)
{
    char message[80] = { 0 };
    //HANDLE hIn, hOut;
    int len = 44 + strlen(f);
    char* fullpath = new char[len];
    strcpy_s(fullpath, 44, PATH);
    strcpy_s(fullpath + 43, strlen(f) + 1, f);
    //fullpath[len] = '\0';
    //printf("File: %s\t Symbs: %s", fullpath, s);
    //hIn = CreateFileA(fullpath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    FILE* infile;
    //std::cout << infile << " " << &infile << *infile << std::endl;
    errno_t err = fopen_s(&infile, fullpath, "r");
    if (err != 0)
        /*}
        catch(std::exception e){
            printf("%s\n", e.what());
            delete[] fullpath;
            return -1;
        }*/
        //if (hIn == INVALID_HANDLE_VALUE) {
    {
        sprintf_s(message, M_SIZE, "(ThreadId: %lu), Can't open file %s. Error: %d!\n", GetCurrentThreadId(), fullpath, GetLastError());
        addLogMessage(message);
        delete[] fullpath;
        return -1;
    }

    char* fullpath2 = new char[len];
    strcpy_s(fullpath2, len, fullpath);
    char* pos = strchr(fullpath2, '.');
    if (pos != nullptr) {
        int letter = pos - fullpath2;
        fullpath2[letter + 1] = 'o';
        fullpath2[letter + 2] = 'u';
        fullpath2[letter + 3] = 't';
        fullpath2[letter + 4] = '\0';
    }
    /*try {
        hOut = CreateFileA(outfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    catch (std::exception e) {
        printf("%s\n", e.what());
        delete[] fullpath;
        return -1;
    }*/
    FILE* outfile;
    err = fopen_s(&outfile, fullpath2, "w");
    if (err == 0)
        //if (hOut != INVALID_HANDLE_VALUE)
    {
        char buffer[BUF_SIZE] = { 0 };
        //wchar_t wbuffer[BUF_SIZE] = { 0 };
        //DWORD nin, nout;
        int count = 0;
        size_t n;

        //while (ReadFile(hIn, buffer, BUF_SIZE, &nin, NULL) && nin > 0)
        while (!feof(infile))
        {
            //mbstowcs_s(&outSize, wbuffer, BUF_SIZE, buffer, BUF_SIZE - 1);
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
            char res[10];
            _itoa_s(strlen(buffer), res, 10, 10);
            addLogMessage(res);
            //WriteFile(hOut, wbuffer, nin * 2, &nout, NULL);
        }
        //char* empty = '\0';
        //fwrite(&empty, sizeof(char), 1, outfile);
        //CloseHandle(hIn);
        //CloseHandle(hOut);
        fclose(infile);
        fclose(outfile);
        sprintf_s(message, M_SIZE, "(ThreadId: %lu) Successfully editing. Changes: %d\n", GetCurrentThreadId(), count);
        addLogMessage(message);
        delete[] fullpath;
        delete[] fullpath2;
        return count;
    }
    else
    {
        fclose(infile);
        //CloseHandle(hIn);
        sprintf_s(message, M_SIZE, "(ThreadId: %lu), Can't create file %s. Error: %d!\n", GetCurrentThreadId(), fullpath, GetLastError());
        addLogMessage(message);
        delete[] fullpath;
        delete[] fullpath2;
        return -1;
    }
}