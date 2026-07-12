#include "combat.hpp"


std::vector<Action> getValidCombatActions(const State& state, Nation player) {
  for (const auto& unit : state.units) {
    if (unit.nation == player) {
    }
  }
}
