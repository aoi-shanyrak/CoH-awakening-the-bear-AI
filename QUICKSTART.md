# ⚡ QUICKSTART: Рефакторинг MCTS

Только самое важное. 2 минуты на чтение.

---

## ✅ ЧТО ГОТОВО

```cpp
// 1. Базовые действия (200-300)
std::vector<Actions::Action> actions = getBaseActions(state);

// 2. Каждое действие содержит флаги о модификаторах
// action.canUseAdrenaline, action.canSpendAdditionalCAPs и т.д.

// 3. MCTS иерархическое дерево
MCTS::MCTSSearch search(state);
search.runIteration();  // select → expand → simulate → backpropagate

// 4. Собираем полное действие из пути дерева
Actions::Action fullAction = MCTS::assembleFullAction(bestNode);
```

---

## ❌ ЧТО НЕ ГОТОВО (БЛОКИРУЕТ)

```cpp
// Эти функции НУЖНЫ, без них MCTS не работает:

State applyAction(const State& state, const Actions::Action& action);
// Применяет действие и возвращает новое состояние
// ОЧЕНЬ КРИТИЧНО!

bool isGameOver(const State& state);
// Определяет, закончилась ли игра

double evaluateState(const State& state);
// Оценивает позицию (-1 до +1, от -1 враг выигрывает до +1 мы выигрываем)

State cloneState(const State& state);
// Глубокое копирование для симуляции
```

---

## 📁 ГДЕ ЧТО

| Что | Файл |
|-----|------|
| Генерация базовых действий | `src/engine/core/action_generation.cpp` |
| Флаги действий | `src/engine/action/action.hpp` (+13 bool) |
| Проверка карт | `src/engine/action/cards/card_utils.cpp` |
| MCTS узлы и поиск | `src/engine/mcts/mcts.hpp` + `mcts.cpp` |
| Полные примеры | `MCTS_INTEGRATION_GUIDE.md` |
| Все детали | `REFACTORING.md` |

---

## 🎯 ПЛАН РЕАЛИЗАЦИИ (приоритет)

### Неделя 1: КРИТИЧНО
1. `applyAction()` – в `src/engine/core/interface.cpp`
2. `cloneState()` – в `src/engine/core/interface.cpp`
3. `isGameOver()` – в `src/engine/core/interface.cpp`

### Неделя 2: ВАЖНО
4. `evaluateState()` – в `src/engine/core/interface.cpp`
5. Полная `simulate()` – в `src/engine/mcts/mcts.cpp`
6. Тестирование MCTS на малой карте

### Неделя 3+: ОПТИМИЗАЦИЯ
7. Нейросеть (замена `simulate()`)
8. Самоигра
9. Тепловые карты

---

## 🚀 БЫСТРЫЙ ТЕСТ

```cpp
#include "engine/core/action_generation.hpp"

int main() {
  State state = createTestState();
  
  auto actions = getBaseActions(state);
  std::cout << "Базовых действий: " << actions.size() << std::endl;
  // Ожидаем: 100-300
  
  for (const auto& action : actions) {
    int flags = 0;
    if (action.canUseAdrenaline) flags++;
    if (action.canUseCommandAction) flags++;
    if (action.canSpendAdditionalCAPs) flags++;
    // ... и т.д.
    
    if (flags > 0) {
      std::cout << "✓ Действие имеет модификаторы: " << flags << std::endl;
      break;
    }
  }
  
  return 0;
}
```

Если скомпилируется и выведет `100-300` действий и `✓`, то архитектура работает!

---

## 📊 ЧЕКЛИСТ

- [ ] Скомпилировать проект с новыми файлами
- [ ] Проверить `getBaseActions()` возвращает ~200 действий
- [ ] Проверить флаги заполнены (`action.canUseAdrenaline` etc)
- [ ] Реализовать `applyAction()`
- [ ] Реализовать `cloneState()`
- [ ] Реализовать `isGameOver()`
- [ ] Запустить одну MCTS итерацию
- [ ] Проверить расширение узлов (Base → CAPs → Card)
- [ ] Собрать полное действие из листового узла
- [ ] Применить действие и получить новое состояние

---

## 💡 ПРИМЕЧАНИЯ

- **Карты больше НЕ добавляют действия** – они только добавляют флаги
- **MCTS выбирает иерархически** – сначала действие, потом CAPs, потом карта
- **Симуляция очень проста сейчас** – просто возвращает случайное число
  - Нужно реализовать полную разыгрывание игры до конца
- **Все 4 функции выше КРИТИЧНЫ** – без них не будет работать

---

## 🆘 ЕСЛИ ОШИБКИ

### Ошибка компиляции: `undefined reference to getBaseActions`
- Проверьте, что `getBaseActions` в `action_generation.cpp` и `getValidActions` вызывает его
- Убедитесь, что файл скомпилирован

### MCTS дерево не расширяется
- Проверьте `expandNode()` в `mcts.cpp`
- Добавьте отладочный вывод в `fillCardFlagsForAction()`

### `simulate()` всегда возвращает 0.5
- Это нормально! Это заглушка
- Нужно реализовать полную версию (см. MCTS_INTEGRATION_GUIDE.md)

---

## 📞 ВОПРОСЫ?

Смотри:
1. `REFACTORING.md` – полное описание
2. `MCTS_INTEGRATION_GUIDE.md` – примеры и проблемы
3. Инлайн комментарии в коде (тонны 📝)

Успехов! 🚀
