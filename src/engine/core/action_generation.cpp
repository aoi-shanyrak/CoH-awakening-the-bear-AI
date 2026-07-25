#include "action_generation.hpp"
#include "../action/cards/card_actions.hpp"
#include "../action/combat/combat.hpp"
#include "../action/movement/movement.hpp"
#include "../action/rally/rally.hpp"
#include "interface.hpp"


using namespace GameEngine;
using namespace Actions;


namespace GameEngine {


  std::vector<Action> getValidActions(const State& state) {
    std::vector<Action> actions {{ActionType::Pass}};

    auto units_in_hex {getUnitsInHexes(state)};
    GenerationContext context {state, units_in_hex, actions};
    for (uint8_t unitIdx {0}; unitIdx < state.units.size(); ++unitIdx) addActionsForUnit(context, unitIdx);

    Cards::addCardActions(context);

    removeActionsCantPay(context);

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
  if (unit.canStall()) context.actions.push_back({ActionType::Stall, {unit.getStallAPcost(), unit.isFresh()}, unitIdx});
}

void removeActionsCantPay(const GenerationContext& context) {
  const auto& player {(context.state.turn == Nation::SovietUnion) ? context.state.soviet : context.state.germany};
  auto max_CAPs_cost {player.getCAPs()};
  for (const auto& action : context.actions) {
  }
}


bool isAnyEnemyUnitInHex(const GenerationContext& context, HexCoord hex) {
  if (context.units_in_hex.find(hex) == context.units_in_hex.end()) return false;

  for (auto unitIdx : context.units_in_hex[hex]) {
    const Unit& unit {context.state.units[unitIdx]};
    if (unit.getNation() != context.state.turn) return true;
  }
  return false;
}
