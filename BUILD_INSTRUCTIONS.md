# 🔨 BUILD INSTRUCTIONS: Добавление новых файлов

Инструкции для подключения новых модулей к системе сборки.

---

## 📋 Новые файлы для компиляции

### Новый модуль: `card_utils`
```
src/engine/action/cards/card_utils.hpp
src/engine/action/cards/card_utils.cpp
```

### Новый модуль: `mcts`
```
src/engine/mcts/mcts.hpp
src/engine/mcts/mcts.cpp
```

### Изменённые файлы (только, но обновлены)
```
src/engine/action/action.hpp
src/engine/core/action_generation.hpp
src/engine/core/action_generation.cpp
```

---

## 🔧 CMake (если используется)

### Добавить в CMakeLists.txt

```cmake
# Найти все cpp файлы в engine
file(GLOB_RECURSE ENGINE_SOURCES
  src/engine/**/*.cpp
)

# Или явно добавить новые файлы:
set(ENGINE_SOURCES
  ${ENGINE_SOURCES}
  src/engine/action/cards/card_utils.cpp
  src/engine/mcts/mcts.cpp
)

# Если нужно исключить старый модуль card_actions:
# list(REMOVE_ITEM ENGINE_SOURCES src/engine/action/cards/card_actions.cpp)

# Компилировать
add_library(engine ${ENGINE_SOURCES})

# Линковать stdm для std::sqrt (если требуется)
target_link_libraries(engine PUBLIC m)
```

### Или если используется явный список:

```cmake
set(ENGINE_SOURCES
  src/engine/player.cpp
  src/engine/units/unit.cpp
  src/engine/field/map.cpp
  src/engine/field/hex.cpp
  src/engine/action/combat/combat.cpp
  src/engine/action/combat/firezone.cpp
  src/engine/action/movement/movement.cpp
  src/engine/action/rally/rally.cpp
  src/engine/action/cards/card_actions.cpp
  src/engine/action/cards/card_utils.cpp        # ✨ НОВОЕ
  src/engine/core/action_generation.cpp
  src/engine/checks/random_generator.cpp
  src/engine/markers/VPcounter.cpp
  src/engine/markers/control.cpp
  src/engine/mcts/mcts.cpp                      # ✨ НОВОЕ
)

add_library(engine ${ENGINE_SOURCES})
```

---

## 📝 Makefile (если используется)

### Добавить переменные

```makefile
# Новые объектные файлы
ENGINE_OBJS += build/card_utils.o
ENGINE_OBJS += build/mcts.o

# Правила компиляции
build/card_utils.o: src/engine/action/cards/card_utils.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/mcts.o: src/engine/mcts/mcts.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Цель, которая зависит от объектных файлов
engine_lib: $(ENGINE_OBJS)
	ar rcs build/libengine.a $(ENGINE_OBJS)
```

---

## 🔗 Включения в заголовки

### В интерфейс (если используется):
```cpp
// src/engine/core/interface.hpp или main.cpp

#include "engine/core/action_generation.hpp"  // getBaseActions()
#include "engine/action/cards/card_utils.hpp" // fillCardFlagsForAction()
#include "engine/mcts/mcts.hpp"                // MCTSSearch
```

### Проверка включений в cpp файлах:

**action_generation.cpp:**
```cpp
#include "action_generation.hpp"
#include "../action/cards/card_utils.hpp"  // ✅ должно быть
#include "../action/combat/combat.hpp"
#include "../action/movement/movement.hpp"
#include "../action/rally/rally.hpp"
#include "interface.hpp"
```

**mcts.cpp:**
```cpp
#include "mcts.hpp"
#include "../core/action_generation.hpp"  // ✅ должно быть
#include "../core/interface.hpp"           // ✅ должно быть
#include "../action/cards/card_utils.hpp" // ✅ должно быть
#include <algorithm>                       // ✅ для std::find, std::max
#include <cmath>                           // ✅ для std::sqrt, std::log
#include <random>                          // ✅ для mt19937
```

**card_utils.cpp:**
```cpp
#include "card_utils.hpp"
#include "../action.hpp"
#include "card.hpp"
#include <algorithm>  // ✅ для std::find
```

---

## ✅ Проверка компиляции

