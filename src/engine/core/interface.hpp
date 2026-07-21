#pragma once

#include "../action/action.hpp"
#include "state.hpp"


namespace GameEngine {


  std::vector<Action> getValidActions(const State& state);

  State applyAction(const State& state, const Action& action);

  State cloneState(const State& state);


};
