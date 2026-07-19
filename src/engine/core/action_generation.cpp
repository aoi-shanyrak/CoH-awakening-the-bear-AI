#include "../combat/combat.hpp"
#include "../movement/movement.hpp"
#include "interface.hpp"


using namespace GameEngine;
using namespace Combat;
using namespace Movement;


std::vector<Action> GameEngine::getValidActions(const State& state) {
  std::vector<Action> actions {{ActionType::Pass, -1, 0, -1, {}}};

  actions += getValidCombatActions(state);
  actions += getValidMovementActions(state);

  return actions;
}
