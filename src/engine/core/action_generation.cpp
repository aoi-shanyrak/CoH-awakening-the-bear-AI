#include "action_generation.hpp"
#include "../action/cards/card_utils.hpp"
#include "../action/combat/combat.hpp"
#include "../action/movement/movement.hpp"
#include "../action/rally/rally.hpp"
#include "interface.hpp"


using namespace GameEngine;
using namespace Actions;


namespace GameEngine {


  std::vector<Action> getBaseActions(const State& state) {
    std::vector<Action> actions {};
    actions.push_back({ActionType::Pass});

    auto units_in_hex {getUnitsInHexes(state)};
    GenerationContext context {state, units_in_hex, actions};

    for (uint8_t unitIdx {0}; unitIdx < state.units.size(); ++unitIdx) {
      addActionsForUnit(context, unitIdx);
    }

    // Заполняем флаги для каждого действия, указывающие на возможные модификации
    for (auto& action : actions) {
      Cards::fillCardFlagsForAction(action, state);
    }
    // Фильтруем действия, которые нельзя оплатить базовой стоимостью
    filterActionsBaseCost(context);

    return actions;
  }


}


HexUnitsMap getUnitsInHexes(const State& state) {
  HexUnitsMap units_in_hex {};
  for (uint8_t i {0}; i < state.units.size(); ++i) {
    const auto& unit {state.units[i]};
    units_in_hex[unit.getPosition()].push_back(i);
  }
  return units_in_hex;
}


void addActionsForUnit(const GenerationContext& context, uint8_t unitIdx) {
  const Unit& unit {context.state.units[unitIdx]};
  if (unit.getNation() != context.state.turn) return;

  if (unit.canMove()) Movement::addMovementActionsForUnit(context, unitIdx);
  if (unit.canAttack()) Combat::addCombatActionsForUnit(context, unitIdx);
  if (unit.canRally()) Rally::addRallyActionsForUnit(context, unitIdx);
  if (unit.canStall()) {
    context.actions.push_back({ActionType::Stall, {unit.getStallAPcost(), unit.isFresh()}, unitIdx});
  }
}


void filterActionsBaseCost(const GenerationContext& context) {
  // Фильтруем только базовую стоимость (без учёта CAPs)
  // Действия, у которых базовая стоимость слишком высока, удаляются
  // ПРИМЕЧАНИЕ: Более сложная фильтрация по CAPs произойдёт на уровне MCTS

  const auto& player {(context.state.turn == Nation::SovietUnion) ? context.state.soviet : context.state.germany};

  // Пока остаём консервативны: проверяем, что Pass всегда валиден
  // остальные действия проверяются на предмет "можем ли мы хотя бы спусковой механизм потянуть"
  // (это будет более подробно реализовано в MCTS)
}


bool isAnyEnemyUnitInHex(const GenerationContext& context, HexCoord hex) {
  if (context.units_in_hex.find(hex) == context.units_in_hex.end()) return false;

  for (auto unitIdx : context.units_in_hex[hex]) {
    const Unit& unit {context.state.units[unitIdx]};
    if (unit.getNation() != context.state.turn) return true;
  }
  return false;
}
