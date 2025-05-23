# Коллоквиум №3: Поведенческие шаблоны проектирования
Петраков Семён Александрович
Группа 10
Вариант 2

## Суть задачи

1. Рассмотреть три поведенческих шаблона:  
- Состояние (State)  
- Посредник (Mediator)  
- Наблюдатель (Observer)  

2. Ответы на вопросы:  
**Что такое дизайн ПО и как на него влияет многопоточность**

3. Описание каждого шаблона дано как пара «проблема — решение» через призму:
- инкапсуляции,
- принципа "разделяй и властвуй",
- разработки на базе ортогональных стратегий,
- с учётом аспектов многопоточного выполнения.

---

## 1. Шаблон «Состояние» (`state.cpp`)

### Проблема  
Объект меняет поведение в зависимости от своего состояния, но логика разрастается за счёт множества условных конструкций `if/else`, усложняя поддержку и развитие.

### Решение  
Инкапсулировать поведение каждого состояния в отдельный класс и делегировать вызовы текущему состоянию. Это упрощает добавление новых состояний и изолирует логику.

### Инкапсуляция  
Поведение изолировано в специализированных классах состояний.

### Разделяй и властвуй  
Логика каждого состояния не связана с другими — изменения локальны.

### Ортогональные стратегии  
Каждое состояние реализует единый интерфейс, обеспечивая заменяемость и совместимость.

### Многопоточность  
Подходит для проектирования потоков с собственным поведением (worker state). При совместном доступе требуется синхронизация состояния.

---

## 2. Шаблон «Посредник» (`mediator.cpp`)

### Проблема  
Объекты напрямую взаимодействуют друг с другом, создавая сложные зависимости и затрудняя модификации.

### Решение  
Создаётся центральный объект-посредник, управляющий взаимодействием между компонентами. Компоненты больше не зависят друг от друга напрямую.

### Инкапсуляция  
Коммуникационная логика скрыта в посреднике.

### Разделяй и властвуй  
Компоненты становятся слабо связанными — проще разрабатывать и тестировать по отдельности.

### Ортогональные стратегии  
Поведение участников не зависит от других, можно заменять или переиспользовать компоненты.

### Многопоточность  
Посредник может координировать потоки. Важно обеспечить потокобезопасность при доступе к общим данным.

---

## 3. Шаблон «Наблюдатель» (`observer.cpp`)

### Проблема  
Нужно обеспечить автоматическое оповещение всех зависимых объектов при изменении состояния другого объекта.

### Решение  
Субъект хранит список наблюдателей и уведомляет их через общий интерфейс. Наблюдатели подписываются и отписываются по необходимости.

### Инкапсуляция  
Субъект ничего не знает о реализации наблюдателей — только об их интерфейсе.

### Разделяй и властвуй  
Модель «один ко многим» без прямых зависимостей.

### Ортогональные стратегии  
Разные наблюдатели могут по-разному реагировать на одно событие.

### Многопоточность  
Наблюдатели могут работать в отдельных потоках. Важно синхронизировать доступ к списку подписчиков и рассылку уведомлений.

### Пример из Qt  
Использована сигнално-слотовая система Qt (`QObject`, `signals`, `slots`) — встроенная реализация паттерна Observer.

---

## 4. Дизайн ПО?

### Определение  
Дизайн ПО — это процесс планирования внутренней структуры программы: классов, модулей, интерфейсов, зависимостей и взаимодействий между компонентами.

Цель дизайна — обеспечить гибкость, расширяемость и простоту поддержки.

### Влияние многопоточности на дизайн  
- Необходимость проектировать **потокобезопасные структуры**  
- Введение **немодифицируемых объектов** и принципов иммутабельности  
- Добавление **очередей, событий, асинхронных вызовов** в архитектуру  
- Повышенная сложность тестирования, логирования и отладки  
- Разделение бизнес-логики и многопоточного исполнения через паттерны (например, Strategy, Command, Future)
