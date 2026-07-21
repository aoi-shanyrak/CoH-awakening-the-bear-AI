#include "movement.hpp"


using namespace Movement;
using namespace MovementImpl;


namespace Movement {


  std::vector<Action> getMovementActionsForUnit(const ActionGenerationContext& context, uint8_t unitIdx) {
    std::vector<Action> actions {};
    const Unit& unit {context.state.units[unitIdx]};
    const auto& map {context.state.map};

    for (uint8_t i {0}; i < static_cast<uint8_t>(Direction::NorthWest); ++i) {
      auto dir {static_cast<Direction>(i)};
      auto target_hex_idx {map->getNeighborIndex(map->toIndex(unit.getPosition()), dir)};
      if (!target_hex_idx) continue;

      auto check {static_cast<int8_t>(unit.getMoveAPcost() + (map->get(*target_hex_idx)).getFootPenaltyAP())};
      Action action {ActionType::Move, {check, {}}, unitIdx, *target_hex_idx, {}};
      actions.push_back(std::move(action));
    }

    actions += getPivotActionsForUnit(unit, unitIdx);

    return actions;
  }


  namespace MovementImpl {


    std::vector<Action> getPivotActionsForUnit(const Unit& unit, uint8_t unitIdx) {
      std::vector<Action> actions {};

      for (uint8_t i {0}; i < static_cast<uint8_t>(Direction::NorthWest); ++i) {
        auto dir {static_cast<Direction>(i)};
        if (dir == unit.getDirection()) continue;

        actions.push_back({ActionType::Move, unit.getPivotAPcost(), {}, unitIdx, dir, {}});
      }
      return actions;
    }


  }


}
