// Удаление транспорта
async function deleteTransport(id) {
    if (confirm('Вы уверены, что хотите удалить это транспортное средство?')) {
        try {
            const response = await fetch(`/api/transports/${id}`, {
                method: 'DELETE'
            });
            
            if (response.ok) {
                window.location.href = '/';
            } else {
                const error = await response.json();
                alert(error.error || 'Ошибка при удалении');
            }
        } catch (err) {
            console.error('Error:', err);
            alert('Ошибка при удалении');
        }
    }
}

// Обработка формы создания/редактирования
document.addEventListener('DOMContentLoaded', () => {
    const form = document.getElementById('transportForm');
    if (form) {
        form.addEventListener('submit', async (e) => {
            e.preventDefault();
            
            const formData = new FormData(form);
            const transportData = Object.fromEntries(formData.entries());
            
            // Преобразуем capacity в число
            if (transportData.capacity) {
                transportData.capacity = parseInt(transportData.capacity);
            }
            
            try {
                let response;
                if (transportData.id) {
                    // Редактирование существующего
                    response = await fetch(`/api/transports/${transportData.id}`, {
                        method: 'PUT',
                        headers: {
                            'Content-Type': 'application/json'
                        },
                        body: JSON.stringify(transportData)
                    });
                } else {
                    // Создание нового
                    response = await fetch('/api/transports', {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json'
                        },
                        body: JSON.stringify(transportData)
                    });
                }
                
                if (response.ok) {
                    window.location.href = '/';
                } else {
                    const error = await response.json();
                    alert(error.error || 'Ошибка при сохранении');
                }
            } catch (err) {
                console.error('Error:', err);
                alert('Ошибка при сохранении');
            }
        });
    }
});