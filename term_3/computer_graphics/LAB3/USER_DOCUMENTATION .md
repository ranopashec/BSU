# Пользовательская документация

## 1. Введение

Программа **ImageProcessor** предназначена для загрузки, обработки и отображения изображений. Приложение позволяет пользователям выполнять различные операции над изображениями, такие как улучшение резкости и морфологические операции (эрозия, дилатация, открытие и закрытие).

## 2. Системные требования

- Операционная система: Windows XP и выше, macOS, Linux
- Программное обеспечение: Python 3.x
- Библиотеки: Tkinter (входит в стандартную библиотеку Python), OpenCV, Pillow (PIL), NumPy
- Аппаратные требования:
  - Минимум 1 ГБ ОЗУ
  - 100 МБ свободного места на диске

## 3. Установка

### 3.1 Установка Python

1. Перейдите на официальный сайт Python: [python.org](https://python.org).
2. Скачайте последнюю версию Python 3.x.
3. Запустите установщик и следуйте инструкциям.

### 3.2 Установка необходимых библиотек

1. Откройте командную строку.
2. Установите библиотеки OpenCV, Pillow и NumPy:
   ```bash
   pip install opencv-python pillow numpy
   ```

### 3.3 Скачивание программы

1. Перейдите в репозиторий проекта: [ImageProcessor](https://github.com/yourusername/imageprocessor).
2. Скачайте файл `main.py` или клонируйте репозиторий:
   ```bash
   git clone https://github.com/yourusername/imageprocessor.git
   ```

### 3.4 Запуск программы

1. Откройте командную строку.
2. Перейдите в директорию:
   ```bash
   cd путь/к/папке/imageprocessor
   ```
3. Запустите программу:
   ```bash
   python main.py
   ```

## 4. Использование приложения

### 4.1 Загрузка изображения

1. Нажмите кнопку "Load Image" для выбора изображения.
2. В открывшемся диалоговом окне выберите нужное изображение и нажмите "OK".

### 4.2 Улучшение резкости изображения

1. После загрузки изображения нажмите кнопку "Sharpen Image".
2. Программа применит фильтр для улучшения резкости изображения и отобразит результат.

### 4.3 Морфологические операции

1. Выберите тип морфологической операции из выпадающего меню:
   - Erosion (Эрозия)
   - Dilation (Дилатация)
   - Opening (Открытие)
   - Closing (Закрытие)
2. Введите размер ядра (kernel size) в соответствующее поле (по умолчанию 3).
3. Нажмите кнопку "Apply Morphological Operation" для применения выбранной операции.
4. Программа применит выбранную операцию и отобразит результат.

### 4.4 Отображение изображения

- Обработанное изображение отображается в главном окне приложения.

## 5. Пример использования

### Пример улучшения резкости изображения

1. Загрузите изображение, нажав кнопку "Load Image".
2. Нажмите кнопку "Sharpen Image" для улучшения резкости изображения.
3. Просмотрите результат в главном окне приложения.

### Пример применения морфологической операции

1. Загрузите изображение, нажав кнопку "Load Image".
2. Выберите тип морфологической операции (например, Erosion).
3. Введите размер ядра (например, 5).
4. Нажмите кнопку "Apply Morphological Operation".
5. Просмотрите результат в главном окне приложения.

## 6. Обратная связь

Для вопросов и сообщений о багах: yourusername@gmail.com

## Приложения

## Приложение 1. Список реализованных операций

1. Улучшение резкости изображения
2. Морфологические операции:
   - Erosion (Эрозия)
   - Dilation (Дилатация)
   - Opening (Открытие)
   - Closing (Закрытие)

## Лицензия

Данное приложение распространяется на условиях лицензии MIT. Подробнее смотрите файл LICENSE в репозитории.

## Дополнительные материалы

- Репозиторий проекта: [ImageProcessor](https://github.com/yourusername/imageprocessor)

## Обновления документации

Документация обновляется при внесении изменений в приложение. Рекомендуется регулярно проверять актуальность документации в репозитории проекта.
