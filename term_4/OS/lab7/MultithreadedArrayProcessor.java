import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class MultithreadedArrayProcessor {

    // Общие данные, доступ к которым будут получать и изменять несколько потоков
    private List<Float> numbers;
    private List<Float> resultNumbers;
    private float X;
    private int sleepInterval;
    private int countGreaterThanX = 0;

    // Объекты для синхронизации
    // Объект для синхронизации события старта (ручной сброс)
    private final Object startEvent = new Object();
    private volatile boolean startFlag = false; // volatile для корректной видимости изменений между потоками

    // Объект для синхронизации доступа к resultNumbers и сигнализации о завершении WorkThread
    private final Object workDoneEvent = new Object();
    private volatile boolean workFinished = false; // Флаг для WorkThread

    // Объект для синхронизации доступа к countGreaterThanX и сигнализации о завершении CountThread
    private final Object countDoneEvent = new Object();
    private volatile boolean countFinished = false; // Флаг для CountThread

    // Конструктор для инициализации
    public MultithreadedArrayProcessor() {
        this.numbers = new ArrayList<>();
        this.resultNumbers = new ArrayList<>();
    }

    // --- Поток WorkThread ---
    class WorkThread extends Thread {
        @Override
        public void run() {
            // Ждем сигнала от main о начале работы (использовать событие с ручным сбросом)
            synchronized (startEvent) {
                while (!startFlag) {
                    try {
                        startEvent.wait(); // Ожидаем, пока main не даст старт
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        return;
                    }
                }
            }

            List<Float> left = new ArrayList<>();
            List<Float> right = new ArrayList<>();

            for (float num : numbers) {
                if (num < Math.abs(X)) {
                    left.add(num);
                } else {
                    right.add(num);
                }
                try {
                    Thread.sleep(sleepInterval); // Имитация работы
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return;
                }
            }

            // Использование 'synchronized' для критической секции с resultNumbers
            synchronized (workDoneEvent) {
                System.out.println("Вход в кр. секцию потоком work");
                resultNumbers.clear();
                resultNumbers.addAll(left);
                resultNumbers.addAll(right);
                System.out.println("Поток work завершил обработку массива.");
                System.out.println("Выход из кр. секции потоком work");
                workFinished = true; // Устанавливаем флаг завершения работы
                workDoneEvent.notify(); // Сигнализируем main
            }
        }
    }

    // --- Поток CountThread ---
    class CountThread extends Thread {
        @Override
        public void run() {
            // Ждем сигнала от main о начале работы (использовать событие с ручным сбросом)
            synchronized (startEvent) {
                while (!startFlag) {
                    try {
                        startEvent.wait(); // Ожидаем, пока main не даст старт
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        return;
                    }
                }
            }

            int count = 0;
            for (float num : numbers) {
                if (num > X) {
                    count++;
                }
            }

            // Использование 'synchronized' для критической секции с countGreaterThanX
            synchronized (countDoneEvent) {
                countGreaterThanX = count;
                System.out.println("Поток CountElement подсчитал значения.");
                countFinished = true; // Устанавливаем флаг завершения работы
                countDoneEvent.notify(); // Сигнализируем main
            }
        }
    }

    // --- Метод main ---
    public static void main(String[] args) {
        // Установка локали для корректного ввода/вывода (обычно не обязательно для консоли, но для надежности)
        // Locale.setDefault(new Locale("ru", "RU")); // Можно попробовать, если будут проблемы с кодировкой

        MultithreadedArrayProcessor processor = new MultithreadedArrayProcessor();
        Scanner scanner = new Scanner(System.in);

        System.out.print("Введите размер массива: ");
        int size = scanner.nextInt();

        System.out.println("Введите элементы массива:");
        for (int i = 0; i < size; ++i) {
            processor.numbers.add(scanner.nextFloat());
        }

        System.out.print("Введите задержку (мс) между элементами: ");
        processor.sleepInterval = scanner.nextInt();

        System.out.print("Введите значение X: ");
        processor.X = scanner.nextFloat();

        // Создаем потоки
        WorkThread workThread = processor.new WorkThread();
        CountThread countThread = processor.new CountThread();

        // Запускаем потоки
        workThread.start();
        countThread.start();

        // Известить потоки work и CountElement о начале работы (использовать событие с ручным сбросом)
        synchronized (processor.startEvent) {
            processor.startFlag = true;
            processor.startEvent.notifyAll(); // Разбудить все ожидающие потоки
        }

        // Ждем завершения потока work через событие (условную переменную)
        synchronized (processor.workDoneEvent) {
            while (!processor.workFinished) {
                try {
                    processor.workDoneEvent.wait(); // Ожидаем сигнала от WorkThread
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
            // Теперь можем безопасно вывести результат work
            System.out.println("\nВход в кр. секцию потоком main");
            System.out.print("Результат потока work: ");
            for (float val : processor.resultNumbers) {
                System.out.print(val + " ");
            }
            System.out.println("\nВыход из кр. секции потоком main");
        }

        // Ждем завершения CountElement через событие (условную переменную)
        synchronized (processor.countDoneEvent) {
            while (!processor.countFinished) {
                try {
                    processor.countDoneEvent.wait(); // Ожидаем сигнала от CountThread
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
            System.out.print("Результат потока CountElement: ");
            System.out.println(processor.countGreaterThanX + " элементов больше X");
        }

        // Ожидаем фактического завершения потоков (хорошая практика)
        try {
            workThread.join();
            countThread.join();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        scanner.close();
    }
}