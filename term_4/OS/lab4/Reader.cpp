#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // ��������� ���������� �� Administrator
    if (argc < 3) {
        std::cerr << "������������ ����������: Reader <P> <��� �����>\n";
        return 1;
    }

    int P = atoi(argv[1]);
    std::string filename = argv[2];

    // 1. �������� ���������
    HANDLE binarySem = OpenSemaphoreA(SYNCHRONIZE, FALSE, "Global\\BinarySemaphore");
    HANDLE finishSem = OpenSemaphoreA(SYNCHRONIZE, FALSE, "Global\\FinishSemaphore");
    if (!binarySem || !finishSem) {
        std::cerr << "������ �������� ���������\n";
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr)) + GetCurrentProcessId());

    // 2. ��������� � ������ P float �������� � ����
    for (int i = 0; i < P; ++i) {
        DWORD waitResult = WaitForSingleObject(binarySem, 0); // �������� ��� ��������

        if (waitResult == WAIT_OBJECT_0) {
            std::cout << "Activ (������ ��������)\n";

            std::ofstream fout(filename, std::ios::binary | std::ios::app);
            if (fout.is_open()) {
                float value = static_cast<float>(rand()) / RAND_MAX * 100;
                fout.write(reinterpret_cast<const char*>(&value), sizeof(float));
                fout.close();
                std::cout << "�������� ��������: " << value << "\n";
            }

            ReleaseSemaphore(binarySem, 1, NULL);
            std::cout << "Inactiv (������������ ��������)\n";
        }
        else {
            std::cout << "Inactiv (�� ������� ��������� �������)\n";
        }

        Sleep(100); // ����� �������� �� ������ ���� �����
    }

    // 3. �������� ������� ���������� �� Administrator
    WaitForSingleObject(finishSem, INFINITE);
    std::cout << "������� ������ ����������. �������� ������.\n";

    return 0;
}
