#include <iostream>
#include <string>
#include <cstring>      // Для strncpy
#include <unistd.h>     // Для read, write, close
#include <fcntl.h>      // Для O_RDWR
#include <cerrno>       // Для errno
#include <limits>       // Для std::numeric_limits

#include "employee.h" // Включаем общий заголовочный файл

// Функция обработки ошибок
void posix_error(const std::string& msg)
{
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    // Проверяем, был ли передан аргумент (имя FIFO)
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <fifo_name>\n";
        return 1;
    }

    std::string fifo_name = argv[1]; // Получаем имя FIFO из аргументов командной строки

    // Открываем FIFO для чтения и записи
    // O_RDWR обеспечивает двустороннюю связь
    int pipe_fd = open(fifo_name.c_str(), O_RDWR);
    if (pipe_fd == -1)
    {
        posix_error("Error opening FIFO for client");
    }

    while (true)
    {
        int option = 0;
        std::cout << "Choose option:\n1. Change record\n2. Read record\n3. Exit\n";
        std::cin >> option;

        // Обработка неверного ввода (например, если пользователь ввёл не число)
        if (std::cin.fail()) {
            std::cin.clear(); // Очистить флаг ошибки
            // Игнорировать оставшуюся часть строки, включая символ новой строки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (option)
        {
            case 1: // Изменить запись
            {
                ssize_t bytes_written;
                ssize_t bytes_read;
                int ID, sendingMessage;

                std::cout << "Input employee's ID: ";
                std::cin >> ID;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Invalid ID. Please enter a number.\n";
                    break;
                }

                sendingMessage = ID * 10 + option;
                // Отправляем сообщение серверу (ID сотрудника и тип операции)
                bytes_written = write(pipe_fd, &sendingMessage, sizeof(sendingMessage));
                if (bytes_written == -1) posix_error("Error writing message to pipe");

                employee object;
                // Читаем текущие данные сотрудника от сервера
                bytes_read = read(pipe_fd, &object, sizeof(employee));
                if (bytes_read == -1) posix_error("Error reading employee from pipe");
                else if (bytes_read == 0) { // Сервер закрыл канал
                    std::cerr << "Server disconnected unexpectedly.\n";
                    close(pipe_fd);
                    return 1;
                }

                std::cout << "\nEmployee" << "\nID: " << object.num << "\nName: " << object.name << "\nHours: " << object.hours << "\n";
                std::cout << "Input new employee's info (name, hours): ";
                std::cin >> object.name >> object.hours;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Invalid input for name/hours. Please try again.\n";
                    int msg_confirm = 0; // Отправляем 0 как подтверждение, что не было обновления
                    write(pipe_fd, &msg_confirm, sizeof(msg_confirm));
                    break;
                }

                // Отправляем обновлённые данные сотрудника серверу
                bytes_written = write(pipe_fd, &object, sizeof(employee));
                if (bytes_written == -1) posix_error("Error writing updated employee to pipe");

                sendingMessage = 1; // Подтверждение операции
                bytes_written = write(pipe_fd, &sendingMessage, sizeof(sendingMessage));
                if (bytes_written == -1) posix_error("Error writing confirmation to pipe");
                break;
            }
            case 2: // Прочитать запись
            {
                ssize_t bytes_written;
                ssize_t bytes_read;
                int ID, sendingMessage;

                std::cout << "Input employee's ID: ";
                std::cin >> ID;
                 if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Invalid ID. Please enter a number.\n";
                    break;
                }

                sendingMessage = ID * 10 + option;
                // Отправляем сообщение серверу
                bytes_written = write(pipe_fd, &sendingMessage, sizeof(sendingMessage));
                if (bytes_written == -1) posix_error("Error writing message to pipe");

                employee object;
                // Читаем данные сотрудника от сервера
                bytes_read = read(pipe_fd, &object, sizeof(employee));
                if (bytes_read == -1) posix_error("Error reading employee from pipe");
                else if (bytes_read == 0) {
                    std::cerr << "Server disconnected unexpectedly.\n";
                    close(pipe_fd);
                    return 1;
                }

                std::cout << "\nID: " << object.num << "\nName: " << object.name << "\nHours: " << object.hours << "\n";

                sendingMessage = 1; // Подтверждение операции
                bytes_written = write(pipe_fd, &sendingMessage, sizeof(sendingMessage));
                if (bytes_written == -1) posix_error("Error writing confirmation to pipe");
                break;
            }
            case 3: // Выход
            {
                close(pipe_fd); // Закрываем FIFO
                return 0;
            }
            default:
            {
                std::cout << "Invalid option. Please try again.\n";
                break;
            }
        }
    }

    close(pipe_fd); // Закрываем FIFO при выходе из цикла
    return 0;
}