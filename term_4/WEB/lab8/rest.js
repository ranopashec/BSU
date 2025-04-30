const fs = require('fs');
const path = require('path');
const store = require('./store');

module.exports = function(app) {
    // Устанавливаем EJS как шаблонизатор
    app.set('view engine', 'ejs');
    app.set('views', path.join(__dirname, 'views'));

    // Главная страница - список всех транспортных средств
    app.get('/', (req, res) => {
        const transports = store.getAll();
        res.render('index', { transports });
    });

    // Страница создания нового транспортного средства
    app.get('/create', (req, res) => {
        res.render('create');
    });

    // Страница просмотра деталей транспортного средства
    app.get('/:id', (req, res) => {
        const transport = store.getById(req.params.id);
        if (transport) {
            res.render('details', { transport });
        } else {
            res.status(404).send('Transport not found');
        }
    });

    // Страница редактирования транспортного средства
    app.get('/:id/edit', (req, res) => {
        const transport = store.getById(req.params.id);
        if (transport) {
            res.render('edit', { transport });
        } else {
            res.status(404).send('Transport not found');
        }
    });

    // API endpoints
    app.post('/api/transports', (req, res) => {
        const newTransport = store.create(req.body);
        res.status(201).json(newTransport);
    });

    app.get('/api/transports', (req, res) => {
        const transports = store.getAll();
        res.json(transports);
    });

    app.get('/api/transports/:id', (req, res) => {
        const transport = store.getById(req.params.id);
        if (transport) {
            res.json(transport);
        } else {
            res.status(404).json({ error: 'Transport not found' });
        }
    });

    app.put('/api/transports/:id', (req, res) => {
        const updatedTransport = store.update(req.params.id, req.body);
        if (updatedTransport) {
            res.json(updatedTransport);
        } else {
            res.status(404).json({ error: 'Transport not found' });
        }
    });

    app.delete('/api/transports/:id', (req, res) => {
        const isDeleted = store.delete(req.params.id);
        if (isDeleted) {
            res.status(204).send();
        } else {
            res.status(404).json({ error: 'Transport not found' });
        }
    });
};