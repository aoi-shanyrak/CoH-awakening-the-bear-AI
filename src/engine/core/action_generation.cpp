#include "action_generation.hpp"
#include "../action/combat/combat.hpp"
#include "../action/movement/movement.hpp"
#include "../action/rally/rally.hpp"
#include "interface.hpp"


using namespace GameEngine;


namespace GameEngine {


  std::vector<Action> getValidActions(const State& state) {
    std::vector<Action> actions {{ActionType::Pass, {}, {}, {}, {}}};

    auto units_in_hex {getUnitsInHexes(state)};
    for (uint8_t unitIdx {0}; unitIdx < state.units.size(); ++unitIdx) {
      ActionGenerationContext context {state, units_in_hex};
      actions += generateActionsForUnit(context, unitIdx);
    }

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

bool isAnyEnemyUnitInHex(const ActionGenerationContext& context, HexCoord hex) {
  if (context.units_in_hex.find(hex) == context.units_in_hex.end()) return false;

  for (auto unitIdx : context.units_in_hex[hex]) {
    const Unit& unit {context.state.units[unitIdx]};
    if (unit.getNation() != context.state.turn) return true;
  }
  return false;
}


std::vector<Action> generateActionsForUnit(const ActionGenerationContext& context, uint8_t unitIdx) {
  std::vector<Action> actions {};
  const Unit& unit {context.state.units[unitIdx]};
  if (unit.getNation() != context.state.turn) return actions;

  if (unit.canMove()) actions += Movement::getMovementActionsForUnit(context, unitIdx);
  if (unit.canAttack()) actions += Combat::getCombatActionsForUnit(context, unitIdx);
  if (unit.canRally()) actions += Rally::getRallyActionsForUnit(context, unitIdx);
  if (unit.canStall()) actions.push_back({ActionType::Stall, {unit.getStallAPcost(), {}}, unitIdx, {}, {}});

  return actions;
}
