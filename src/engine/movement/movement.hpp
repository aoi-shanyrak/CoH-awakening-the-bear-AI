#pragma once

#include "../action.hpp"
#include "../core/state.hpp"


namespace Movement {


  std::vector<Action> getValidMovementActions(const State& state);


  namespace MovementImpl {


    std::vector<Action> getPivotActionsForUnit(const Unit& unit, int8_t unitIdx);
    std::vector<Action> getMovementActionsForUnit(const Map& map, const Unit& unit, int8_t unitIdx);


  }


}
