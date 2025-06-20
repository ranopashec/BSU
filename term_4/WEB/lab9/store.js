const fs = require('fs');
const path = './data.json';

function loadData() {
    const data = fs.readFileSync(path, 'utf-8');
    return JSON.parse(data);
}

function saveData(data) {
    fs.writeFileSync(path, JSON.stringify(data, null, 4), 'utf-8');
}

module.exports = {
    getAll: () => loadData(),
    get: (name) => loadData()[name],
    add: (name, part) => {
        const data = loadData();
        data[name] = part;
        saveData(data);
    },
    update: (oldName, newName, part) => {
        const data = loadData();
        if (oldName !== newName) {
            delete data[oldName];
        }
        data[newName] = part;
        saveData(data);
    },
    delete: (name) => {
        const data = loadData();
        delete data[name];
        saveData(data);
    }
};
