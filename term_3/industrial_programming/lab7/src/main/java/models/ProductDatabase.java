package models;

import lombok.Getter;
import lombok.Setter;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;

@Getter
@Setter
public class ProductDatabase {
    private RandomAccessFile file;
    private Map<Integer, List<Long>> warehouseIndex;
    private Map<Integer, Long> productCodeIndex;
    private Map<String, List<Long>> entryDateIndex;
    private Map<Integer, List<Long>> storagePeriodIndex;

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");

    public ProductDatabase(String filename) throws IOException {
        file = new RandomAccessFile(filename, "rw");
        warehouseIndex = new HashMap<>();
        productCodeIndex = new HashMap<>();
        entryDateIndex = new HashMap<>();
        storagePeriodIndex = new HashMap<>();
        loadIndexes();
    }

    private void loadIndexes() throws IOException {
        file.seek(0);
        while (file.getFilePointer() < file.length()) {
            long position = file.getFilePointer();
            ProductRecord record = readRecord();

            warehouseIndex.computeIfAbsent(record.getWarehouseNumber(), k -> new ArrayList<>()).add(position);
            productCodeIndex.put(record.getProductCode(), position);

            String entryDateKey = record.getEntryDateWithoutTime();
            entryDateIndex.computeIfAbsent(entryDateKey, k -> new ArrayList<>()).add(position);

            storagePeriodIndex.computeIfAbsent(record.getStoragePeriodDays(), k -> new ArrayList<>()).add(position);
        }
    }

    public void addRecord(ProductRecord record) throws IOException {
        file.seek(file.length());
        long position = file.getFilePointer();
        writeRecord(record);

        warehouseIndex.computeIfAbsent(record.getWarehouseNumber(), k -> new ArrayList<>()).add(position);
        productCodeIndex.put(record.getProductCode(), position);

        String entryDateKey = record.getEntryDateWithoutTime();
        entryDateIndex.computeIfAbsent(entryDateKey, k -> new ArrayList<>()).add(position);

        storagePeriodIndex.computeIfAbsent(record.getStoragePeriodDays(), k -> new ArrayList<>()).add(position);
    }

    public List<ProductRecord> searchByWarehouse(int warehouseNumber) throws IOException {
        List<ProductRecord> results = new ArrayList<>();
        List<Long> pointers = warehouseIndex.get(warehouseNumber);
        if (pointers != null)
            for (Long pointer : pointers)
                results.add(readRecordAt(pointer));
        return results;
    }

    public ProductRecord searchByProductCode(int productCode) throws IOException {
        Long pointer = productCodeIndex.get(productCode);
        if (pointer != null)
            return readRecordAt(pointer);
        return null;
    }

    public List<ProductRecord> searchByEntryDate(Date date) throws IOException {
        String dateKey = dateFormat.format(date);
        List<ProductRecord> results = new ArrayList<>();
        List<Long> pointers = entryDateIndex.get(dateKey);
        if (pointers != null)
            for (Long pointer : pointers)
                results.add(readRecordAt(pointer));
        return results;
    }

    public List<ProductRecord> searchByStoragePeriod(int days) throws IOException {
        List<ProductRecord> results = new ArrayList<>();
        List<Long> pointers = storagePeriodIndex.get(days);
        if (pointers != null)
            for (Long pointer : pointers)
                results.add(readRecordAt(pointer));
        return results;
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
        return new ProductRecord(warehouseNumber, productCode, productName, entryDate,
                storagePeriodDays, quantity, unitPrice);
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

    public void displayAllRecords() throws IOException {
        file.seek(0);  // Ставим указатель в начало файла
        while (file.getFilePointer() < file.length()) {
            ProductRecord record = readRecord();  // Читаем запись
            System.out.println(record);  // Печатаем запись в консоль
        }
    }
}
