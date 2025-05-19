#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>

CRITICAL_SECTION cs;
HANDLE hStartEvent;           // ������� (������ �����)
HANDLE hWorkDoneEvent;        // ������� (�������������� �����)
HANDLE hSemaphore;            // �������� �������

std::vector<float> numbers;
std::vector<float> resultNumbers;
float X;
int sleepInterval;
int countGreaterThanX = 0;

// ����� work
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
    std::cout << "���� � ��. ������ ������� work\n";
    std::cout << "����� work �������� ��������� �������.\n";
    LeaveCriticalSection(&cs);
    std::cout << "����� �� ��. ������ ������� work\n";

    SetEvent(hWorkDoneEvent); // ������ main
    return 0;
}

// ����� CountElement
DWORD WINAPI CountThread(LPVOID) {
    WaitForSingleObject(hStartEvent, INFINITE);

    int count = 0;
    for (float num : numbers) {
        if (num > X)
            count++;
    }

    countGreaterThanX = count;
    std::cout << "����� CountElement ��������� ��������.\n";
    ReleaseSemaphore(hSemaphore, 1, NULL); // ������ main
    return 0;
}

int main() {
    // ������������� ��������� ������� �� 1251 (������� Windows)
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    InitializeCriticalSection(&cs);
    hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);        // ������ �����
    hWorkDoneEvent = CreateEvent(NULL, FALSE, FALSE, NULL);    // ���������
    hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);             // �������� �������

    int size;
    std::cout << "������� ������ �������: ";
    std::cin >> size;

    numbers.resize(size);
    std::cout << "������� �������� �������:\n";
    for (int i = 0; i < size; ++i)
        std::cin >> numbers[i];

    std::cout << "������� �������� (��) ����� ����������: ";
    std::cin >> sleepInterval;

    std::cout << "������� �������� X: ";
    std::cin >> X;

    HANDLE hThread1 = CreateThread(NULL, 0, WorkThread, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, CountThread, NULL, 0, NULL);

    SetEvent(hStartEvent); // ����� ����� �������

    // ��� ���������� ������ work ����� �������
    WaitForSingleObject(hWorkDoneEvent, INFINITE);

    EnterCriticalSection(&cs);
    std::cout << "\n���� � ��. ������ ������� main\n";
    std::cout << "��������� ������ work: ";
    for (float val : resultNumbers)
        std::cout << val << " ";
    std::cout << "\n����� �� ��. ������ ������� main\n";
    LeaveCriticalSection(&cs);

    // ��� ���������� CountElement ����� �������� �������
    WaitForSingleObject(hSemaphore, INFINITE);
    std::cout << "��������� ������ CountElement: ";
    std::cout << countGreaterThanX << " ��������� ������ X\n";

    // ���������
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