### Командная строка:

```bash
# Компилировать с предупреждениями
g++ -std=c++17 -Wall -Wextra -c src/engine/action/cards/card_utils.cpp
g++ -std=c++17 -Wall -Wextra -c src/engine/mcts/mcts.cpp
g++ -std=c++17 -Wall -Wextra -c src/engine/core/action_generation.cpp

# Или одной командой:
g++ -std=c++17 -Wall -Wextra -c src/engine/action/cards/card_utils.cpp \
                             -c src/engine/mcts/mcts.cpp \
                             -c src/engine/core/action_generation.cpp

# Удалить старые объектные файлы перед пересборкой:
rm -f build/*.o
```

### Проверка символов:

```bash
# Убедиться, что символы экспортированы:
nm build/card_utils.o | grep fillCardFlagsForAction
nm build/mcts.o | grep MCTSSearch

# Если ничего не выведется, значит функции не откомпилировались
```

---

## 🚨 Типичные ошибки при компиляции

### Ошибка 1: `undefined reference to getBaseActions`
```
/usr/bin/ld: undefined reference to `GameEngine::getBaseActions()'
```
**Решение:**
- Убедитесь, что `getBaseActions()` определена в `action_generation.cpp`
- Переименуйте `getValidActions()` на `getBaseActions()`
- Добавьте пустую реализацию `getValidActions()`, которая вызывает `getBaseActions()`

### Ошибка 2: `invalid use of incomplete type`
```
error: invalid use of incomplete type 'struct MCTSNode'
```
**Решение:**
- Убедитесь, что `#include "mcts.hpp"` есть в файле
- Проверьте, что `MCTSNode` определена в `mcts.hpp`, не в `mcts.cpp`

### Ошибка 3: `std::sqrt` не найден
```
error: 'sqrt' was not declared in this scope
```
**Решение:**
- Добавьте `#include <cmath>` в `mcts.cpp`
- Используйте `std::sqrt()` вместо просто `sqrt()`

### Ошибка 4: конфликт символов
```
error: multiple definition of 'fillCardFlagsForAction'
```
**Решение:**
- Функции в `.hpp` файлах должны быть `inline` ИЛИ в namespace
- Или переместите реализацию в `.cpp` файл

---

## 📦 Примерная структура build

```
build/
├── player.o
├── unit.o
├── map.o
├── hex.o
├── card_utils.o              ✨ НОВОЕ
├── mcts.o                    ✨ НОВОЕ
├── action_generation.o       (пересобрать)
├── combat.o
├── movement.o
├── rally.o
├── random_generator.o
├── VPcounter.o
├── control.o
└── libengine.a              (пересобрать)
```

---

## 🔗 Связывание (Linking)

### Если создаётся статическая библиотека:
```bash
ar rcs build/libengine.a build/player.o build/unit.o ... build/mcts.o

# Проверить содержимое:
ar t build/libengine.a | grep mcts
```

### Если создаётся исполняемый файл:
```bash
g++ -std=c++17 -o game main.cpp build/*.o -lm

# Или с явным линкованием mcts:
g++ -std=c++17 -o game main.cpp \
  build/player.o build/unit.o ... build/mcts.o -lm
```

---

## 🧪 Финальная проверка

```bash
# 1. Очистить старые файлы
rm -rf build/
mkdir build

# 2. Пересобрать всё
cmake -B build .    # если CMake
make -B              # если Make

# 3. Или вручную (если нет build системы):
cd src/engine
g++ -std=c++17 -c action/cards/card_utils.cpp -o ../../build/card_utils.o
g++ -std=c++17 -c mcts/mcts.cpp -o ../../build/mcts.o
# ... остальные файлы

# 4. Проверить что всё скомпилировалось
ls -la build/

# 5. Если требуется, линковать в единую библиотеку
ar rcs build/libengine.a build/*.o

# 6. Использовать в своём коде:
# g++ -std=c++17 -o my_program my_program.cpp build/libengine.a -lm
```

---

## 📚 Документация

- **CMake:** https://cmake.org/cmake/help/latest/
- **GNU Make:** https://www.gnu.org/software/make/manual/make.html
- **g++:** https://gcc.gnu.org/onlinedocs/

