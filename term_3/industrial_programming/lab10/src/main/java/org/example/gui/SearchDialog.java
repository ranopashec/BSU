package org.example.gui;

import org.example.warehouse.ProductDatabase;
import org.example.warehouse.ProductRecord;


import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.util.List;

public class SearchDialog extends JDialog {
    public SearchDialog(JFrame parent, String type, ProductDatabase database, DefaultTableModel tableModel, JLabel statusBar) {
        super(parent, "Поиск", true);
        setLayout(new GridLayout(3, 2));
        setSize(600, 200);

        JLabel label = new JLabel("Введите значение для поиска:");
        JTextField textField = new JTextField();
        JButton searchButton = new JButton("Поиск");
        JButton cancelButton = new JButton("Отмена");

        add(label);
        add(textField);
        add(searchButton);
        add(cancelButton);

        if (type.equals("range")) {
            label.setText("Формат ввода: [Код товара] [Пробел] [>, <, =]");
        } else if (type.equals("date")) {
            label.setText("Формат: yyyy-MM-dd");
        }

        searchButton.addActionListener(e -> {
            String input = textField.getText();
            List<ProductRecord> results = null;

            try {
                switch (type) {
                    case "warehouse" -> results = database.searchByWarehouse(Integer.parseInt(input));
                    case "code" -> results = database.searchByProductCode(Integer.parseInt(input));
                    case "date" -> results = database.searchByEntryDate(ProductRecord.parseDate(input));
                    case "storage" -> results = database.searchByStoragePeriod(Integer.parseInt(input));
                    case "range" -> {
                        String[] parts = input.split(" ");
                        if (parts.length != 2) {
                            throw new IllegalArgumentException("Неверный формат ввода. Пример: '100 >'");
                        }
                        int code = Integer.parseInt(parts[0]);
                        String operator = parts[1];
                        results = database.searchByProductCodeRange(code, operator);
                    }
                }

                if (results != null) {
                    tableModel.setRowCount(0);
                    for (ProductRecord record : results) {
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
                    statusBar.setText("Результаты поиска обновлены.");
                    dispose();
                }

            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Ошибка: " + ex.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
            }
        });

        cancelButton.addActionListener(e -> dispose());
        setVisible(true);
    }
}