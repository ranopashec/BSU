#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int main() {
    SetConsoleCP(1251);         // Поддержка русского ввода
    SetConsoleOutputCP(1251);   // Поддержка русского вывода

    int M, P, K;
    std::string filename;

    // 1. Запрашивает у пользователя количество Reader процессов
    std::cout << "Введите количество Reader процессов (M): ";
    std::cin >> M;

    // 2. Запрашивает количество элементов на процесс (P)
    std::cout << "Введите количество элементов на процесс (P): ";
    std::cin >> P;

    // 3. Запрашивает количество элементов, которые будет читать сам
    std::cout << "Введите количество элементов для чтения (K <= P*M): ";
    std::cin >> K;

    // 4. Запрашивает имя файла и создаёт его
    std::cout << "Введите имя файла: ";
    std::cin >> filename;
    std::ofstream fout(filename, std::ios::binary);
    if (!fout.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << filename << "\n";
        return 1;
    }
    fout.close();
    std::cout << "Файл успешно создан: " << filename << "\n";

    // 5. Создание бинарного семафора (одновременная запись только 1 Reader)
    HANDLE binarySem = CreateSemaphoreA(nullptr, 1, 1, "Global\\BinarySemaphore");
    if (binarySem == INVALID_HANDLE_VALUE) {
        std::cerr << "Ошибка создания бинарного семафора.\n";
        return 1;
    }

    // 6. Создание семафора завершения для M процессов
    HANDLE finishSem = CreateSemaphoreA(nullptr, 0, M, "Global\\FinishSemaphore");
    if (finishSem == INVALID_HANDLE_VALUE) {
        std::cerr << "Ошибка создания семафора завершения.\n";
        CloseHandle(binarySem);
        return 1;
    }

    // 7. Запуск M Reader-процессов в отдельных окнах
    std::vector<PROCESS_INFORMATION> processes;
    for (int i = 0; i < M; ++i) {
        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        std::stringstream cmd;
        cmd << "\"Reader.exe\" " << P << " " << filename;

        if (!CreateProcessA(NULL, (LPSTR)cmd.str().c_str(), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cerr << "Ошибка запуска Reader #" << i << "\n";
        }
        else {
            processes.push_back(pi);
        }
    }

    // 8. Ожидание завершения всех Reader'ов
    for (auto& pi : processes) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // 9. Чтение K элементов из файла
    std::ifstream fin(filename, std::ios::binary);
    if (fin.is_open()) {
        std::cout << "\nДанные в файле:\n";
        float val;
        int count = 0;
        while (count < K && fin.read(reinterpret_cast<char*>(&val), sizeof(float))) {
            std::cout << "[" << count << "] = " << val << "\n";
            ++count;
        }
        fin.close();
    }

    // 10. Отправка сигнала завершения процессам Reader
    for (int i = 0; i < M; ++i) {
        ReleaseSemaphore(finishSem, 1, nullptr);
    }

    // 11. Завершение
    CloseHandle(binarySem);
    CloseHandle(finishSem);

    std::cout << "\nAdministrator завершил работу.\n";
    return 0;
}
