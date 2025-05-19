#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Получение аргументов от Administrator
    if (argc < 3) {
        std::cerr << "Недостаточно аргументов: Reader <P> <имя файла>\n";
        return 1;
    }

    int P = atoi(argv[1]);
    std::string filename = argv[2];

    // 1. Открытие семафоров
    HANDLE binarySem = OpenSemaphoreA(SYNCHRONIZE, FALSE, "Global\\BinarySemaphore");
    HANDLE finishSem = OpenSemaphoreA(SYNCHRONIZE, FALSE, "Global\\FinishSemaphore");
    if (!binarySem || !finishSem) {
        std::cerr << "Ошибка открытия семафоров\n";
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr)) + GetCurrentProcessId());

    // 2. Генерация и запись P float значений в файл
    for (int i = 0; i < P; ++i) {
        DWORD waitResult = WaitForSingleObject(binarySem, 0); // ожидание без таймаута

        if (waitResult == WAIT_OBJECT_0) {
            std::cout << "Activ (захват семафора)\n";

            std::ofstream fout(filename, std::ios::binary | std::ios::app);
            if (fout.is_open()) {
                float value = static_cast<float>(rand()) / RAND_MAX * 100;
                fout.write(reinterpret_cast<const char*>(&value), sizeof(float));
                fout.close();
                std::cout << "Записано значение: " << value << "\n";
            }

            ReleaseSemaphore(binarySem, 1, NULL);
            std::cout << "Inactiv (освобождение семафора)\n";
        }
        else {
            std::cout << "Inactiv (не удалось захватить семафор)\n";
        }

        Sleep(100); // чтобы процессы не мешали друг другу
    }

    // 3. Ожидание сигнала завершения от Administrator
    WaitForSingleObject(finishSem, INFINITE);
    std::cout << "Получен сигнал завершения. Завершаю работу.\n";

    return 0;
}
