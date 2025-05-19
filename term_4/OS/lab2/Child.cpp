#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

// Функция бинарной сортировки (по сути бинарная вставка)
void binarySort(vector<int>& arr) {
    for (int i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int left = 0;
        int right = i - 1;

        // Бинарный поиск места вставки
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid] > key) {
                right = mid - 1;
            }
            else {
                left = mid + 1;
            }
        }

        // Сдвиг элементов и вставка
        for (int j = i - 1; j >= left; --j) {
            arr[j + 1] = arr[j];
        }
        arr[left] = key;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        wcerr << L"Usage: Child.exe <size> <elements...>" << endl;
        system("pause");
        return 1;
    }

    // Парсинг аргументов
    int size = stoi(argv[1]);
    vector<int> arr;
    for (int i = 2; i < argc && arr.size() < size; ++i) {
        arr.push_back(stoi(argv[i]));
    }

    // Вывод исходного массива
    wcout << L"Original array: ";
    for (int num : arr) {
        wcout << num << L" ";
    }
    wcout << endl;

    // Сортировка
    binarySort(arr);

    // Вывод результата
    wcout << L"Sorted array (binary sort): ";
    for (int num : arr) {
        wcout << num << L" ";
    }
    wcout << endl;

    // Ожидание завершения
    system("pause");

    return 0;
}