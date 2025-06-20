const express = require('express');
const path = require('path');
const app = express();
const PORT = 3000;

// Раздаём статику из папки dist
app.use(express.static(path.join(__dirname, 'dist')));

// Путь к JSON-файлу
app.get('/data', (req, res) => {
    res.sendFile(path.join(__dirname, 'files', 'base.json'));
});

// Отдаём index.html по умолчанию
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'dist', 'index.html'));
});

// Скачивание JSON по запросу (доп. условие)
app.get('/download', (req, res) => {
    res.download(path.join(__dirname, 'files', 'base.json'));
});

// Запуск сервера
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
});
