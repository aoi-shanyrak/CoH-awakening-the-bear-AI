# MCTS Integration Guide: Практические примеры и проблемы

## 📖 Как использовать новую архитектуру

### Пример 1: Базовое использование MCTS для выбора действия

```cpp
#include "engine/mcts/mcts.hpp"
#include "engine/core/interface.hpp"

// Инициализация
State currentState = ...;  // текущее состояние игры

MCTS::MCTSSearch search(currentState);

// Запускаем поиск на 5000 итераций
for (int i = 0; i < 5000; ++i) {
  search.runIteration();
}

// Выбираем лучшее действие
const MCTS::MCTSNode* bestNode = search.getBestAction();
Actions::Action chosenAction = MCTS::assembleFullAction(bestNode);

// Применяем (когда applyAction будет реализована)
State nextState = GameEngine::applyAction(currentState, chosenAction);
```

---

### Пример 2: Анализ расширения дерева

```cpp
// После выполнения поиска
auto rootNode = search.getRootNode();

std::cout << "Корневых действий (уровень Base): " << rootNode->children.size() << std::endl;

for (const auto& baseChild : rootNode->children) {
  std::cout << "  Действие: " << static_cast<int>(baseChild->action.type);
  std::cout << " | CAPs-детей: " << baseChild->children.size() << std::endl;
  
  for (const auto& capsChild : baseChild->children) {
    std::cout << "    CAPs=" << static_cast<int>(capsChild->capsSpent);
    std::cout << " | Card-детей: " << capsChild->children.size() << std::endl;
  }
}

std::cout << "Всего посещений: " << search.getTotalVisits() << std::endl;
std::cout << "Среднее вознаграждение корня: " << search.getAverageRootReward() << std::endl;
```

---

## ⚠️ Текущие проблемы и их решения

### ПРОБЛЕМА 1: `simulate()` возвращает случайное значение

**Где:** `src/engine/mcts/mcts.cpp`, функция `MCTSSearch::simulate()`

**Текущий код:**
```cpp
double MCTSSearch::simulate(const State& state) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<> dis(0.0, 1.0);
  return dis(gen);  // ❌ Просто случайное число
}
```

**Что нужно:**
```cpp
double MCTSSearch::simulate(const State& state) {
  // 1. Копируем состояние
  State simState = state;
  
  // 2. Разыгрываем игру до конца (или до определённой глубины)
  int depth = 0;
  const int MAX_DEPTH = 50;  // или до конца игры
  
  while (!isGameOver(simState) && depth < MAX_DEPTH) {
    // Получаем базовые действия
    auto actions = getBaseActions(simState);
    if (actions.empty()) break;
    
    // Выбираем случайное действие
    int idx = rand() % actions.size();
    
    // Применяем действие
    simState = applyAction(simState, actions[idx]);
    depth++;
  }
  
  // 3. Оцениваем итоговое состояние
  double reward = evaluateState(simState);  // -1 до +1
  return reward;
}
```

**Зависит от:**
- ✅ `getBaseActions()` – уже реализована
- ❌ `applyAction()` – НУЖНО РЕАЛИЗОВАТЬ
- ❌ `isGameOver()` – НУЖНО РЕАЛИЗОВАТЬ
- ❌ `evaluateState()` – НУЖНО РЕАЛИЗОВАТЬ (оценка по VP и позициям)

---

### ПРОБЛЕМА 2: Воспроизведение пути в дереве

**Что происходит сейчас:**
```cpp
MCTSNode* MCTSSearch::selectNode(MCTSNode* node) {
  // Спускаемся вниз по дереву
  while (node->isExpanded()) {
    MCTSNode* child = selectUCB1Child(node);
    if (!child) return node;
    node = child;  // ✅ Выбираем ребёнка
  }
  return node;
}

void MCTSSearch::expandNode(MCTSNode* node, const State& state) {
  // ❌ ПРОБЛЕМА: state параметр – это ROOT состояние, не состояние в узле!
  // Нам нужно применить все действия от корня до node
}
```

**Решение:**
```cpp
struct MCTSNode {
  // ... остальные поля ...
  
  // Новое поле: путь от корня (последовательность примененных действий)
  std::vector<Actions::Action> pathFromRoot;
};

State MCTSSearch::getStateAtNode(const MCTSNode* node) const {
  State state = rootState;
  
  // Применяем все действия по пути от корня к узлу
  for (const auto& action : node->pathFromRoot) {
    state = GameEngine::applyAction(state, action);
  }
  
  return state;
}
```

**Или более сложное:**
```cpp
MCTSNode* MCTSSearch::selectNode(MCTSNode* node, State& stateAtNode) {
  stateAtNode = rootState;  // начинаем с корня
  
  while (node->isExpanded()) {
    MCTSNode* child = selectUCB1Child(node);
    if (!child) return node;
    
    // Применяем действие для корректного состояния
    stateAtNode = GameEngine::applyAction(stateAtNode, child->action);
    node = child;
  }
  
  return node;
}
```

---

### ПРОБЛЕМА 3: Пересчёт UCB1 с нулевым родителем

