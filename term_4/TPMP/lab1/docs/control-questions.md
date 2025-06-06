# Контрольные вопросы

## 1. Какая текущая версия macOS?
Определить текущую версию macOS можно с помощью команды:
sw_vers -productVersion
У меня версия 14.0

## 2. На основе какой UNIX-подобной системы построена macOS?
macOS построена на основе BSD (Berkeley Software Distribution) и использует ядро XNU.

## 3. К какому типу относится ядро macOS? В чем отличия между XNU и Mach?
Ядро macOS является гибридным. Оно сочетает микроядро Mach и модифицированное ядро BSD.
- Mach — микроядро, обеспечивающее базовые функции ОС.
- XNU (X is Not Unix) — ядро macOS, в котором Mach дополнено компонентами BSD и I/O Kit.

## 4. Какой тип архитектуры у процессоров линейки M1 и выше, производимых Apple?
Архитектура ARM64 (AArch64).

## 5. Что такое виртуализация? Приведите примеры гипервизоров.
Виртуализация — это технология, позволяющая запускать несколько операционных систем на одном физическом оборудовании.
Примеры гипервизоров:
- VMware Workstation
- VirtualBox
- Parallels Desktop
- Hyper-V

## 6. Что такое «командная оболочка» / «командный интерпретатор»?
Командная оболочка (Shell) — это программа, принимающая команды пользователя и передающая их операционной системе.

## 7. Как называется командный интерпретатор и какая его версия в используемой macOS?
По умолчанию в macOS используется zsh (Z shell).
Версию можно узнать командой:
zsh --version
У меня версия zsh 5.9

## 8. Для каких задач предназначено приложение Terminal в macOS?
Terminal используется для выполнения команд в командной строке macOS.

## 9. Полный путь к домашней папке и команда для его определения
Полный путь:
/Users/$(whoami)
Команда для вывода пути:
echo $HOME

## 10. Какая команда выводит содержимое каталога системных конфигурационных файлов?
ls -l /etc

## 11. Как установить файловый менеджер Midnight Commander в macOS?
brew install mc
Другие варианты:
sudo port install mc  # Через MacPorts
nix-env -i mc        # Через Nix Package Manager

## 12. Как вывести справочную информацию о команде?
man <команда>
help <команда>
<команда> --help

## 13. Для чего используется команда open?
Открывает файлы и приложения:
open file.txt   # Открыть файл в стандартном редакторе
open .          # Открыть текущую папку в Finder

## 14. Примеры группировки и перенаправления в zsh
ls > files.txt  # Перенаправление вывода в файл
ls | grep .c    # Передача вывода одной команды в другую

## 15. Как сменить оболочку с zsh на bash?
chsh -s /bin/bash

## 16. В каких случаях выполняется инициализация репозитория?
При создании нового репозитория:
git init

## 17. Что такое коммит?
Фиксация изменений в репозитории Git.

## 18. Почему перед созданием новой ветки важно сделать коммит?
Чтобы сохранить текущее состояние и избежать конфликтов.

## 19. В каких состояниях может находиться файл в Git?
- Untracked (неотслеживаемый)
- Staged (готов к коммиту)
- Committed (зафиксирован)

## 20. Что такое ветка в Git?
Отдельная линия разработки в репозитории.

## 21. Как создать ветку master при клонировании репозитория?
После первого коммита в пустом репозитории создаётся ветка main.

## 22. Что такое HEAD в Git?
Указатель на текущий коммит.

## 23. Способы создания веток
git branch new-branch   # Создать ветку

## 24. Как узнать текущую ветку?
git branch

## 25. Как переключаться между ветками?
git checkout branch-name

## 26. Как слить две ветки?
git merge branch-name

## 27. Как подключить внешний репозиторий?
git remote add origin <URL>

## 28. Как отменить последний коммит?
git reset --soft HEAD~1

## 29. Для чего предназначена команда rebase?
Обновляет историю коммитов, упрощая структуру веток.

## 30. В каких случаях требуется форк?
При работе с чужими репозиториями. Можно создать форк через интерфейс GitHub или командой:
git clone --bare <repo-url>
