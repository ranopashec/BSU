package org.example.gui;

import org.example.warehouse.ProductDatabase;

import javax.swing.*;

public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                ProductDatabase database = new ProductDatabase("products.dat");

                MainFrame mainFrame = new MainFrame(database);
                mainFrame.setVisible(true);
            } catch (Exception e) {
                JOptionPane.showMessageDialog(null,
                        "Ошибка запуска приложения: " + e.getMessage(),
                        "Ошибка",
                        JOptionPane.ERROR_MESSAGE);
            }
        });
    }
}
