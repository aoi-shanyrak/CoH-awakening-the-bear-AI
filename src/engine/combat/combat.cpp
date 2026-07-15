#include <algorithm>

#include "combat.hpp"


using namespace Firezone;
using namespace Combat;
using namespace CombatImpl;


std::vector<Action> getValidCombatActions(const State& state, Nation player) {
  std::vector<Action> actions {};

  std::map<HexCoord, std::vector<int8_t>> units_in_hex {};
  for (int8_t i {0}; i < state.units.size(); ++i) {
    const auto& unit {state.units[i]};
    units_in_hex[unit.getPosition()].push_back(i);
  }
  removeHexesWithoutEnemyUnits(units_in_hex, state);

  for (int8_t attackerIdx {0}; attackerIdx < state.units.size(); ++attackerIdx) {
    const Unit& attacker {state.units[attackerIdx]};
    if (attacker.getNation() != player || !attacker.canAttack()) continue;

    for (const auto& [hex, unit_indices] : units_in_hex) {
      auto range {isInFirezone(*state.map, attacker, hex)};
      if (!range) continue;

      std::vector<std::pair<int8_t, int8_t>> targets_and_checks {};
      int8_t terrainDRmod {state.map->get(hex).getDRmodifier()};
      for (int8_t enemyIdx : unit_indices) {
        const Unit& defender {state.units[enemyIdx]};
        int8_t hitNumber {getHitNumber(attacker, defender, *range, terrainDRmod)};
        targets_and_checks.emplace_back(enemyIdx, hitNumber);
      }

      if (!targets_and_checks.empty()) {
        Action action {ActionType::Attack, attackerIdx, attacker.getAttackAPCost(), hex, std::move(targets_and_checks)};
        actions.push_back(std::move(action));
      }
    }
  }
  return actions;
}


void removeHexesWithoutEnemyUnits(std::map<HexCoord, std::vector<int8_t>>& units_in_hex, const State& state) {
  units_in_hex.erase(std::remove_if(units_in_hex.begin(), units_in_hex.end(),
                                    [&](const auto& pair) {
                                      const auto& [_, unit_indices] = pair;
                                      return std::none_of(unit_indices.begin(), unit_indices.end(), [&](int8_t index) {
                                        return state.units[index].getNation() != state.player;
                                      });
                                    }),
                     units_in_hex.end());
}


int8_t getHitNumber(const Unit& attacker, const Unit& defender, Firezone::RangeType range, int8_t terrainDRmod) {
  auto AR {getAttackRating(attacker, range)};
  auto DR {getDefenseRating(defender, attacker, range) + terrainDRmod};
  return AR - DR;
}

int8_t getAttackRating(const Unit& attacker, Firezone::RangeType range) {
  int8_t AR {attacker.getFirepower()};
  switch (range) {
    case Firezone::RangeType::Close: AR += (attacker.isCrewedUnit()) ? -2 : 4; break;
    case Firezone::RangeType::Short: AR += 3; break;
    case Firezone::RangeType::Medium: break;
    case Firezone::RangeType::Long: AR += -2; break;
  }
  return AR;
}

int8_t getDefenseRating(const Unit& defender, const Unit& attacker, Firezone::RangeType range) {
  auto defense_type {getDefenseType(defender, attacker, range)};
  return (defense_type == UnitTypes::DefenseType::Front) ? defender.getFrontDefense() : defender.getFlankDefense();
}

UnitTypes::DefenseType getDefenseType(const Unit& defender, const Unit& attacker, Firezone::RangeType range) {
  if (range == Firezone::RangeType::Close) {
    return UnitTypes::DefenseType::Flank;
  }
  return (isInArc(defender, attacker.getPosition())) ? UnitTypes::DefenseType::Front : UnitTypes::DefenseType::Flank;
}
