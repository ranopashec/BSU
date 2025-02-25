package models;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.ToString;

import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.Date;

@Getter
@AllArgsConstructor
@ToString
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
    private static final SimpleDateFormat dateTimeFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

    public String getEntryDateWithoutTime() {
        return dateFormat.format(entryDate);
    }

    public String getEntryDateWithTime() {
        return dateTimeFormat.format(entryDate);
    }

    @Override
    public String toString() {
        return "ProductRecord [warehouseNumber=" + warehouseNumber +
                ", productCode=" + productCode +
                ", productName=" + productName +
                ", entryDate=" + getEntryDateWithTime() +
                ", storagePeriodDays=" + storagePeriodDays +
                ", quantity=" + quantity +
                ", unitPrice=" + unitPrice + "]";
    }
}
