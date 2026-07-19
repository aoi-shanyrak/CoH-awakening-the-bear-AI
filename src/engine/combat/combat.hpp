#pragma once

#include <cstdint>
#include <map>

#include "../action.hpp"
#include "../core/state.hpp"
#include "firezone.hpp"


namespace Combat {


  std::vector<Action> getValidCombatActions(const State& state);


  namespace CombatImpl {


    struct AttackContext {
      const Unit& attacker;
      Firezone::RangeType range;
      int8_t terrainDRmod;
    };

    std::map<HexCoord, std::vector<int8_t>> getUnitsInHexes(const State& state);
    void removeHexesWithoutEnemyUnits(std::map<HexCoord, std::vector<int8_t>>& units_in_hex, const State& state);

    std::vector<Action> buildCombatActionsForAttacker(const State& state,
                                                      const std::map<HexCoord, std::vector<int8_t>> units_in_hex,
                                                      int8_t attackerIdx);
    std::vector<ActionDetails::Target> getTargets(const State& state, const AttackContext& context,
                                                  const std::vector<int8_t> unit_indices);

    int8_t getHitNumber(const AttackContext& context, const Unit& defender);
    int8_t getAttackRating(const Unit& attacker, Firezone::RangeType range);
    int8_t getDefenseRating(const AttackContext& context, const Unit& defender);
    UnitTypes::DefenseType getDefenseType(const AttackContext& context, const Unit& defender);


  }


}
