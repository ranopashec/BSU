package org.example;
import models.ProductDatabase;
import models.ProductRecord;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Scanner;
import java.util.List;

public class Main
{
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
    private static final Scanner scanner = new Scanner(System.in);

    public static void main(String[] args)
    {
        ProductDatabase database;

        try
        {
            database = new ProductDatabase("products.dat");
        } catch (IOException e)
        {
            System.err.println("Ошибка при открытии базы данных: " + e.getMessage());
            return;
        }

        while (true)
        {
            System.out.println("\nВыберите действие:");
            System.out.println("1 - Заполнение тестовыми данными");
            System.out.println("2 - Вывод всех объектов");
            System.out.println("3 - Поиск по номеру склада");
            System.out.println("4 - Поиск по коду товара");
            System.out.println("5 - Поиск по дате поступления");
            System.out.println("6 - Поиск по сроку хранения");
            System.out.println("0 - Выход");
            System.out.print("Введите номер действия: ");

            int choice;
            try
            {
                choice = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e)
            {
                System.out.println("Неверный ввод. Пожалуйста, введите число.");
                continue;
            }

            switch (choice)
            {
                case 1 -> addTestData(database);
                case 2 -> displayAllRecords(database);
                case 3 -> searchByWarehouse(database);
                case 4 -> searchByProductCode(database);
                case 5 -> searchByEntryDate(database);
                case 6 -> searchByStoragePeriod(database);
                case 0 ->
                {
                    closeDatabase(database);
                    System.out.println("Выход из программы.");
                    return;
                }
                default -> System.out.println("Неверный выбор. Пожалуйста, выберите корректное действие.");
            }
        }
    }

    private static void addTestData(ProductDatabase database)
    {
        try
        {
            database.addRecord(new ProductRecord(1, 1001, "Молоток", new Date(), 365, 50, 199.99));
            database.addRecord(new ProductRecord(2, 1002, "Отвертка", new Date(), 180, 150, 99.99));
            System.out.println("Добавлены тестовые данные.");
        } catch (IOException e)
        {
            System.out.println("Ошибка при добавлении данных: " + e.getMessage());
        }
    }

    private static void displayAllRecords(ProductDatabase database) {
        try
        {
            database.displayAllRecords();
        } catch (IOException e)
        {
            System.out.println("Ошибка при выводе данных: " + e.getMessage());
        }
    }

    private static void searchByWarehouse(ProductDatabase database)
    {
        System.out.print("Введите номер склада: ");
        int warehouseNumber = Integer.parseInt(scanner.nextLine());
        try
        {
            List<ProductRecord> results = database.searchByWarehouse(warehouseNumber);
            results.forEach(System.out::println);
        } catch (IOException e)
        {
            System.out.println("Ошибка при поиске по номеру склада: " + e.getMessage());
        }
    }

    private static void searchByProductCode(ProductDatabase database)
    {
        System.out.print("Введите код товара: ");
        int productCode = Integer.parseInt(scanner.nextLine());
        try {
            ProductRecord record = database.searchByProductCode(productCode);
            if (record != null)
            {
                System.out.println(record);
            } else
            {
                System.out.println("Товар с таким кодом не найден.");
            }
        } catch (IOException e)
        {
            System.out.println("Ошибка при поиске по коду товара: " + e.getMessage());
        }
    }

    private static void searchByEntryDate(ProductDatabase database)
    {
        System.out.print("Введите дату поступления в формате 'yyyy-MM-dd': ");
        try
        {
            Date date = dateFormat.parse(scanner.nextLine());
            List<ProductRecord> results = database.searchByEntryDate(date);
            results.forEach(System.out::println);
        } catch (ParseException e)
        {
            System.out.println("Неверный формат даты.");
        } catch (IOException e)
        {
            System.out.println("Ошибка при поиске по дате поступления: " + e.getMessage());
        }
    }

    private static void searchByStoragePeriod(ProductDatabase database)
    {
        System.out.print("Введите срок хранения в днях: ");
        int days = Integer.parseInt(scanner.nextLine());
        try
        {
            List<ProductRecord> results = database.searchByStoragePeriod(days);
            results.forEach(System.out::println);
        } catch (IOException e)
        {
            System.out.println("Ошибка при поиске по сроку хранения: " + e.getMessage());
        }
    }

    private static void closeDatabase(ProductDatabase database)
    {
        try
        {
            database.close();
        } catch (IOException e)
        {
            System.out.println("Ошибка при закрытии базы данных: " + e.getMessage());
        }
    }
}
