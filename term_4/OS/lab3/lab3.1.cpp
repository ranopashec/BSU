#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>

CRITICAL_SECTION cs;
HANDLE hStartEvent;           // Событие (ручной сброс)
HANDLE hWorkDoneEvent;        // Событие (автоматический сброс)
HANDLE hSemaphore;            // Бинарный семафор

std::vector<float> numbers;
std::vector<float> resultNumbers;
float X;
int sleepInterval;
int countGreaterThanX = 0;

// Поток work
DWORD WINAPI WorkThread(LPVOID) {
    WaitForSingleObject(hStartEvent, INFINITE);

    std::vector<float> left, right;
    for (float num : numbers) {
        if (num < std::abs((int)X))
            left.push_back(num);
        else
            right.push_back(num);
        Sleep(sleepInterval);
    }

    resultNumbers.clear();
    resultNumbers.insert(resultNumbers.end(), left.begin(), left.end());
    resultNumbers.insert(resultNumbers.end(), right.begin(), right.end());

    EnterCriticalSection(&cs);
    std::cout << "Вход в кр. секцию потоком work\n";
    std::cout << "Поток work завершил обработку массива.\n";
    LeaveCriticalSection(&cs);
    std::cout << "Выход из кр. секции потоком work\n";

    SetEvent(hWorkDoneEvent); // Сигнал main
    return 0;
}

// Поток CountElement
DWORD WINAPI CountThread(LPVOID) {
    WaitForSingleObject(hStartEvent, INFINITE);

    int count = 0;
    for (float num : numbers) {
        if (num > X)
            count++;
    }

    countGreaterThanX = count;
    std::cout << "Поток CountElement подсчитал значения.\n";
    ReleaseSemaphore(hSemaphore, 1, NULL); // Сигнал main
    return 0;
}

int main() {
    // Устанавливаем кодировку консоли на 1251 (русская Windows)
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    InitializeCriticalSection(&cs);
    hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);        // Ручной сброс
    hWorkDoneEvent = CreateEvent(NULL, FALSE, FALSE, NULL);    // Автосброс
    hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);             // Бинарный семафор

    int size;
    std::cout << "Введите размер массива: ";
    std::cin >> size;

    numbers.resize(size);
    std::cout << "Введите элементы массива:\n";
    for (int i = 0; i < size; ++i)
        std::cin >> numbers[i];

    std::cout << "Введите задержку (мс) между элементами: ";
    std::cin >> sleepInterval;

    std::cout << "Введите значение X: ";
    std::cin >> X;

    HANDLE hThread1 = CreateThread(NULL, 0, WorkThread, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, CountThread, NULL, 0, NULL);

    SetEvent(hStartEvent); // Старт обоим потокам

    // Ждём завершения потока work через событие
    WaitForSingleObject(hWorkDoneEvent, INFINITE);

    EnterCriticalSection(&cs);
    std::cout << "\nВход в кр. секцию потоком main\n";
    std::cout << "Результат потока work: ";
    for (float val : resultNumbers)
        std::cout << val << " ";
    std::cout << "\nВыход из кр. секции потоком main\n";
    LeaveCriticalSection(&cs);

    // Ждём завершения CountElement через бинарный семафор
    WaitForSingleObject(hSemaphore, INFINITE);
    std::cout << "Результат потока CountElement: ";
    std::cout << countGreaterThanX << " элементов больше X\n";

    // Завершаем
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hStartEvent);
    CloseHandle(hWorkDoneEvent);
    CloseHandle(hSemaphore);
    DeleteCriticalSection(&cs);

    return 0;
}
