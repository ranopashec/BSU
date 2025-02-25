package org.example.gui;

import org.example.warehouse.ProductDatabase;
import org.example.warehouse.ProductRecord;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.io.IOException;
import java.util.Comparator;
import java.util.List;

public class MainFrame extends JFrame {
    private final ProductDatabase database;
    private final JTable table;
    private final DefaultTableModel tableModel;
    private final JLabel statusBar;

    public MainFrame(ProductDatabase database) throws IOException {
        super("Warehouse Management");
        this.database = database;

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(1000, 600);
        setLayout(new BorderLayout());

        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("Команды");
        JMenuItem fillDataItem = new JMenuItem("Заполнить тестовыми данными");
        JMenuItem displayAllItem = new JMenuItem("Вывести все объекты");
        JMenuItem searchWarehouseItem = new JMenuItem("Поиск по номеру склада");
        JMenuItem searchCodeItem = new JMenuItem("Поиск по коду товара");
        JMenuItem searchDateItem = new JMenuItem("Поиск по дате поступления");
        JMenuItem searchStorageItem = new JMenuItem("Поиск по сроку хранения");
        JMenuItem deleteItem = new JMenuItem("Удалить по коду товара");
        JMenuItem sortItem = new JMenuItem("Сортировать по коду товара");
        JMenuItem rangeSearchItem = new JMenuItem("Поиск по диапазону кода товара");
        JMenuItem exitItem = new JMenuItem("Выход");

        menu.add(fillDataItem);
        menu.add(displayAllItem);
        menu.addSeparator();
        menu.add(searchWarehouseItem);
        menu.add(searchCodeItem);
        menu.add(searchDateItem);
        menu.add(searchStorageItem);
        menu.addSeparator();
        menu.add(deleteItem);
        menu.add(sortItem);
        menu.add(rangeSearchItem);
        menu.addSeparator();
        menu.add(exitItem);
        menuBar.add(menu);
        setJMenuBar(menuBar);

        fillDataItem.addActionListener(e -> fillTestData());
        displayAllItem.addActionListener(e -> displayAll());
        searchWarehouseItem.addActionListener(e -> openSearchWindow("warehouse"));
        searchCodeItem.addActionListener(e -> openSearchWindow("code"));
        searchDateItem.addActionListener(e -> openSearchWindow("date"));
        searchStorageItem.addActionListener(e -> openSearchWindow("storage"));
        deleteItem.addActionListener(e -> openDeleteWindow());
        sortItem.addActionListener(e -> sortByProductCode());
        rangeSearchItem.addActionListener(e -> openSearchWindow("range"));
        exitItem.addActionListener(e -> System.exit(0));

        tableModel = new DefaultTableModel(new Object[]{"Склад", "Код товара", "Название", "Дата", "Срок хранения", "Количество", "Цена"}, 0);
        table = new JTable(tableModel);

        JScrollPane scrollPane = new JScrollPane(table);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);

        statusBar = new JLabel("Готово");
        statusBar.setBorder(BorderFactory.createEtchedBorder());

        add(menuBar, BorderLayout.NORTH);
        add(statusBar, BorderLayout.SOUTH);
        add(scrollPane, BorderLayout.CENTER);

        displayAll();
    }

    private void fillTestData() {
        try {
            database.addRecord(new ProductRecord(1, 101, "Молоток", ProductRecord.parseDate("2023-12-01"), 365, 50, 199.99));
            database.addRecord(new ProductRecord(2, 102, "Отвертка", ProductRecord.parseDate("2023-12-02"), 180, 30, 99.99));
            database.addRecord(new ProductRecord(3, 103, "Гаечный ключ", ProductRecord.parseDate("2023-12-03"), 180, 20, 149.99));
            statusBar.setText("Тестовые данные успешно добавлены.");
            displayAll();
        } catch (Exception e) {
            showError("Ошибка добавления тестовых данных: " + e.getMessage());
        }
    }

    public void displayAll() {
        try {
            tableModel.setRowCount(0);
            List<ProductRecord> records = database.getAllRecords();
            for (ProductRecord record : records) {
                if (record.getProductCode() != -1) {
                    tableModel.addRow(new Object[]{
                            record.getWarehouseNumber(),
                            record.getProductCode(),
                            record.getProductName(),
                            record.getEntryDateWithoutTime(),
                            record.getStoragePeriodDays(),
                            record.getQuantity(),
                            record.getUnitPrice()
                    });
                }
            }
            statusBar.setText("Выведены все объекты.");
        } catch (IOException e) {
            showError("Ошибка вывода данных: " + e.getMessage());
        }
    }

    private void openSearchWindow(String type) {
        new SearchDialog(this, type, database, tableModel, statusBar);
    }

    private void openDeleteWindow() {
        new DeleteDialog(this, database);
    }

    private void sortByProductCode() {
        try {
            String[] options = {"По возрастанию", "По убыванию"};
            int choice = JOptionPane.showOptionDialog(
                    this,
                    "Выберите порядок сортировки:",
                    "Сортировка",
                    JOptionPane.DEFAULT_OPTION,
                    JOptionPane.QUESTION_MESSAGE,
                    null,
                    options,
                    options[0]
            );

            if (choice == JOptionPane.CLOSED_OPTION) return;

            boolean ascending = (choice == 0);

            tableModel.setRowCount(0);
            List<ProductRecord> records = database.getAllRecords();

            records.sort(ascending
                    ? Comparator.comparingInt(ProductRecord::getProductCode)
                    : Comparator.comparingInt(ProductRecord::getProductCode).reversed()
            );

            for (ProductRecord record : records) {
                if (record.getProductCode() != -1) {
                    tableModel.addRow(new Object[]{
                            record.getWarehouseNumber(),
                            record.getProductCode(),
                            record.getProductName(),
                            record.getEntryDateWithoutTime(),
                            record.getStoragePeriodDays(),
                            record.getQuantity(),
                            record.getUnitPrice()
                    });
                }
            }
            statusBar.setText(ascending ? "Данные отсортированы по возрастанию." : "Данные отсортированы по убыванию.");
        } catch (IOException e) {
            showError("Ошибка сортировки данных: " + e.getMessage());
        }
    }


    private void showError(String message) {
        JOptionPane.showMessageDialog(this, message, "Ошибка", JOptionPane.ERROR_MESSAGE);
        statusBar.setText(message);
    }
}