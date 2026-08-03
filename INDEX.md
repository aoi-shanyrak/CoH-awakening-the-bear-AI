# 📚 НАВИГАЦИЯ ПО ДОКУМЕНТАЦИИ

Полный индекс рефакторинга MCTS для CoH-awakening-the-bear-AI.

---

## 🚀 НАЧНИТЕ ОТСЮДА

### ⚡ Очень спешите? (2 минуты)
👉 **[QUICKSTART.md](QUICKSTART.md)**
- Только самое важное
- 4 критичные функции для реализации
- Быстрый тест

### 📖 Хотите понять архитектуру? (15 минут)
👉 **[REFACTORING_SUMMARY.md](REFACTORING_SUMMARY.md)**
- Что изменилось
- Почему это лучше
- Ключевые числа и метрики

### 🏗️ Нужна полная диаграмма? (20 минут)
👉 **[ARCHITECTURE_DIAGRAM.md](ARCHITECTURE_DIAGRAM.md)**
- Четырёхуровневое дерево MCTS
- Примеры расширения узлов
- Формулы и оптимизации

---

## 📋 ПОЛНЫЙ ПУТЕВОДИТЕЛЬ

### 1. **Обзор (30 мин)**
| Документ | Содержимое | Для кого |
|----------|-----------|---------|
| [REFACTORING_SUMMARY.md](REFACTORING_SUMMARY.md) | Что изменилось, почему, результаты | Все |
| [ARCHITECTURE_DIAGRAM.md](ARCHITECTURE_DIAGRAM.md) | Диаграммы, примеры, формулы | Архитекторы |
| [coh-project-analysis.md](coh-project-analysis.md) | Состояние проекта (repo memory) | Новички |

### 2. **Практика (1 час)**
| Документ | Содержимое | Для кого |
|----------|-----------|---------|
| [MCTS_INTEGRATION_GUIDE.md](MCTS_INTEGRATION_GUIDE.md) | Примеры кода, проблемы, решения | Разработчики |
| [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) | CMake, Makefile, компиляция | Devops |
| [REFACTORING.md](REFACTORING.md) | Полное описание каждого изменения | Рецензенты кода |

