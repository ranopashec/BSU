package org.example.gui;

import org.example.warehouse.ProductDatabase;


import javax.swing.*;
import java.awt.*;

public class DeleteDialog extends JDialog {
    public DeleteDialog(JFrame parent, ProductDatabase database) {
        super(parent, "Удалить по коду товара", true);
        setLayout(new GridLayout(2, 2));
        setSize(300, 150);

        JLabel label = new JLabel("Введите код товара для удаления:");
        JTextField textField = new JTextField();
        JButton deleteButton = new JButton("Удалить");
        JButton cancelButton = new JButton("Отмена");

        add(label);
        add(textField);
        add(deleteButton);
        add(cancelButton);

        deleteButton.addActionListener(e -> {
            try {
                int productCode = Integer.parseInt(textField.getText());
                boolean result = database.deleteByProductCode(productCode); // Удаляем товар
                if (result) {
                    JOptionPane.showMessageDialog(this, "Товар успешно удалён.");
                    ((MainFrame) parent).displayAll(); // Немедленно обновляем клиентскую форму
                } else {
                    JOptionPane.showMessageDialog(this, "Товар с таким кодом не найден.");
                }
                dispose();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Ошибка удаления: " + ex.getMessage());
            }
        });

        cancelButton.addActionListener(e -> dispose());

        setVisible(true);
    }
}
