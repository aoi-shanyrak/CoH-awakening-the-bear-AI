# 🏗️ АРХИТЕКТУРНАЯ ДИАГРАММА: Иерархическое дерево решений

---

## 📊 Полный поток MCTS

```
┌────────────────────────────────────────────────────────────────┐
│  State (текущее состояние игры: юниты, CAPs, карты и т.д.)   │
└────────────────────────────────────────────────────────────────┘
                            ↓
┌────────────────────────────────────────────────────────────────┐
│  getBaseActions(state) → ≈200 базовых действий                │
│  (Move, Attack, Rally, Stall, Pass)                           │
│  Каждое с флагами: canUseAdrenaline, canSpendCAPs и т.д.      │
└────────────────────────────────────────────────────────────────┘
                            ↓
┌────────────────────────────────────────────────────────────────┐
│  MCTSSearch mcts(state)                                        │
│  ┌─────────────────────────────────────────────────────┐       │
│  │ root = MCTSNode(Level::Base)                        │       │
│  │  visits=0, totalReward=0                           │       │
│  └─────────────────────────────────────────────────────┘       │
└────────────────────────────────────────────────────────────────┘
                            ↓
          [ runIteration() × 5000 раз ]
                            ↓
    ┌─────────────────────────────────────────────┐
    │  ИТЕРАЦИЯ MCTS (Select → Expand → Sim → BP) │
    └─────────────────────────────────────────────┘
```

---

## 🌳 Структура дерева (4 уровня)

```
                        ROOT
                    (Level::Base)
                          │
        ┌─────────────────┼─────────────────┐
        │                 │                 │
      Move             Attack             Rally
    (Юнит 0)         (Юнит 1)           (Юнит 2)
      visits=500     visits=300          visits=200
    reward=0.55     reward=0.42          reward=0.35
        │                 │                 │
        ├─CAPs=0       ├─CAPs=0          ├─CAPs=0
        ├─CAPs=1       ├─CAPs=1          └─CAPs=1
        └─CAPs=2       └─CAPs=2
        (Level::CAPs)  (Level::CAPs)     (Level::CAPs)
        
Каждый CAPs-узел:
        │
        ├─ No Card (Level::Terminal)
        ├─ Adrenaline (Level::Terminal)
        ├─ CommandAction (Level::Terminal)
        └─ ... другие карты
        (Level::Card)
```

---

## 📋 Иерархия уровней и расширения

### Уровень 1: Base (базовые действия)
```
getBaseActions() выполняется ОДН РАЗ перед MCTS

Входе (State):
  - Юниты, их позиции, состояние
  - Текущий игрок
  - Доступные CAPs

Выход (≈200 действий):
  - Move (для каждого юнита)
  - Attack (для каждого юнита со способностью)
  - Rally (для раненых юнитов)
  - Stall (для каждого юнита)
  - Pass (одно)

Флаги в каждом Action:
  - canSpendAdditionalCAPs: true если action_points > 0 и юнит Fresh
  - canUseAdrenaline: true если юнит Spent
  - canUseCommandAction: true если карта в руке
  - ... ещё 10 карт
```

### Уровень 2: CAPs (дополнительные затраты)
```
Создаётся при расширении узла Level::Base

expandNode(baseNode, state):
  if (baseNode.action.canSpendAdditionalCAPs) {
    for capsSpent in [0, 1, 2]:
      if (capsSpent <= playerCAPs):
        создать дочерний узел Level::CAPs
  }
  
Расширение происходит ЛЕ́НИ́ВО при первом посещении

Варианты:
  - CAPs=0 (нет доп затрат)
  - CAPs=1 (потратить 1 дополнительный САП)
  - CAPs=2 (потратить 2 доп САПа)

Если нельзя тратить CAPs:
  - Только CAPs=0
```

### Уровень 3: Card (использование карт)
```
Создаётся при расширении узла Level::CAPs

expandNode(capsNode, state):
  applicableCards = []
  if (capsNode.action.canUseAdrenaline):
    applicableCards.push(Card::Adrenaline)
  if (capsNode.action.canUseCommandAction):
    applicableCards.push(Card::CommandAction)
  ... остальные

Расширение:
  - Всегда создаём "No Card" (Level::Terminal)
  - Для каждой применимой карты создаём (Level::Terminal)

Результат: 1-13 детей (в среднем 2-4)
```