### 3. **Исходный код (самостоятельно)**
- [src/engine/action/action.hpp](src/engine/action/action.hpp#L33-L47) – новые флаги
- [src/engine/core/action_generation.cpp](src/engine/core/action_generation.cpp) – getBaseActions()
- [src/engine/action/cards/card_utils.hpp](src/engine/action/cards/card_utils.hpp) – проверка карт
- [src/engine/action/cards/card_utils.cpp](src/engine/action/cards/card_utils.cpp) – реализация
- [src/engine/mcts/mcts.hpp](src/engine/mcts/mcts.hpp) – узлы MCTS
- [src/engine/mcts/mcts.cpp](src/engine/mcts/mcts.cpp) – цикл MCTS

---

## 🎯 БЫСТРЫЕ ССЫЛКИ ПО ТЕМАМ

### Если вопрос про: **"Почему ветвление 1800 вместо 500?"**
👉 [ARCHITECTURE_DIAGRAM.md § Анализ разветвления](ARCHITECTURE_DIAGRAM.md#-анализ-разветвления)

### Если вопрос про: **"Как работает MCTSNode?"**
👉 [ARCHITECTURE_DIAGRAM.md § Структура дерева](ARCHITECTURE_DIAGRAM.md#-структура-дерева-4-уровня)

### Если вопрос про: **"Что реализовать в первую очередь?"**
👉 [QUICKSTART.md § ПЛАН РЕАЛИЗАЦИИ](QUICKSTART.md#-план-реализации-приоритет)

### Если вопрос про: **"Как добавить новую карту?"**
👉 [MCTS_INTEGRATION_GUIDE.md § Интеграция карт](MCTS_INTEGRATION_GUIDE.md)

### Если вопрос про: **"Как скомпилировать новые файлы?"**
👉 [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md)

### Если вопрос про: **"Какие ошибки возможны?"**
👉 [MCTS_INTEGRATION_GUIDE.md § Текущие проблемы](MCTS_INTEGRATION_GUIDE.md#-текущие-проблемы-и-их-решения)

---

## 📊 СТАТУС РЕАЛИЗАЦИИ

### ✅ ГОТОВО (4 недели работы)
- [x] Структура Action с флагами (13 bool)
- [x] Модуль card_utils для проверки карт
- [x] Иерархическое дерево MCTSNode
- [x] Класс MCTSSearch с циклом MCTS
- [x] UCB1 выбор узлов
- [x] Полная документация (6 файлов)

### ❌ БЛОКИРУЕТ (критично реализовать)
- [ ] `applyAction()` – применение действия
- [ ] `cloneState()` – копирование состояния
- [ ] `isGameOver()` – условие конца игры
- [ ] `evaluateState()` – оценка позиции
- [ ] полная `simulate()` – разыгрывание игры

### 🟡 ОПТИМИЗАЦИЯ (потом)
- [ ] Нейросеть для замены simulate()
- [ ] Параллельный MCTS
- [ ] Кэш состояний
- [ ] Битовое представление State
- [ ] Генерация тепловых карт

---

## 🔧 ПОЛЕЗНЫЕ КОМАНДЫ

### Компиляция
```bash
# Один файл
g++ -std=c++17 -c src/engine/mcts/mcts.cpp

# Все новые файлы
g++ -std=c++17 -c src/engine/action/cards/card_utils.cpp src/engine/mcts/mcts.cpp

# С CMake
cmake -B build && make -C build
```

### Тестирование
```bash
# Быстрый тест (см. QUICKSTART.md)
./test_basic

# Анализ дерева (см. MCTS_INTEGRATION_GUIDE.md)
./test_tree_expansion

# Полное тестирование
./test_all
```

### Поиск проблем
```bash
# Символы экспортированы?
nm build/*.o | grep getBaseActions

# Включения правильные?
grep -r "#include" src/engine/mcts/

# Нет конфликтов?
g++ -std=c++17 -Wall -Wextra -c ...
```

---

## 📖 РЕКОМЕНДУЕМЫЙ ПОРЯДОК ЧТЕНИЯ

### День 1: Знакомство
1. [QUICKSTART.md](QUICKSTART.md) ⚡
2. [REFACTORING_SUMMARY.md](REFACTORING_SUMMARY.md) 📊
3. Посмотреть [src/engine/mcts/mcts.hpp](src/engine/mcts/mcts.hpp) (структуры)

### День 2: Детали
4. [ARCHITECTURE_DIAGRAM.md](ARCHITECTURE_DIAGRAM.md) 🏗️
5. [REFACTORING.md](REFACTORING.md) 📝
6. Посмотреть [src/engine/action/cards/card_utils.cpp](src/engine/action/cards/card_utils.cpp) (логика)

### День 3: Практика
7. [MCTS_INTEGRATION_GUIDE.md](MCTS_INTEGRATION_GUIDE.md) 💻
8. [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) 🔧
9. Скомпилировать и протестировать

### Неделя 2: Реализация
10. Реализовать `applyAction()`
11. Реализовать `cloneState()`, `isGameOver()`, `evaluateState()`
12. Интегрировать MCTS в основной цикл игры

---

## 🎓 ОБУЧАЮЩИЕ МАТЕРИАЛЫ

### По MCTS алгоритму:
- UCB1 формула: [ARCHITECTURE_DIAGRAM.md § Выбор узла](ARCHITECTURE_DIAGRAM.md#-выбор-узла-ucb1-формула)
- Цикл MCTS: [ARCHITECTURE_DIAGRAM.md § Цикл MCTS](ARCHITECTURE_DIAGRAM.md#-цикл-mcts-одна-итерация)
- Примеры пути: [ARCHITECTURE_DIAGRAM.md § Пример полного пути](ARCHITECTURE_DIAGRAM.md#-пример-полного-пути-от-root-к-terminal)

### По архитектуре коммуникации:
- Структура Action: [REFACTORING.md § Action структура](REFACTORING.md#1-структура-action-расширена-флагами)
- Модуль card_utils: [REFACTORING.md § card_utils](REFACTORING.md#3-новый-модуль-card_utils)
- Класс MCTSSearch: [REFACTORING.md § MCTSSearch](REFACTORING.md#4-новый-модуль-mcts)

---

## 🆘 FAQ (Часто задаваемые вопросы)

### В: Какие файлы я должен был изменить?
О: Три основных файла:
- `src/engine/action/action.hpp` – добавлены флаги
- `src/engine/core/action_generation.cpp` – переделано
- Всё остальное – новые файлы

**Подробнее:** [REFACTORING.md § Что было изменено](REFACTORING.md#-что-было-изменено)

### В: Я должен удалить старый модуль card_actions?
О: Не обязательно. Он больше не используется, но можно оставить для истории или удалить из компиляции.

**Подробнее:** [MCTS_INTEGRATION_GUIDE.md § ПРОБЛЕМА 5](MCTS_INTEGRATION_GUIDE.md#проблема-5-старый-модуль-card_actions-больше-не-используется)

### В: MCTS не расширяется дальше Level::Base?
О: Проверьте:
1. Что `expandNode()` вызывается
2. Что `fillCardFlagsForAction()` заполняет флаги
3. Добавьте отладочный вывод

**Подробнее:** [MCTS_INTEGRATION_GUIDE.md § Если ошибки](MCTS_INTEGRATION_GUIDE.md#-если-ошибки)

### В: Как я добавляю новую карту?
О: Три шага:
1. Добавить тип в `enum Card`
2. Добавить флаг в `Action` (e.g., `canUseMyNewCard`)
3. Добавить проверку в `card_utils.cpp`

**Подробнее:** [ARCHITECTURE_DIAGRAM.md § Оптимизационные возможности](ARCHITECTURE_DIAGRAM.md#-оптимизационные-возможности)

---

## 🎯 КОНТРОЛЬНЫЙ СПИСОК

### Перед началом разработки:
- [ ] Прочитал [QUICKSTART.md](QUICKSTART.md)
- [ ] Понимаю 4 критичные функции для реализации
- [ ] Скомпилировал новые файлы без ошибок
- [ ] Запустил базовый тест (getBaseActions возвращает ~200 действий)

### Перед интеграцией MCTS:
- [ ] Реализовал `applyAction()`
- [ ] Реализовал `cloneState()`
- [ ] Реализовал `isGameOver()`
- [ ] Реализовал `evaluateState()`
- [ ] Протестировал на малой карте (1v1)

### Перед production:
- [ ] Интегрировал нейросеть для `simulate()`
- [ ] Протестировал на полной карте
- [ ] Запустил 1000 самоигр для обучения
- [ ] Генерирую тепловые карты

---

## 💬 СООБЩЕНИЕ РАЗРАБОТЧИКУ

Спасибо за то, что используете этот рефакторинг! 

Документация создана для того, чтобы вы:
1. 🎯 **Быстро поняли** что произошло
2. 🔧 **Легко реализовали** оставшиеся функции
3. 🚀 **Уверенно запустили** MCTS+NN систему

Если вопросы:
- Начните с [QUICKSTART.md](QUICKSTART.md)
- Затем смотрите релевантный FAQ выше
- Потом изучите полное описание в [REFACTORING.md](REFACTORING.md)

**Успехов в развитии AI! 🤖**

---

**Последнее обновление:** 26 июля 2026  
**Версия документации:** 1.0  
**Статус проекта:** Рефакторинг завершён, ожидается реализация `applyAction()` и др.

