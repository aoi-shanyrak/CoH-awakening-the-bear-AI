#pragma once

#include "../../core/action_generation.hpp"


namespace Rally {


  std::vector<Action> getRallyActionsForUnit(const ActionGenerationContext& context, uint8_t unitIdx);


}
