package org.example.warehouse;

import java.io.Serializable;
import java.util.*;

public class Index implements Serializable {
    private static final long serialVersionUID = 1L;

    private final Map<Integer, List<Long>> warehouseIndex;
    private final Map<Integer, List<Long>> productCodeIndex;
    private final Map<String, List<Long>> entryDateIndex;
    private final Map<Integer, List<Long>> storagePeriodIndex;

    public Index() {
        warehouseIndex = new HashMap<>();
        productCodeIndex = new HashMap<>();
        entryDateIndex = new HashMap<>();
        storagePeriodIndex = new HashMap<>();
    }

    public void addToIndex(ProductRecord record, long position) throws KeyNotUniqueException {
        addToMap(warehouseIndex, record.getWarehouseNumber(), position);
        addToMap(productCodeIndex, record.getProductCode(), position);

        String entryDateKey = record.getEntryDateWithoutTime();
        addToMap(entryDateIndex, entryDateKey, position);

        addToMap(storagePeriodIndex, record.getStoragePeriodDays(), position);
    }

    private void addToMap(Map<Integer, List<Long>> index, int key, long position) throws KeyNotUniqueException {
        List<Long> positions = index.computeIfAbsent(key, k -> new ArrayList<>());
        if (positions.contains(position)) {
            throw new KeyNotUniqueException(String.valueOf(key));
        }
        positions.add(position);
    }

    private void addToMap(Map<String, List<Long>> index, String key, long position) {
        List<Long> positions = index.computeIfAbsent(key, k -> new ArrayList<>());
        positions.add(position);
    }

    public List<Long> getByWarehouseNumber(int warehouseNumber) {
        return warehouseIndex.getOrDefault(warehouseNumber, Collections.emptyList());
    }

    public List<Long> getByProductCode(int productCode) {
        return productCodeIndex.getOrDefault(productCode, Collections.emptyList());
    }

    public List<Long> getByEntryDate(String entryDate) {
        return entryDateIndex.getOrDefault(entryDate, Collections.emptyList());
    }

    public List<Long> getByStoragePeriod(int days) {
        return storagePeriodIndex.getOrDefault(days, Collections.emptyList());
    }

    public Map<Integer, List<Long>> getProductCodeIndex() {
        return productCodeIndex;
    }

    public boolean containsWarehouseNumber(int warehouseNumber) {
        return warehouseIndex.containsKey(warehouseNumber);
    }

    public boolean containsProductCode(int productCode) {
        return productCodeIndex.containsKey(productCode);
    }

    public void removeProductCode(int productCode) {
        productCodeIndex.remove(productCode);
    }
}
