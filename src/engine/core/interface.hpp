#pragma once

#include <cstdint>
#include <vector>

#include "../action.hpp"
#include "../nations.hpp"
#include "state.hpp"


namespace GameEngine {


  std::vector<Action> getValidActions(const State& state, Nation player);

  State applyAction(const State& state, const Action& action);

  State cloneState(const State& state, Nation player);

  bool isTerminal(const State& state);

  int8_t getVPAdvantage(const State& state);


};
