package org.example.warehouse;

import java.io.*;
import java.util.*;

public class ProductDatabase {
    private RandomAccessFile file;
    private final Index index;

    public ProductDatabase(String filename) throws IOException {
        file = new RandomAccessFile(filename, "rw");
        index = new Index();
        loadIndexes();
    }

    public boolean deleteByProductCode(int productCode) throws IOException {
        List<Long> positions = index.getByProductCode(productCode);
        if (positions == null || positions.isEmpty()) {
            System.out.println("Записи с кодом товара " + productCode + " не найдены.");
            return false;
        }

        for (long position : positions) {
            file.seek(position);
            ProductRecord record = readRecord();

            record.setProductCode(-1);
            file.seek(position);
            writeRecord(record);
        }

        index.removeProductCode(productCode);
        System.out.println("Записи с кодом товара " + productCode + " успешно удалены.");
        return true;
    }

    public List<ProductRecord> getAllRecords() throws IOException {
        List<ProductRecord> records = new ArrayList<>();
        file.seek(0);
        while (file.getFilePointer() < file.length()) {
            ProductRecord record = readRecord();
            if (record.getProductCode() != -1) {
                records.add(record);
            }
        }
        return records;
    }


    private void loadIndexes() throws IOException {
        file.seek(0);
        while (file.getFilePointer() < file.length()) {
            long position = file.getFilePointer();
            ProductRecord record = readRecord();
            try {
                index.addToIndex(record, position);
            } catch (KeyNotUniqueException ignored) {

            }
        }
    }

    public void addRecord(ProductRecord record) throws IOException, KeyNotUniqueException {
        file.seek(file.length());
        long position = file.getFilePointer();
        writeRecord(record);
        index.addToIndex(record, position);
    }

    public List<ProductRecord> searchByWarehouse(int warehouseNumber) throws IOException {
        List<Long> positions = index.getByWarehouseNumber(warehouseNumber);
        return readRecordsAtPositions(positions).stream()
                .filter(record -> record.getProductCode() != -1)
                .toList();
    }

    public List<ProductRecord> searchByProductCode(int productCode) throws IOException {
        List<Long> positions = index.getByProductCode(productCode);
        return readRecordsAtPositions(positions).stream().filter(record -> record.getProductCode() != -1).toList();
    }

    public List<ProductRecord> searchByEntryDate(Date date) throws IOException {
        String dateKey = ProductRecord.getDateFormat().format(date);
        List<Long> positions = index.getByEntryDate(dateKey);
        return readRecordsAtPositions(positions).stream()
                .filter(record -> record.getProductCode() != -1)
                .toList();
    }

    public List<ProductRecord> searchByStoragePeriod(int days) throws IOException {
        List<Long> positions = index.getByStoragePeriod(days);
        return readRecordsAtPositions(positions).stream()
                .filter(record -> record.getProductCode() != -1)
                .toList();
    }

    public List<ProductRecord> searchByProductCodeRange(int code, String operator) throws IOException {
        List<ProductRecord> results = new ArrayList<>();

        for (Map.Entry<Integer, List<Long>> entry : index.getProductCodeIndex().entrySet()) {
            boolean condition = switch (operator) {
                case ">" -> entry.getKey() > code;
                case "<" -> entry.getKey() < code;
                case "=" -> entry.getKey() == code;
                default -> throw new IllegalArgumentException("Неверный оператор: " + operator);
            };

            if (condition) {
                for (long pos : entry.getValue()) {
                    ProductRecord record = readRecordAt(pos);
                    if (record.getProductCode() != -1) {
                        results.add(record);
                    }
                }
            }
        }
        return results;
    }

    private List<ProductRecord> readRecordsAtPositions(List<Long> positions) throws IOException {
        List<ProductRecord> records = new ArrayList<>();
        for (long position : positions) {
            records.add(readRecordAt(position));
        }
        return records;
    }

    private void writeRecord(ProductRecord record) throws IOException {
        file.writeInt(record.getWarehouseNumber());
        file.writeInt(record.getProductCode());
        writeString(record.getProductName());
        file.writeLong(record.getEntryDate().getTime());
        file.writeInt(record.getStoragePeriodDays());
        file.writeInt(record.getQuantity());
        file.writeDouble(record.getUnitPrice());
    }

    private ProductRecord readRecord() throws IOException {
        int warehouseNumber = file.readInt();
        int productCode = file.readInt();
        String productName = readString();
        Date entryDate = new Date(file.readLong());
        int storagePeriodDays = file.readInt();
        int quantity = file.readInt();
        double unitPrice = file.readDouble();
        return new ProductRecord(warehouseNumber, productCode, productName, entryDate, storagePeriodDays, quantity, unitPrice);
    }

    private ProductRecord readRecordAt(long position) throws IOException {
        file.seek(position);
        return readRecord();
    }

    private void writeString(String str) throws IOException {
        byte[] bytes = str.getBytes("UTF-8");
        file.writeInt(bytes.length);
        file.write(bytes);
    }

    private String readString() throws IOException {
        int length = file.readInt();
        byte[] bytes = new byte[length];
        file.readFully(bytes);
        return new String(bytes, "UTF-8");
    }

    public void close() throws IOException {
        file.close();
    }
}
