package org.example.warehouse;

public class KeyNotUniqueException extends Exception {
    private static final long serialVersionUID = 1L;

    public KeyNotUniqueException(String key) {
        super("Key is not unique: " + key);
    }
}
