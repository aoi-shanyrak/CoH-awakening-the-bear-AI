#include "rally.hpp"


using namespace Actions;
using namespace ActionDetails;


namespace Actions::Rally {


  void addRallyActionsForUnit(const GenerationContext& context, uint8_t unitIdx) {
    const Unit& unit {context.state.units[unitIdx]};

    if (!unit.hasHitmarker() || isAnyEnemyUnitInHex(context, unit.getPosition())) return;

    auto rallyModifier {context.state.map->get(unit.getPosition()).getCoverModifier()};
    rallyModifier -= context.units_in_hex[unit.getPosition()].size() - 1;

    auto check {static_cast<int8_t>(unit.getRallyAPcost() + rallyModifier)};
    Action action {ActionType::Rally, {check, unit.isFresh()}, unitIdx, {}, *unit.getRallyNumber()};
    context.actions.push_back(action);
  }


}
