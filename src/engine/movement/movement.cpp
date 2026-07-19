#include "movement.hpp"
#include <cstdint>


using namespace Movement::MovementImpl;


namespace Movement {


  std::vector<Action> getValidMovementActions(const State& state) {
    std::vector<Action> actions {};

    for (int8_t unitIdx {0}; unitIdx < state.units.size(); ++unitIdx) {
      const Unit& unit {state.units[unitIdx]};
      if (unit.getNation() != state.player || !unit.canMove()) continue;

      actions += getPivotActionsForUnit(unit, unitIdx);
      actions += getMovementActionsForUnit(*state.map, unit, unitIdx);
    }
    return actions;
  }


  namespace MovementImpl {


    std::vector<Action> getPivotActionsForUnit(const Unit& unit, int8_t unitIdx) {
      std::vector<Action> actions {};

      for (uint8_t i {0}; i < static_cast<uint8_t>(Direction::NorthWest); ++i) {
        auto cur {static_cast<Direction>(i)};
        if (cur == unit.getDirection()) continue;

        Action action {ActionType::Move, unitIdx, unit.getPivotAPcost(), unit.getPosition(), {}};
        actions.push_back(std::move(action));
      }
      return actions;
    }

    std::vector<Action> getMovementActionsForUnit(const Map& map, const Unit& unit, int8_t unitIdx) {
      std::vector<Action> actions {};

      for (uint8_t i {0}; i < static_cast<uint8_t>(Direction::NorthWest); ++i) {
        auto dir {static_cast<Direction>(i)};
        auto target_hex_idx {map.getNeighborIndex(map.toIndex(unit.getPosition()), dir)};
        if (!target_hex_idx) continue;

        int8_t check = static_cast<int8_t>(unit.getMoveAPCost() + (map.get(*target_hex_idx)).getFootPenaltyAP());
        Action action {ActionType::Move, unitIdx, check, *target_hex_idx, {}};
        actions.push_back(std::move(action));
      }
      return actions;
    }


  }


}
