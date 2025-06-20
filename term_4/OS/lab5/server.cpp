#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>      // Для strncpy
#include <unistd.h>     // Для fork, exec, read, write, close
#include <fcntl.h>      // Для O_RDONLY, O_WRONLY, O_RDWR, O_CREAT
#include <sys/stat.h>   // Для mkfifo, S_IRUSR, S_IWUSR, S_MODE
#include <sys/wait.h>   // Для waitpid
#include <semaphore.h>  // Для POSIX семафоров
#include <pthread.h>    // Для POSIX потоков
#include <cerrno>       // Для errno
#include <cstdio>       // Для remove (удаление FIFO)
#include <limits>       // Для std::numeric_limits

#include "employee.h" // Включаем общий заголовочный файл

// Глобальные переменные
std::vector<employee> employees_data;
int number_of_employees;
int number_of_clients;
std::string file_name;

// Семафоры для каждого сотрудника - теперь будут указатели на именованные семафоры
std::vector<sem_t*> employee_semaphores;
std::string sem_name_prefix = "/employee_sem_"; // Prefix for named semaphores

// Функция обработки ошибок
void posix_error(const std::string& msg)
{
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

// Функция для записи данных сотрудников в файл
void write_employees_to_file()
{
    std::ofstream file(file_name, std::ios::out);
    if (!file.is_open())
    {
        posix_error("Error opening file for writing");
    }
    for (const auto& emp : employees_data)
    {
        file << emp.num << " " << emp.name << " " << emp.hours << "\n";
    }
    file.close();
}

// Функция для инициализации файла с данными сотрудников
void init_employee_file()
{
    std::cout << "Input file name: ";
    std::cin >> file_name;
    std::cout << "Input number of employees: ";
    std::cin >> number_of_employees;

    employees_data.resize(number_of_employees); // Изменяем размер вектора под количество сотрудников

    for (int i = 0; i < number_of_employees; ++i)
    {
        std::cout << "Input employee's data (id, name, hours): ";
        std::cin >> employees_data[i].num >> employees_data[i].name >> employees_data[i].hours;
    }

    write_employees_to_file();
}

// Функция для вывода содержимого файла
void display_file_content()
{
    std::ifstream file(file_name, std::ios::in);
    if (!file.is_open())
    {
        posix_error("Error opening file for reading");
    }
    std::cout << "\n\n - - - File content - - - \n\n";
    int id_read;
    char name_read[10];
    double hours_read;
    for (int i = 0; i < number_of_employees; ++i)
    {
        if (!(file >> id_read >> name_read >> hours_read))
        {
            std::cerr << "Error reading employee data from file.\n";
            break;
        }
        std::cout << "\nEmployee #" << (i + 1) << "\nID: " << id_read << "\nName: " << name_read << "\nHours: " << hours_read << "\n";
    }
    std::cout << "\n\n - - - File content - - - \n\n";
    file.close();
}

// Структура для передачи данных в поток
struct ThreadArgs {
    int pipe_fd; // Файловый дескриптор FIFO для этого клиента
};

// Функция, выполняемая каждым потоком сервера для обработки клиента
void* handle_client_operations(void* arg)
{
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    int pipe_fd = args->pipe_fd;
    delete args; // Освобождаем память, выделенную для аргументов

    int message;
    ssize_t bytes_read;
    ssize_t bytes_written;

    while (true)
    {
        // Читаем сообщение от клиента
        bytes_read = read(pipe_fd, &message, sizeof(message));
        if (bytes_read == -1)
        {
            if (errno == EBADF) // Неверный файловый дескриптор, канал мог быть закрыт клиентом
            {
                std::cerr << "Pipe closed by client.\n";
                break;
            }
            posix_error("Error reading from pipe");
        }
        else if (bytes_read == 0) // Конец файла (клиент закрыл канал)
        {
            std::cerr << "Client disconnected.\n";
            break;
        }
        else
        {
            int ID = message / 10;
            int option = message % 10;

            if (ID < 1 || ID > number_of_employees) {
                std::cerr << "Invalid employee ID received: " << ID << std::endl;
                // Может потребоваться отправить сообщение об ошибке обратно клиенту
                continue;
            }

            switch (option)
            {
                case 1: // Изменить запись
                {
                    // Захватываем семафор для данного сотрудника перед модификацией
                    sem_wait(employee_semaphores[ID - 1]);

                    employee emp_to_push;
                    emp_to_push.num = employees_data[ID - 1].num;
                    emp_to_push.hours = employees_data[ID - 1].hours;
                    strncpy(emp_to_push.name, employees_data[ID - 1].name, sizeof(emp_to_push.name) - 1);
                    emp_to_push.name[sizeof(emp_to_push.name) - 1] = '\0';

                    // Отправляем текущие данные сотрудника клиенту
                    bytes_written = write(pipe_fd, &emp_to_push, sizeof(employee));
                    if (bytes_written == -1) posix_error("Error writing employee to pipe");

                    // Читаем обновлённые данные сотрудника от клиента
                    bytes_read = read(pipe_fd, &emp_to_push, sizeof(employee));
                    if (bytes_read == -1) posix_error("Error reading updated employee from pipe");
                    else if (bytes_read == 0) {
                        std::cerr << "Client disconnected during update.\n";
                        sem_post(employee_semaphores[ID - 1]); // Освобождаем семафор
                        break;
                    }

                    // Обновляем данные на сервере
                    employees_data[ID - 1].hours = emp_to_push.hours;
                    strncpy(employees_data[ID - 1].name, emp_to_push.name, sizeof(employees_data[ID - 1].name) - 1);
                    employees_data[ID - 1].name[sizeof(employees_data[ID - 1].name) - 1] = '\0';
                    
                    write_employees_to_file(); // Обновляем файл после изменения данных

                    int msg_confirm;
                    bytes_read = read(pipe_fd, &msg_confirm, sizeof(msg_confirm)); // Ожидаем подтверждения
                    if (bytes_read == -1) posix_error("Error reading confirmation from pipe");

                    sem_post(employee_semaphores[ID - 1]); // Освобождаем семафор
                    break;
                }
                case 2: // Прочитать запись
                {
                    sem_wait(employee_semaphores[ID - 1]); // Захватываем семафор для чтения
                    
                    employee emp_to_push;
                    emp_to_push.num = employees_data[ID - 1].num;
                    emp_to_push.hours = employees_data[ID - 1].hours;
                    strncpy(emp_to_push.name, employees_data[ID - 1].name, sizeof(emp_to_push.name) - 1);
                    emp_to_push.name[sizeof(emp_to_push.name) - 1] = '\0';

                    bytes_written = write(pipe_fd, &emp_to_push, sizeof(employee));
                    if (bytes_written == -1) posix_error("Error writing employee to pipe");

                    int msg_confirm;
                    bytes_read = read(pipe_fd, &msg_confirm, sizeof(msg_confirm)); // Ожидаем подтверждения
                    if (bytes_read == -1) posix_error("Error reading confirmation from pipe");

                    sem_post(employee_semaphores[ID - 1]); // Освобождаем семафор
                    break;
                }
                default:
                    std::cerr << "Unknown option: " << option << std::endl;
                    break;
            }
        }
    }

    close(pipe_fd); // Закрываем файловый дескриптор FIFO
    pthread_exit(NULL); // Завершаем поток
}

// Функция для инициализации клиентов (создание процессов)
void init_clients()
{
    std::cout << "Input number of clients: ";
    std::cin >> number_of_clients;

    // Инициализируем семафоры для каждого сотрудника
    employee_semaphores.resize(number_of_employees); // Resize vector for sem_t pointers
    
    for (int i = 0; i < number_of_employees; ++i)
    {
        std::string current_sem_name = sem_name_prefix + std::to_string(i);
        // sem_open: name, flags (O_CREAT for creation, O_EXCL for exclusive creation), mode (permissions), initial value
        employee_semaphores[i] = sem_open(current_sem_name.c_str(), O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
        if (employee_semaphores[i] == SEM_FAILED)
        {
            if (errno == EEXIST) { // If semaphore already exists, try to open it without O_EXCL
                employee_semaphores[i] = sem_open(current_sem_name.c_str(), 0);
                if (employee_semaphores[i] == SEM_FAILED) {
                    posix_error("Error opening existing semaphore");
                }
            } else {
                posix_error("Error initializing semaphore");
            }
        }
    }

    std::vector<pid_t> client_pids(number_of_clients);
    std::vector<pthread_t> server_threads(number_of_clients);
    std::string fifo_name_prefix = "/tmp/server_fifo_"; // Именованный канал для каждого клиента

    for (int i = 0; i < number_of_clients; ++i)
    {
        std::string current_fifo_name = fifo_name_prefix + std::to_string(i);

        // Создаем именованный канал (FIFO)
        if (mkfifo(current_fifo_name.c_str(), S_IRUSR | S_IWUSR) == -1 && errno != EEXIST)
        {
            posix_error("Error creating FIFO");
        }

        pid_t pid = fork();

        if (pid == -1)
        {
            posix_error("Error forking process");
        }
        else if (pid == 0) // Дочерний процесс (Клиент)
        {
            execlp("./Client", "Client", current_fifo_name.c_str(), NULL);
            posix_error("Error executing client process");
        }
        else // Родительский процесс (Сервер)
        {
            client_pids[i] = pid; // Сохраняем PID клиента

            int pipe_fd = open(current_fifo_name.c_str(), O_RDWR);
            if (pipe_fd == -1)
            {
                posix_error("Error opening FIFO for server");
            }

            ThreadArgs* args = new ThreadArgs(); // Выделяем память для аргументов потока
            args->pipe_fd = pipe_fd;

            if (pthread_create(&server_threads[i], NULL, handle_client_operations, static_cast<void*>(args)) != 0)
            {
                posix_error("Error creating thread");
            }
        }
    }

    // Ждем завершения всех клиентских процессов
    for (int i = 0; i < number_of_clients; ++i)
    {
        int status;
        waitpid(client_pids[i], &status, 0);
        std::cout << "Client " << client_pids[i] << " exited with status " << status << std::endl;
        
        // Ждем завершения соответствующего потока сервера
        pthread_join(server_threads[i], NULL);

        // Удаляем именованный канал (FIFO) после завершения работы
        std::string current_fifo_name = fifo_name_prefix + std::to_string(i);
        if (remove(current_fifo_name.c_str()) == -1)
        {
            perror(("Error removing FIFO: " + current_fifo_name).c_str());
        }
    }

    // Уничтожаем семафоры (закрываем и удаляем)
    for (int i = 0; i < number_of_employees; ++i)
    {
        sem_close(employee_semaphores[i]); // Close the semaphore
        std::string current_sem_name = sem_name_prefix + std::to_string(i);
        sem_unlink(current_sem_name.c_str()); // Unlink (delete) the semaphore from the system
    }
}

int main()
{
    init_employee_file();
    display_file_content();
    init_clients();
    display_file_content(); // Показываем обновленное содержимое файла после работы клиентов

    return 0;
}