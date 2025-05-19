#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int main() {
    SetConsoleCP(1251);         // ��������� �������� �����
    SetConsoleOutputCP(1251);   // ��������� �������� ������

    int M, P, K;
    std::string filename;

    // 1. ����������� � ������������ ���������� Reader ���������
    std::cout << "������� ���������� Reader ��������� (M): ";
    std::cin >> M;

    // 2. ����������� ���������� ��������� �� ������� (P)
    std::cout << "������� ���������� ��������� �� ������� (P): ";
    std::cin >> P;

    // 3. ����������� ���������� ���������, ������� ����� ������ ���
    std::cout << "������� ���������� ��������� ��� ������ (K <= P*M): ";
    std::cin >> K;

    // 4. ����������� ��� ����� � ������ ���
    std::cout << "������� ��� �����: ";
    std::cin >> filename;
    std::ofstream fout(filename, std::ios::binary);
    if (!fout.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << filename << "\n";
        return 1;
    }
    fout.close();
    std::cout << "���� ������� ������: " << filename << "\n";

    // 5. �������� ��������� �������� (������������� ������ ������ 1 Reader)
    HANDLE binarySem = CreateSemaphoreA(nullptr, 1, 1, "Global\\BinarySemaphore");
    if (binarySem == INVALID_HANDLE_VALUE) {
        std::cerr << "������ �������� ��������� ��������.\n";
        return 1;
    }

    // 6. �������� �������� ���������� ��� M ���������
    HANDLE finishSem = CreateSemaphoreA(nullptr, 0, M, "Global\\FinishSemaphore");
    if (finishSem == INVALID_HANDLE_VALUE) {
        std::cerr << "������ �������� �������� ����������.\n";
        CloseHandle(binarySem);
        return 1;
    }

    // 7. ������ M Reader-��������� � ��������� �����
    std::vector<PROCESS_INFORMATION> processes;
    for (int i = 0; i < M; ++i) {
        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        std::stringstream cmd;
        cmd << "\"Reader.exe\" " << P << " " << filename;

        if (!CreateProcessA(NULL, (LPSTR)cmd.str().c_str(), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cerr << "������ ������� Reader #" << i << "\n";
        }
        else {
            processes.push_back(pi);
        }
    }

    // 8. �������� ���������� ���� Reader'��
    for (auto& pi : processes) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // 9. ������ K ��������� �� �����
    std::ifstream fin(filename, std::ios::binary);
    if (fin.is_open()) {
        std::cout << "\n������ � �����:\n";
        float val;
        int count = 0;
        while (count < K && fin.read(reinterpret_cast<char*>(&val), sizeof(float))) {
            std::cout << "[" << count << "] = " << val << "\n";
            ++count;
        }
        fin.close();
    }

    // 10. �������� ������� ���������� ��������� Reader
    for (int i = 0; i < M; ++i) {
        ReleaseSemaphore(finishSem, 1, nullptr);
    }

    // 11. ����������
    CloseHandle(binarySem);
    CloseHandle(finishSem);

    std::cout << "\nAdministrator �������� ������.\n";
    return 0;
}
