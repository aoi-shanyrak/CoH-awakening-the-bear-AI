#include "rally.hpp"


namespace Rally {


  std::vector<Action> getRallyActionsForUnit(const ActionGenerationContext& context, uint8_t unitIdx) {
    std::vector<Action> actions {};
    const Unit& unit {context.state.units[unitIdx]};

    if (!unit.hasHitmarker() || isAnyEnemyUnitInHex(context, unit.getPosition())) return actions;

    auto rallyModifier {context.state.map->get(unit.getPosition()).getCoverModifier()};
    rallyModifier -= context.units_in_hex[unit.getPosition()].size() - 1;

    auto check {static_cast<int8_t>(unit.getRallyAPcost() + rallyModifier)};
    Action action {ActionType::Rally, {check, {}}, unitIdx, unit.getPosition(), {}};
    actions.push_back(action);

    return actions;
  }


}
