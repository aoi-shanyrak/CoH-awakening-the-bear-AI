#pragma once

#include "../../core/action_generation.hpp"


namespace Movement {


  std::vector<Action> getMovementActionsForUnit(const ActionGenerationContext& context, uint8_t unitIdx);


  namespace MovementImpl {


    std::vector<Action> getPivotActionsForUnit(const Unit& unit, uint8_t unitIdx);


  }


}