### Уровень 4: Terminal (готовое действие)
```
Листовой узел = полное, готовое действие

Содержит:
  - baseAction (Move, Attack и т.д.)
  - capsSpent (0, 1, 2)
  - cardUsed (std::nullopt или конкретная карта)

MCTS не расширяет дальше этого уровня

После достижения Terminal:
  - Вызываем simulate() для получения reward
  - Обновляем статистику путём backpropagation
```

---

## 🔄 Цикл MCTS (одна итерация)

```
runIteration() {
  1. SELECT (выбор по UCB1 вниз):
     node = selectNode(root)  // спускаемся вниз по дереву
     
  2. EXPAND (создание детей если нужно):
     if (!node.isExpanded) {
       expandNode(node, state)  // создаём детей Level+1
     }
     
  3. SIMULATE (разыгрывание):
     if (node.children.empty()) {
       reward = simulate(state)  // случайное разыгрывание
     } else {
       node = node.children[0]   // берём первого дитя
       reward = simulate(state)  // разыгрываем от него
     }
     
  4. BACKPROPAGATE (обновление статистики):
     backpropagate(node, reward) {
       while (node) {
         node.visits++
         node.totalReward += reward
         node = node.parent
       }
     }
}
```

---

## 🎲 Выбор узла (UCB1 формула)

```
UCB1 = exploitation + exploration
     = mean_reward + C * sqrt(ln(parent_visits) / visits)

Где:
  - mean_reward = totalReward / visits (среднее вознаграждение)
  - C ≈ 1.414 (sqrt(2), баланс)
  - parent_visits = сколько раз родитель посещали
  - visits = сколько раз этот узел посещали

Интерпретация:
  - Если узел не посещали (visits=0) → UCB1 = ∞ (всегда выбирается)
  - Узлы с высокой средней наградой выбираются чаще
  - Узлы с низким посещением выбираются (investigation)
  - Баланс exploration/exploitation через C
```

---

## 📈 Примерные числа роста дерева

### Начало (1 итерация):
```
Посещено:
  - 1 Base узел (root)
  - 5-10 CAPs узлов (в среднем 3)
  - 1-3 Card узла
  - 1 Terminal узел
= 8-17 узлов в памяти

Статистика обновлена:
  - Root: visits=1
  - Base детей: каждый visits=1 (если выбран)
  - ... вниз до Terminal
```

### После 100 итераций:
```
Структура дерева:
  - Root: visits=100
  - Base узлы: 5-20 (в среднем 10)
  - CAPs узлы: 20-50 (3 per Base)
  - Card узлы: 30-100 (2-4 per CAPs)
  - Terminal узлы: 30-100 (каждый хотя бы раз посещён)
  
Всего узлов: ~100-200
Память: ~1-5 МБ (если каждый узел ≈ 200 байт)
```

### После 5000 итераций (типичный поиск):
```
Дерево почти полностью развито:
  - Root: visits=5000
  - Base узлы: 200 (все)
  - CAPs узлы: 400-600 (средний 2.5 per Base)
  - Card узлы: 1000-2000 (средний 2.5 per CAPs)
  - Terminal узлы: 1000-2000 (все достижимые)
  
Всего узлов: ~2000-3000
Память: ~5-15 МБ
Время поиска: ~100-300 мс (зависит от evaluate)
```

---

## 🔍 Пример полного пути от Root к Terminal

```
Итерация 347:

SELECT:
  root (visits=346, reward≈0.4)
  ├─ selectUCB1Child(root)
  └─ выбран Move_Unit3
       (visits=45, reward≈0.55)

EXPAND:
  Move_Unit3 не расширен
  expandNode(Move_Unit3, Level::Base → Level::CAPs)
  ├─ создан CAPs=0 (visits=0)
  ├─ создан CAPs=1 (visits=0)
  └─ создан CAPs=2 (visits=0)

SELECT (продолжение):
  CAPs=1 имеет visits=0 → UCB1=∞ → выбран

EXPAND:
  CAPs=1 не расширен
  expandNode(CAPs=1, Level::CAPs → Level::Card)
  ├─ создан NoCard (Level::Terminal)
  ├─ создан Adrenaline (Level::Terminal)
  └─ создан CommandAction (Level::Terminal)

SELECT (продолжение):
  Adrenaline имеет visits=0 → UCB1=∞ → выбран

SIMULATE:
  simulate(state) → разыгрываем игру
  → return 0.8 (хорошая позиция)

BACKPROPAGATE:
  Adrenaline: visits=1, totalReward=0.8
  CAPs=1: visits=1, totalReward=0.8
  Move_Unit3: visits=46, totalReward=25.1 (старые) + 0.8 = 25.9
  root: visits=347, totalReward≈137.5
```

