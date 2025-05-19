function ShowMenu(MenuItemsA, ParentElem) {
    const menuContainer = document.createElement('div');
    menuContainer.className = 'menu-container';
    
    const menuList = document.createElement('ul');
    menuList.className = 'menu';
    
    MenuItemsA.forEach(item => {
        const menuItem = document.createElement('li');
        menuItem.className = 'menu-item';
        
        if (item.url) {
            const link = document.createElement('a');
            link.href = item.url;
            link.textContent = item.name;
            menuItem.appendChild(link);
        } else {
            menuItem.textContent = item.name;
        }
        
        if (item.submenu) {
            menuItem.classList.add('has-submenu');
            
            menuItem.addEventListener('click', function(e) {
                e.stopPropagation();
                
                const existingSubmenus = menuList.querySelectorAll('.submenu');
                existingSubmenus.forEach(submenu => {
                    if (submenu !== this.nextElementSibling) {
                        submenu.remove();
                    }
                });
                
                if (this.nextElementSibling && this.nextElementSibling.classList.contains('submenu')) {
                    this.nextElementSibling.remove();
                    return;
                }
                
                ShowMenu(item.submenu, this);
            });
        }
        
        menuList.appendChild(menuItem);
    });
    
    menuContainer.appendChild(menuList);
    
    if (ParentElem !== document.getElementById('menuContainer')) {
        menuContainer.className = 'submenu';
        ParentElem.appendChild(menuContainer);
    } else {
        ParentElem.appendChild(menuContainer);
    }
}

document.addEventListener('click', function() {
    const submenus = document.querySelectorAll('.submenu');
    submenus.forEach(submenu => {
        submenu.remove();
    });
});

window.onload = function() {
    const menuContainer = document.getElementById('menuContainer');
    ShowMenu(MenuA, menuContainer);
};