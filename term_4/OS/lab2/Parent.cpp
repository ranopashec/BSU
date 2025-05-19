#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

int main() {
    // Ввод данных
    int size;
    cout << "Enter array size: ";
    cin >> size;

    vector<int> arr(size);
    cout << "Enter " << size << " integer elements: ";
    for (int i = 0; i < size; ++i) {
        cin >> arr[i];
    }

    // Создание командной строки для Child процесса
    wstring cmdLine = L"Child.exe " + to_wstring(size);
    for (int num : arr) {
        cmdLine += L" " + to_wstring(num);
    }

    // Подготовка буфера для командной строки (исправление ошибки LPWSTR)
    wchar_t* cmdLineBuf = new wchar_t[cmdLine.size() + 1];
    wcscpy_s(cmdLineBuf, cmdLine.size() + 1, cmdLine.c_str());

    // Структуры для создания процесса
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    wchar_t title[] = L"Child Process - Binary Sort";
    si.lpTitle = title; // Установка нового заголовка окна
    ZeroMemory(&pi, sizeof(pi));

    // Создание Child процесса 
    if (!CreateProcess(
        NULL,                   // Имя модуля
        cmdLineBuf,             // Командная строка
        NULL,                   // Атрибуты безопасности процесса
        NULL,                   // Атрибуты безопасности потока
        FALSE,                  // Наследование дескрипторов
        0,                      // Флаги создания
        NULL,                   // Окружение
        NULL,                   // Текущий каталог
        &si,                    // STARTUPINFO
        &pi))                   // PROCESS_INFORMATION
    {
        wcerr << L"CreateProcess failed (" << GetLastError() << L")." << endl;
        delete[] cmdLineBuf;
        return 1;
    }

    cout << "Child process started. Waiting for completion..." << endl;

    // Ожидание завершения Child процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Закрытие дескрипторов
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    delete[] cmdLineBuf;

    // Запуск Count процесса (вывод чисел Фибоначчи)
    STARTUPINFO siCount;
    PROCESS_INFORMATION piCount;

    ZeroMemory(&siCount, sizeof(siCount));
    siCount.cb = sizeof(siCount);
    ZeroMemory(&piCount, sizeof(piCount));

    // Подготовка командной строки для Count
    wchar_t countCmd[] = L"Count.exe";

    if (!CreateProcess(
        NULL,
        countCmd,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE | BELOW_NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &siCount,
        &piCount))
    {
        wcerr << L"Failed to start Count process (" << GetLastError() << L")." << endl;
        return 1;
    }

    // Установка приоритета для Count процесса
    SetPriorityClass(piCount.hProcess, BELOW_NORMAL_PRIORITY_CLASS);

    cout << "Count process started with below normal priority." << endl;

    // Даем Count процессу поработать 10 секунд
    Sleep(10000);

    // Завершение Count процесса
    TerminateProcess(piCount.hProcess, 0);
    cout << "Count process terminated." << endl;

    // Закрытие дескрипторов
    CloseHandle(piCount.hProcess);
    CloseHandle(piCount.hThread);

    // Завершение Parent процесса
    ExitProcess(0);

    return 0;
}