**Ошибка в коде:**
```cpp
double MCTSNode::ucb1(double explorationConstant) const {
  if (visits == 0) return std::numeric_limits<double>::infinity();
  
  double exploitation = getAverageReward();
  double exploration = explorationConstant * std::sqrt(std::log(parent->visits) / visits);
  // ❌ Если parent == nullptr, это crash!
  
  return exploitation + exploration;
}
```

**Исправление:**
```cpp
double MCTSNode::ucb1(double explorationConstant) const {
  if (visits == 0) return std::numeric_limits<double>::infinity();
  
  double exploitation = getAverageReward();
  
  if (!parent) {
    return exploitation;  // Корневой узел: только exploitation
  }
  
  double exploration = explorationConstant * std::sqrt(std::log(parent->visits) / visits);
  return exploitation + exploration;
}
```

---

### ПРОБЛЕМА 4: Включение заголовков

**Проблема:** `card_utils.cpp` использует `std::find`, но нет `#include <algorithm>`

**Решение:** В начало `card_utils.cpp` добавить:
```cpp
#include <algorithm>
```

---

### ПРОБЛЕМА 5: Старый модуль `card_actions` больше не используется?

**Текущая ситуация:**
- `getBaseActions()` больше не вызывает `Cards::addCardActions()`
- Старый модуль `card_actions.cpp` остаётся без использования

**Рекомендация:** 
- Можно оставить как историческую справку (но убрать из компиляции)
- Или удалить, если нет других зависимостей
- Новая функциональность полностью в `card_utils`

---

## 🔧 Необходимые функции для реализации

### 1. `applyAction()` – КРИТИЧНА
```cpp
State GameEngine::applyAction(const State& state, const Actions::Action& action);
```
**Что должна делать:**
- Применить действие (Move, Attack, Rally, Stall, Pass)
- Потратить AP/CAPs
- Если карта: использовать карту и применить эффект
- Обновить состояние юнитов
- Вернуть новое State

---

### 2. `isGameOver()` – для `simulate()`
```cpp
bool GameEngine::isGameOver(const State& state);
```
**Условия:**
- Один из игроков потерял все юниты?
- Достигнут лимит раундов?
- Один из игроков набрал достаточно VP?

---

### 3. `evaluateState()` – оценка позиции для reward
```cpp
double GameEngine::evaluateState(const State& state, Nation perspective);
```
**Возвращает:** значение позиции от -1 до +1 (мат. ожидание выигрыша)

**Факторы:**
- VP счёт (главный фактор)
- Количество и боеспособность юнитов
- Контроль территории
- Материальное преимущество

---

### 4. `cloneState()` – глубокое копирование
```cpp
State GameEngine::cloneState(const State& state);
```
**Нужно скопировать:**
- Все юниты с их состоянием
- Данные игроков (CAPs, hand, limit)
- RNG (или пересеять новым)
- Карта (если она внутри State)
- Контрольные маркеры
- VP счётчик

---

## 📊 Ожидаемые размеры дерева

При средних условиях игры (10 юнитов, 5 возможных действий на юнит):

| Уровень | Размер | Комментарий |
|---------|--------|-----------|
| Base    | 150-300 | Move/Attack/Rally/Stall per unit + Pass |
| CAPs    | 2-3 на узел | 0, 1, 2 CAPs (или меньше если нет) |
| Card    | 2-4 на узел | "No card" + 1-3 применимые карты |
| Итого   | ~1200-3600 | Намного лучше чем 500+ в одном списке |

---

## 🎯 Тестирование

### Простой тест расширения дерева

```cpp
#include "engine/core/action_generation.hpp"
#include "engine/mcts/mcts.hpp"

void testMCTSExpansion() {
  // Инициализируем тестовое состояние
  State testState = ...;  // загружаем начальное состояние
  
  MCTS::MCTSSearch search(testState);
  
  // Запускаем одну итерацию
  search.runIteration();
  
  auto root = search.getRootNode();
  
  // Проверяем расширение уровня Base
  assert(!root->children.empty());
  std::cout << "✓ Уровень Base расширился: " << root->children.size() << " детей\n";
  
  // Проверяем расширение уровня CAPs
  for (auto& baseChild : root->children) {
    if (!baseChild->children.empty()) {
      assert(baseChild->level == MCTS::Level::Base);
      std::cout << "✓ CAPs узел расширился: " << baseChild->children.size() << " вариантов\n";
      break;
    }
  }
  
  // Проверяем расширение уровня Card
  for (auto& baseChild : root->children) {
    for (auto& capsChild : baseChild->children) {
      if (!capsChild->children.empty()) {
        assert(capsChild->level == MCTS::Level::CAPs);
        std::cout << "✓ Card узел расширился: " << capsChild->children.size() << " карт\n";
        return;
      }
    }
  }
}
```

---

## 🚀 Оптимизации (будущее)

1. **Параллельный MCTS** – использовать несколько потоков для расширения дерева
2. **Кэширование состояний** – чтобы не пересчитывать state для каждого узла
3. **Bitboard State** – компактное представление позиций вместо vectorов
4. **Neural Network Pruning** – удалять низкоценные ветви раньше

