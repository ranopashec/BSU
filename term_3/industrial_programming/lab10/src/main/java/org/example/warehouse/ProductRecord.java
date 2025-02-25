package org.example.warehouse;

import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.Date;

public class ProductRecord implements Serializable {
    private static final long serialVersionUID = 1L;

    private int warehouseNumber;
    private int productCode;
    private String productName;
    private Date entryDate;
    private int storagePeriodDays;
    private int quantity;
    private double unitPrice;

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");

    public ProductRecord() {}

    public ProductRecord(int warehouseNumber, int productCode, String productName, Date entryDate,
                         int storagePeriodDays, int quantity, double unitPrice) {
        this.warehouseNumber = warehouseNumber;
        this.productCode = productCode;
        this.productName = productName;
        this.entryDate = entryDate;
        this.storagePeriodDays = storagePeriodDays;
        this.quantity = quantity;
        this.unitPrice = unitPrice;
    }

    public int getWarehouseNumber() {
        return warehouseNumber;
    }

    public void setWarehouseNumber(int warehouseNumber) {
        this.warehouseNumber = warehouseNumber;
    }

    public int getProductCode() {
        return productCode;
    }

    public void setProductCode(int productCode) {
        this.productCode = productCode;
    }

    public String getProductName() {
        return productName;
    }

    public void setProductName(String productName) {
        this.productName = productName;
    }

    public Date getEntryDate() {
        return entryDate;
    }

    public void setEntryDate(Date entryDate) {
        this.entryDate = entryDate;
    }

    public int getStoragePeriodDays() {
        return storagePeriodDays;
    }

    public void setStoragePeriodDays(int storagePeriodDays) {
        this.storagePeriodDays = storagePeriodDays;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    public double getUnitPrice() {
        return unitPrice;
    }

    public void setUnitPrice(double unitPrice) {
        this.unitPrice = unitPrice;
    }

    // Метод для получения даты в виде строки (без времени)
    public String getEntryDateWithoutTime() {
        return dateFormat.format(entryDate);
    }

    public static SimpleDateFormat getDateFormat() {
        return new SimpleDateFormat("yyyy-MM-dd");
    }

    @Override
    public String toString() {
        return "ProductRecord{" +
                "warehouseNumber=" + warehouseNumber +
                ", productCode=" + productCode +
                ", productName='" + productName + '\'' +
                ", entryDate=" + dateFormat.format(entryDate) +
                ", storagePeriodDays=" + storagePeriodDays +
                ", quantity=" + quantity +
                ", unitPrice=" + unitPrice +
                '}';
    }

    public static Date parseDate(String dateStr) throws IllegalArgumentException {
        try {
            return dateFormat.parse(dateStr);
        } catch (Exception e) {
            throw new IllegalArgumentException("Неверный формат даты: " + dateStr + ". Ожидается формат yyyy-MM-dd.");
        }
    }

    public Date getExpirationDate() {
        long expirationTime = entryDate.getTime() + (long) storagePeriodDays * 24 * 60 * 60 * 1000;
        return new Date(expirationTime);
    }
}