---

## 🎯 Стратегия расширения

### Ленивое расширение (текущая):
```
Узел расширяется только при посещении
Преимущество: меньше памяти в начале
Недостаток: медленнее в конце (создаём узлы по одному)
```

### Полное расширение (альтернатива):
```
Root сразу создаёт всех 200 Base детей
Каждый Base сразу создаёт 2-3 CAPs детей
Каждый CAPs сразу создаёт 2-4 Card детей
Результат: ~2000-3000 узлов в памяти сразу

Преимущество: быстрее исследовать
Недостаток: много памяти потрачено впустую
```

### Рекомендация:
⭐ Текущая ленивая стратегия оптимальна для MCTS

---

## 📊 Анализ разветвления

### ДО рефакторинга (плоский список):
```
getValidActions() → 500+ действий в одном списке
├─ Move (50)
├─ Attack (150)
├─ Rally (80)
├─ Stall (20)
├─ Pass (1)
├─ Move + Adrenaline (40)
├─ Move + CommandAction (45)
├─ ... все комбинации
└─ Card combinations (100+)

Ветвление: 1 уровень, 500 вариантов
MCTS эффективность: низкая (слишком много выборов)
```

### ПОСЛЕ рефакторинга (иерархическое дерево):
```
Level 1 (Base): 200 действий
├─ Move, Attack, Rally, Stall, Pass

Level 2 (CAPs): 2-3 варианта каждый
├─ 0, 1, 2 дополнительных САПа

Level 3 (Card): 2-4 варианта каждый
├─ NoCard, Adrenaline, CommandAction, ...

Level 4 (Terminal): готовое действие

Разветвление: 200 → 3 → 3 → 1 = 1800-3600 комбинаций
MCTS эффективность: ВЫСОКАЯ (глубокое дерево, меньше выборов на уровень)
```

---

## 🧮 Стоимость операций (примерная)

| Операция | Время | Память |
|----------|-------|--------|
| `getBaseActions()` | 5-10 мс | 10-50 КБ (временно) |
| `fillCardFlagsForAction()` | 0.1 мс / действие | 0 (инлайн) |
| `expandNode()` Base | 1 мс | 50 КБ (200 узлов) |
| `expandNode()` CAPs | 0.1 мс | 5 КБ (3 узла) |
| `expandNode()` Card | 0.2 мс | 10 КБ (3-4 узла) |
| `simulate()` (случайная) | 0.1 мс | 0 (в памяти) |
| `simulate()` (полная игра) | 100-300 мс | 10-50 МБ |
| `backpropagate()` | 0.01 мс | 0 |
| Одна MCTS итерация | **0.3-0.5 мс** (без simulate) | ~150 байт |
| 5000 итераций | **1.5-2.5 сек** | ~15 МБ (дерево) |

---

## 💡 Оптимизационные возможности

```cpp
// 1. Кэш состояний на пути:
struct PathCache {
  std::vector<State> states;  // кэш State для каждого узла
  // Экономит пересчёты при simulate()
};

// 2. Параллельный MCTS:
std::thread threads[8];
for (int i = 0; i < 8; ++i) {
  threads[i] = std::thread([&]() {
    for (int j = 0; j < 625; ++j) {
      mcts.runIteration();  // каждый поток делает 625 итераций
    }
  });
}

// 3. Битовое представление State:
struct StateBitboard {
  uint64_t positions[32];      // позиции юнитов
  uint8_t states[32];          // состояния юнитов (скомпактировано)
  uint8_t caps[2];             // CAPs каждого игрока
  // Намного меньше памяти чем vector<Unit>
};
```

