﻿// Программа из конспекта "Системное программное обеспечение"
// Версия для Windows
// стр. ?? 
// Приложение Service
// Служба Windows, использующая объекты ядра
#include "Header.h"
// -----------------------------------------------------
// Глобальные переменные
// -----------------------------------------------------
// Код ошибки
DWORD dwErrCode;
// Текущее состояние сервиса
SERVICE_STATUS ss;
// Идентификатор сервиса
SERVICE_STATUS_HANDLE ssHandle;
const char* MyServiceName = "Sample service";
char* path;
char* logfile;
// функции службы, выполняющие работу
extern int Server();
extern int ServiceStart();
extern void ServiceStop();

// функции управления службой
void WINAPI ServiceMain(DWORD dwArgc, LPSTR* lpszArv);
void WINAPI ServiceControl(DWORD dwControlCode);
void ReportStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
PROCESS_INFORMATION pid;
// ведение логов
int addLogMessage(const char* text)
{
	DWORD res, Sz;
	HANDLE hFile;
	char buf[256];
	hFile = CreateFileA(logfile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
	if (!hFile) return (-1);
	else
	{
		GetFileSize(hFile, &Sz);
		SetFilePointer(hFile, 0, NULL, FILE_END);
		sprintf_s(buf, "%s\r\n", text);
		WriteFile(hFile, buf, strlen(buf), &res, NULL);
		CloseHandle(hFile);
		return (int)res;
	}
}
// -----------------------------------------------------
// Функция main
// Точка входа процесса
// -----------------------------------------------------
void main(int agrc, char* argv[])
{
	path = new char[strlen(argv[0] + 2)];
	strcpy_s(path, strlen(argv[0]) + 1, argv[0]);
	char name[] = "service.exe";
	char* p = strstr(path, name);
	if (p != nullptr) {
		*p = '\0';
	}
	logfile = new char[strlen(path) + 12];
	strcpy_s(logfile, strlen(path) + 1, path);
	strcpy_s(logfile + strlen(path), 12, "logfile.log");

	addLogMessage("PATH");
	addLogMessage(path);
	char buffer[256];
	// Таблица точек входа
	SERVICE_TABLE_ENTRYA DispatcherTable[] =
	{
		{
			// Имя сервиса
			(LPSTR)MyServiceName,
			// Функция main сервиса
			(LPSERVICE_MAIN_FUNCTIONA)ServiceMain
		},
		{
			NULL,
			NULL
		}
	};
	addLogMessage("Sample service entry point");
	// Запуск диспетчера
	if (!StartServiceCtrlDispatcherA(DispatcherTable))
	{
		sprintf_s(buffer, "StartServiceCtrlDispatcher: Error %ld\n", GetLastError());
		addLogMessage(buffer);
		return;
	}
}
// -----------------------------------------------------
// Функция ServiceMain
// Точка входа сервиса 
// -----------------------------------------------------
void WINAPI ServiceMain(DWORD argc, LPSTR* argv)
{
	char buf[256];
	int res = 0;
	// Регистрируем управляющую функцию сервиса
	ssHandle = RegisterServiceCtrlHandlerA(MyServiceName, ServiceControl);
	if (!ssHandle)
	{
		addLogMessage("Error RegisterServiceCtrlHandler");
		return;
	}
	// Устанавливаем состояние сервиса
	// Сервис работает как отдельный процесс
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	// Код ошибки при инициализации и завершения сервиса не используется
	ss.dwServiceSpecificExitCode = 0;
	// Начинаем запуск сервиса.
	// Прежде всего устанавливаем состояние ожидания запуска сервиса
	ReportStatus(SERVICE_START_PENDING, NO_ERROR, 30000);
	addLogMessage("Service starting...");
	// Вызываем функцию, которая выполняет все необходимые инициализирующие действия
	res = ServiceStart();
	if (res < 0)
	{
		sprintf_s(buf, "Error init server %d", res);
		addLogMessage(buf);
		ServiceControl(SERVICE_CONTROL_STOP);
		return;
	}
	// После завершения инициализации устанавливаем состояние работающего сервиса
	ReportStatus(SERVICE_RUNNING, NOERROR, 0);
	addLogMessage("Service started!");
	// основное тело службы
	if (Server() > 0)
	{
		addLogMessage("Server MF started!");
	}
	else
	{
		sprintf_s(buf, "Error starting server %d", res);
		addLogMessage(buf);
		ServiceControl(SERVICE_CONTROL_STOP);
	}
	return;
}
// -----------------------------------------------------
// Функция ServiceControl
// Точка входа функции обработки команд
// -----------------------------------------------------
void WINAPI ServiceControl(DWORD dwControlCode)
{
	// Анализируем код команды и выполняем эту команду
	switch (dwControlCode)
	{
		// Команда остановки сервиса
	case SERVICE_CONTROL_STOP:
	{
		// Устанавливаем состояние ожидания остановки
		ss.dwCurrentState = SERVICE_STOP_PENDING;
		ReportStatus(ss.dwCurrentState, NOERROR, 0);
		addLogMessage("Service stopping...");
		// Выполняем остановку сервиса, вызывая функцию, которая выполняет все необходимые для этого действия
		ServiceStop();
		// Отмечаем состояние как остановленный сервис
		ReportStatus(SERVICE_STOPPED, NOERROR, 0);
		addLogMessage("Service stopped!");
		break;
	}
	// Определение текущего состояния сервиса
	case SERVICE_CONTROL_INTERROGATE:
	{
		// Возвращаем текущее состояние сервиса
		ReportStatus(ss.dwCurrentState, NOERROR, 0);
		break;
	}
	// В ответ на другие команды просто возвращаем текущее состояние сервиса
	default:
	{
		ReportStatus(ss.dwCurrentState, NOERROR, 0);
		break;
	}
	}
}
// -----------------------------------------------------
// Функция ReportStatus
// Посылка состояния сервиса системе управления сервисами
// -----------------------------------------------------
void ReportStatus(DWORD dwCurrentState,
	DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
	// Счетчик шагов длительных операций
	static DWORD dwCheckPoint = 1;
	// Если сервис не находится в процессе запуска, его можно остановить
	if (dwCurrentState == SERVICE_START_PENDING)
		ss.dwControlsAccepted = 0;
	else
		ss.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	// Сохраняем состояние, переданное через параметры функции
	ss.dwCurrentState = dwCurrentState;
	ss.dwWin32ExitCode = dwWin32ExitCode;
	ss.dwWaitHint = dwWaitHint;
	// Если сервис не работает и не остановлен, увеличиваем значение счетчика шагов длительных операций
	if ((dwCurrentState == SERVICE_RUNNING) ||
		(dwCurrentState == SERVICE_STOPPED))
		ss.dwCheckPoint = 0;
	else
		ss.dwCheckPoint = dwCheckPoint++;
	// Вызываем функцию установки состояния
	SetServiceStatus(ssHandle, &ss);
}