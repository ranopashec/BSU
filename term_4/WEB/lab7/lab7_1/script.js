// Текстовое содержимое страниц
const pages = {
    "Main": {
        title: "Transport Company",
        content: "Welcome to our Transport Company. We provide reliable transportation services worldwide."
    },
    "About": {
        title: "About Us",
        content: "At Transport Company, we redefine the essence of mobility. With a relentless commitment to excellence, innovation, and sustainability, we stand at the forefront of the transportation industry."
    }
};

// Функция для отображения страницы
function showPage(pageName) {
    const page = pages[pageName];
    document.getElementById('content').innerHTML = `
        <h2>${page.title}</h2>
        <p>${page.content}</p>
    `;
}