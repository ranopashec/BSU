#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h> // Для POSIX-потоков
#include <locale.h>  // Для setlocale

// Общие переменные и объекты синхронизации
pthread_mutex_t start_mutex;        // Мьютекс для hStartEvent
pthread_cond_t start_cond;          // Условная переменная для hStartEvent
bool start_flag = false;            // Флаг для hStartEvent (ручной сброс)

pthread_mutex_t work_done_mutex;    // Мьютекс для hWorkDoneEvent (критическая секция для resultNumbers)
pthread_cond_t work_done_cond;      // Условная переменная для hWorkDoneEvent

pthread_mutex_t count_done_mutex;   // Мьютекс для сигнализации от CountThread к main
pthread_cond_t count_done_cond;     // Условная переменная для сигнализации от CountThread к main
bool count_done_flag = false;       // Флаг для сигнализации от CountThread

std::vector<float> numbers;
std::vector<float> resultNumbers;
float X;
int sleepInterval;
int countGreaterThanX = 0;

// Функция для имитации Sleep в миллисекундах
void msleep(int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

// Поток work
void* WorkThread(void*) {
    // Ждем сигнала от main о начале работы (использовать событие с ручным сбросом)
    pthread_mutex_lock(&start_mutex);
    while (!start_flag) {
        pthread_cond_wait(&start_cond, &start_mutex);
    }
    pthread_mutex_unlock(&start_mutex);

    std::vector<float> left, right;
    for (float num : numbers) {
        if (num < std::abs(X)) { // Исправлено на std::abs(X) для float
            left.push_back(num);
        } else {
            right.push_back(num);
        }
        msleep(sleepInterval); // Отдыхаем
    }

    // Защита доступа к resultNumbers мьютексом
   //  pthread_mutex_lock(&work_done_mutex);
    std::cout << "Вход в кр. секцию потоком work\n";
    resultNumbers.clear();
    resultNumbers.insert(resultNumbers.end(), left.begin(), left.end());
    resultNumbers.insert(resultNumbers.end(), right.begin(), right.end());
    std::cout << "Поток work завершил обработку массива.\n";
    std::cout << "Выход из кр. секции потоком work\n";
     pthread_mutex_unlock(&work_done_mutex);

    // Известить поток main о выводе итогового массива (использовать условную переменную)
    pthread_mutex_lock(&work_done_mutex);
    pthread_cond_signal(&work_done_cond); // Сигнал main
    pthread_mutex_unlock(&work_done_mutex);

    return NULL;
}

// Поток CountElement
void* CountThread(void*) {
    // Ждем сигнала от main о начале работы (использовать событие с ручным сбросом)
    pthread_mutex_lock(&start_mutex);
    while (!start_flag) {
        pthread_cond_wait(&start_cond, &start_mutex);
    }
    pthread_mutex_unlock(&start_mutex);

    int count = 0;
    for (float num : numbers) {
        if (num > X) {
            count++;
        }
    }

    countGreaterThanX = count;
    std::cout << "Поток CountElement подсчитал значения.\n";

    // Известить поток main о выводе результата (использовать мьютекс и условную переменную)
    pthread_mutex_lock(&count_done_mutex);
    count_done_flag = true;
    pthread_cond_signal(&count_done_cond); // Сигнал main
    pthread_mutex_unlock(&count_done_mutex);

    return NULL;
}

int main() {
    // Установка локали для корректного вывода русских символов
    // В зависимости от вашей системы, возможно, потребуется "ru_RU.UTF-8"
    setlocale(LC_ALL, ""); 

    // Инициализация мьютексов и условных переменных
    pthread_mutex_init(&start_mutex, NULL);
    pthread_cond_init(&start_cond, NULL);

    pthread_mutex_init(&work_done_mutex, NULL);
    pthread_cond_init(&work_done_cond, NULL);

    pthread_mutex_init(&count_done_mutex, NULL);
    pthread_cond_init(&count_done_cond, NULL);

    int size;
    std::cout << "Введите размер массива: ";
    std::cin >> size;

    numbers.resize(size);
    std::cout << "Введите элементы массива:\n";
    for (int i = 0; i < size; ++i) {
        std::cin >> numbers[i];
    }

    std::cout << "Введите задержку (мс) между элементами: ";
    std::cin >> sleepInterval;

    std::cout << "Введите значение X: ";
    std::cin >> X;

    pthread_t hThread1, hThread2;
    pthread_create(&hThread1, NULL, WorkThread, NULL);
    pthread_create(&hThread2, NULL, CountThread, NULL);

    // Известить потоки work и CountElement о начале работы (использовать событие с ручным сбросом)
    pthread_mutex_lock(&start_mutex);
    start_flag = true;
    pthread_cond_broadcast(&start_cond); // Сигнал обоим потокам
    pthread_mutex_unlock(&start_mutex);

    // Ждем завершения потока work через условную переменную
    pthread_mutex_lock(&work_done_mutex);
    pthread_cond_wait(&work_done_cond, &work_done_mutex);
    // Теперь можем безопасно вывести результат work
    std::cout << "\nВход в кр. секцию потоком main\n";
    std::cout << "Результат потока work: ";
    for (float val : resultNumbers) {
        std::cout << val << " ";
    }
    std::cout << "\nВыход из кр. секции потоком main\n";
    pthread_mutex_unlock(&work_done_mutex);

    // Ждем сигнала потока CountElement через мьютекс и условную переменную
    pthread_mutex_lock(&count_done_mutex);
    while (!count_done_flag) {
        pthread_cond_wait(&count_done_cond, &count_done_mutex);
    }
    std::cout << "Результат потока CountElement: ";
    std::cout << countGreaterThanX << " элементов больше X\n";
    pthread_mutex_unlock(&count_done_mutex);


    // Ожидаем завершения потоков
    pthread_join(hThread1, NULL);
    pthread_join(hThread2, NULL);

    // Очистка ресурсов
    pthread_mutex_destroy(&start_mutex);
    pthread_cond_destroy(&start_cond);

    pthread_mutex_destroy(&work_done_mutex);
    pthread_cond_destroy(&work_done_cond);

    pthread_mutex_destroy(&count_done_mutex);
    pthread_cond_destroy(&count_done_cond);

    return 0;
}