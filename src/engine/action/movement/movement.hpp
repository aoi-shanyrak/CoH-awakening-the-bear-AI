#pragma once

#include "../../core/action_generation.hpp"


namespace Actions::Movement {


  void addMovementActionsForUnit(const GenerationContext& context, uint8_t unitIdx);


  namespace MovementImpl {


    void addPivotActionsForUnit(std::vector<Action>& actions, const Unit& unit, uint8_t unitIdx);


  }


}
