#pragma once

#include <vector>

#include "../action.hpp"
#include "../core/state.hpp"
#include "../nations.hpp"


std::vector<Action> getValidCombatActions(const State& state, Nation player);
