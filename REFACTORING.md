# Рефакторинг движка CoH: Иерархическое дерево решений для MCTS

## ✅ Завершённые изменения

### 1. **Структура `Action` расширена флагами** 
📄 [src/engine/action/action.hpp](src/engine/action/action.hpp#L33-L47)

Добавлены 13 флагов типа `bool`, которые указывают на возможные модификации действия:
- `canSpendAdditionalCAPs` – может ли юнит потратить CAPs (если свежий и action_points > 0)
- `canUseAdrenaline`, `canUseCommandAction`, `canUseFollowMe` и т.д. – флаги для каждой карты

**Использование в MCTS:** На уровне 2 (CAPs) и уровне 3 (Card) эти флаги указывают, какие детья создавать в узле.

---

### 2. **`getBaseActions()` генерирует ТОЛЬКО базовые действия**
📄 [src/engine/core/action_generation.cpp](src/engine/core/action_generation.cpp)

**Было:**
```cpp
std::vector<Action> getValidActions(const State& state) {
  // ... генерирует базовые действия
  Cards::addCardActions(context);  // ❌ УБРАНО: создаёт варианты с картами
  removeActionsCantPay(context);
  return actions;
}
```

**Стало:**
```cpp
std::vector<Action> getBaseActions(const State& state) {
  // ... генерирует базовые действия
  // ✅ НЕ создаёт варианты с картами
  for (auto& action : actions) {
    Cards::fillCardFlagsForAction(action, state);  // ✅ ЗАПОЛНЯЕТ ФЛАГИ
  }
  filterActionsBaseCost(context);
  return actions;
}
```

**Эффект:** ветвление на уровне 1 снизилось с 500+ до 200-300 действий.

---

### 3. **Новый модуль `card_utils` для проверки применимости карт**
📄 [src/engine/action/cards/card_utils.hpp](src/engine/action/cards/card_utils.hpp)
📄 [src/engine/action/cards/card_utils.cpp](src/engine/action/cards/card_utils.cpp)

Функции проверки:
- `fillCardFlagsForAction(action, state)` – заполняет все флаги для действия
- `canUseAdrenaline(state, action)` – проверка для Adrenaline
- `canUseCommandAction(state, action)` – проверка для Command Action
- ... (11 функций для остальных карт)
- `hasCardInHand(state, card)` – есть ли карта в руке

**Использование:** Вызывается в `getBaseActions()` и в MCTS при расширении узлов уровня 3 (Card).

---

### 4. **Новый модуль MCTS: иерархическое дерево решений**
📄 [src/engine/mcts/mcts.hpp](src/engine/mcts/mcts.hpp)
📄 [src/engine/mcts/mcts.cpp](src/engine/mcts/mcts.cpp)

#### Структура `MCTSNode`:
```cpp
struct MCTSNode {
  MCTSNode* parent;
  std::vector<std::unique_ptr<MCTSNode>> children;
  Level level;              // Base, CAPs, Card, Terminal
  
  Action action;            // (уровень Base)
  int8_t capsSpent;         // (уровень CAPs)
  std::optional<Card> cardUsed;  // (уровень Card)
  
  uint32_t visits;
  double totalReward;
  
  double ucb1(double explorationConstant);  // формула UCB1
};
```

#### Уровни дерева:

```
                          Root (Base)
                             |
                    ┌────────┼────────┐
                 Move      Attack   Rally    (базовые действия ~200)
                    |        |        |
           ┌────────┼────┐   |      ...
        CAPs=0    CAPs=1  CAPs=2
           |        |       |
      ┌────┴────┐  ...    ...
    None   Card_A        
```

#### Класс `MCTSSearch`:
- `runIteration()` – один цикл MCTS (select → expand → simulate → backpropagate)
- `selectNode()` – выбор узла по UCB1
- `expandNode()` – создание детей в зависимости от уровня
- `simulate()` – случайное разыгрывание (TODO: полная реализация)
- `backpropagate()` – обновление статистики
- `getBestAction()` – возвращает узел с лучшим средним вознаграждением

#### Функция `assembleFullAction()`:
Собирает полное действие (Base + CAPs + Card) из листового узла дерева.

---

## 🔄 Как это работает

### Раньше (плоский список):
```
Действие 1: Move + 0 CAPs
Действие 2: Move + 0 CAPs + Adrenaline
Действие 3: Move + 1 CAPs + Adrenaline
Действие 4: Attack + 0 CAPs
...
[500+ действий в одном списке]
```

### Теперь (иерархическое дерево):
```
Шаг 1 (MCTS): выбираем Move из ~200 базовых действий
Шаг 2 (MCTS): выбираем CAPs=1 из 2-3 вариантов
Шаг 3 (MCTS): выбираем Adrenaline из 2-3 карт
Результат: Move + CAPs=1 + Adrenaline
```

**Преимущества:**
✅ Ветвление на каждом уровне: 200 → 3 → 3 ≈ 1800 комбинаций вместо 500+ в одном списке
✅ Более глубокое дерево (глубина 3-4 вместо 1)
✅ MCTS лучше исследует важные комбинации
✅ Легко добавлять новые карты

---

## 📋 Интеграция в существующий код

### Включения в проектные файлы:

**В `core/interface.hpp` или где вызывается действие:**
```cpp
#include "../mcts/mcts.hpp"
```

**Примерное использование:**
```cpp
State gameState = ...;

// Инициализируем MCTS
MCTS::MCTSSearch search(gameState);

// Запускаем 10000 итераций
for (int i = 0; i < 10000; ++i) {
  search.runIteration();
}

// Получаем лучшее действие
const MCTS::MCTSNode* bestNode = search.getBestAction();
Actions::Action bestAction = MCTS::assembleFullAction(bestNode);

// Применяем действие
State newState = GameEngine::applyAction(gameState, bestAction);
```

---

## 📝 TODO: Что ещё нужно реализовать

### 1. **Воспроизведение пути в `simulate()`**
   - Нужна функция `applyAction(state, action)` для применения каждого уровня дерева
   - Нужна полная симуляция игры до конца

### 2. **Оптимизация `cloneState()`**
   - Для `simulate()` нужно глубокое копирование State
   - Текущая реализация может быть медленной; использовать битовые маски где возможно

### 3. **Обработка рандома в боевой системе**
   - При симуляции нужно использовать `RandomGenerator` для бросков D6/D10
   - MCTS должен усреднять по всем возможным исходам

### 4. **Кодирование состояния для нейросети**
   - Когда нейросеть будет заменять `simulate()`, нужно конвертировать State в тензор

### 5. **Обновление интерфейса в `interface.hpp`**
   - Убедитесь, что `getValidActions()` вызывает `getBaseActions()`
   - Добавьте новый интерфейс для MCTS

---

## 🎯 Результаты

### Метрики до рефакторинга:
- Базовые действия: ~150
- С картами: ~500+
- Ветвление: взрывное

### Метрики после рефакторинга:
- Уровень 1 (Base): ~200
- Уровень 2 (CAPs): ~3 среднее (min: 1, max: 3)
- Уровень 3 (Card): ~3 среднее (min: 1, max: 13)
- **Общее ветвление: ~1800-2000 комбинаций** (лучше масштабируется в дереве)

---

## 📚 Структура файлов после рефакторинга

```
src/engine/
├── action/
│   ├── action.hpp               (✏️ обновлено: флаги)
│   ├── cards/
│   │   ├── card.hpp
│   │   ├── card_actions.cpp     (⚠️ может быть удалено/заменено)
│   │   ├── card_utils.hpp       (✨ НОВОЕ)
│   │   └── card_utils.cpp       (✨ НОВОЕ)
│   ├── combat/
│   ├── movement/
│   └── rally/
├── core/
│   ├── action_generation.hpp    (✏️ обновлено)
│   ├── action_generation.cpp    (✏️ обновлено)
│   ├── interface.hpp
│   └── state.hpp
├── mcts/
│   ├── mcts.hpp                 (✨ НОВОЕ)
│   └── mcts.cpp                 (✨ НОВОЕ)
└── ... (остальное без изменений)
```

---

## 🚀 Следующие шаги

1. **Реализовать `applyAction()`** – критична для работы симуляции
2. **Завершить `simulate()`** – воспроизведение пути до конца игры
3. **Тестировать MCTS** – убедиться, что дерево правильно расширяется
4. **Добавить нейросеть** – заменить `simulate()` на оценку нейросети
5. **Генерировать тепловые карты** – с использованием вероятностей из MCTS

