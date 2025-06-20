import { ajax } from 'rxjs/ajax';
import { map, switchMap, startWith, combineLatestWith } from 'rxjs/operators';
import { fromEvent, BehaviorSubject, combineLatest } from 'rxjs';

document.addEventListener('DOMContentLoaded', () => {
    const apiUrl = '/api/parts';
    const limit = 5;

    const searchInput = document.querySelector('input[name="search"]');
    const sortSelect = document.querySelector('select[name="sort"]');
    const listContainer = document.querySelector('.parts-list');
    const prevBtn = document.getElementById('prev-btn');
    const nextBtn = document.getElementById('next-btn');
    const pageInfo = document.getElementById('page-info');

    const parts$ = ajax.getJSON(apiUrl);
    const search$ = fromEvent(searchInput, 'input').pipe(
        map(e => e.target.value), startWith('')
    );
    const sort$ = fromEvent(sortSelect, 'change').pipe(
        map(e => e.target.value), startWith('')
    );

    const page$ = new BehaviorSubject(1);

    fromEvent(prevBtn, 'click').subscribe(() => {
        const p = page$.getValue();
        page$.next(p - 1);
    });
    fromEvent(nextBtn, 'click').subscribe(() => {
        const p = page$.getValue();
        page$.next(p + 1);
    });

    const filteredSorted$ = parts$.pipe(
        switchMap(parts =>
            combineLatest([search$, sort$]).pipe(
                map(([searchTerm, sortOrder]) => {
                    let entries = Object.entries(parts)
                        .filter(([name]) => name.toLowerCase().includes(searchTerm.toLowerCase()));
                    if (sortOrder === 'asc' || sortOrder === 'desc') {
                        entries.sort((a, b) => {
                            const na = a[0].toLowerCase(), nb = b[0].toLowerCase();
                            return sortOrder === 'asc'
                                ? na.localeCompare(nb)
                                : nb.localeCompare(na);
                        });
                    }
                    return entries;
                })
            )
        )
    );

    const paged$ = filteredSorted$.pipe(
        switchMap(entries =>
            page$.pipe(
                map(page => {
                    const totalPages = Math.ceil(entries.length / limit) || 1;
                    const currentPage = Math.min(Math.max(1, page), totalPages);
                    const start = (currentPage - 1) * limit;
                    const paged = entries.slice(start, start + limit);
                    return { paged, currentPage, totalPages };
                })
            )
        )
    );

    paged$.subscribe(({ paged, currentPage, totalPages }) => {
        listContainer.innerHTML = paged.map(([name, part]) =>
            `<li class="part-item">
         <strong>${name}</strong> — ${part.description} — ${part.price}₽
         <a href="/parts/edit/${encodeURIComponent(name)}">Редактировать</a>
         <form method="POST" action="/parts/delete/${encodeURIComponent(name)}" style="display:inline">
           <button type="submit">Удалить</button>
         </form>
       </li>`
        ).join('');

        pageInfo.textContent = `Страница ${currentPage} из ${totalPages}`;
        prevBtn.disabled = currentPage === 1;
        nextBtn.disabled = currentPage === totalPages;
        if (currentPage !== page$.getValue()) {
            page$.next(currentPage);
        }
    });
});
