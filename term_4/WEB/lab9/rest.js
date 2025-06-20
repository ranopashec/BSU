const store = require('./store');

module.exports = function (app) {
    app.get('/', (req, res) => {
        res.render('index');
    });

    app.get('/api/parts', (req, res) => {
        res.json(store.getAll());
    });

    app.get('/parts', (req, res) => {
        const page = parseInt(req.query.page, 10) || 1;
        const limit = parseInt(req.query.limit, 10) || 5;
        const search = (req.query.search || '').toLowerCase();
        const sort = req.query.sort === 'asc'
            ? 'asc'
            : req.query.sort === 'desc'
                ? 'desc'
                : null;

        let entries = Object.entries(store.getAll())
            .filter(([name]) =>
                name.toLowerCase().includes(search)
            );
        if (sort) {
            entries.sort((a, b) => {
                const na = a[0].toLowerCase(), nb = b[0].toLowerCase();
                return sort === 'asc'
                    ? na.localeCompare(nb)
                    : nb.localeCompare(na);
            });
        }

        const total = entries.length;
        const totalPages = Math.ceil(total / limit);
        const offset = (page - 1) * limit;
        const paged = entries.slice(offset, offset + limit);
        const partsPaged = Object.fromEntries(paged);

        res.render('list', {
            parts: partsPaged,
            currentPage: page,
            totalPages,
            search: req.query.search || '',
            sort,
            limit
        });
    });

    app.get('/parts/add', (req, res) => {
        res.render('add');
    });

    app.post('/parts/add', (req, res) => {
        const { name, description, price } = req.body;
        store.add(name, { description, price: parseFloat(price) });
        res.redirect('/parts');
    });

    app.get('/parts/edit/:name', (req, res) => {
        const part = store.get(req.params.name);
        res.render('edit', { oldName: req.params.name, part });
    });

    app.post('/parts/edit/:oldName', (req, res) => {
        const { name, description, price } = req.body;
        store.update(req.params.oldName, name, { description, price: parseFloat(price) });
        res.redirect('/parts');
    });

    app.post('/parts/delete/:name', (req, res) => {
        store.delete(req.params.name);
        res.redirect('/parts');
    });
};
