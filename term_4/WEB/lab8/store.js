const fs = require('fs');
const path = require('path');

const dataPath = path.join(__dirname, 'data', 'transports.json');

// Чтение всех данных из файла
function readData() {
    try {
        const data = fs.readFileSync(dataPath, 'utf8');
        return JSON.parse(data);
    } catch (err) {
        // Если файла нет, возвращаем пустой массив
        if (err.code === 'ENOENT') {
            return [];
        }
        throw err;
    }
}

// Запись данных в файл
function writeData(data) {
    fs.writeFileSync(dataPath, JSON.stringify(data, null, 2), 'utf8');
}

// Генерация ID
function generateId() {
    return Date.now().toString();
}

module.exports = {
    getAll: () => {
        return readData();
    },
    
    getById: (id) => {
        const transports = readData();
        return transports.find(t => t.id === id);
    },
    
    create: (transportData) => {
        const transports = readData();
        const newTransport = {
            id: generateId(),
            ...transportData,
            createdAt: new Date().toISOString(),
            updatedAt: new Date().toISOString()
        };
        transports.push(newTransport);
        writeData(transports);
        return newTransport;
    },
    
    update: (id, transportData) => {
        const transports = readData();
        const index = transports.findIndex(t => t.id === id);
        
        if (index === -1) return null;
        
        const updatedTransport = {
            ...transports[index],
            ...transportData,
            updatedAt: new Date().toISOString()
        };
        
        transports[index] = updatedTransport;
        writeData(transports);
        return updatedTransport;
    },
    
    delete: (id) => {
        const transports = readData();
        const index = transports.findIndex(t => t.id === id);
        
        if (index === -1) return false;
        
        transports.splice(index, 1);
        writeData(transports);
        return true;
    }
};