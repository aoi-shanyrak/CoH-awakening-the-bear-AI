#include "../combat/combat.hpp"
#include "interface.hpp"


using namespace GameEngine;


std::vector<Action> GameEngine::getValidActions(const State& state, Nation player) {
  std::vector<Action> validActions {{ActionType::Pass, std::nullopt, std::nullopt}};

  std::vector<Action> combatActions {getValidCombatActions(state, player)};
  validActions.insert(validActions.end(), combatActions.begin(), combatActions.end());

  return validActions;
}
