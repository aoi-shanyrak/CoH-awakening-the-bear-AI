#pragma once

#include "../action/action.hpp"
#include "state.hpp"


namespace GameEngine {


  std::vector<Actions::Action> getValidActions(const State& state);

  State applyAction(const State& state, const Actions::Action& action);

  State cloneState(const State& state);


};
