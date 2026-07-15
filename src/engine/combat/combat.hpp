#pragma once

#include <map>

#include "../action.hpp"
#include "../core/state.hpp"
#include "../nations.hpp"
#include "firezone.hpp"


namespace Combat {


  std::vector<Action> getValidCombatActions(const State& state, Nation player);


  namespace CombatImpl {


    void removeHexesWithoutEnemyUnits(std::map<HexCoord, std::vector<int8_t>>& units_in_hex, const State& state);

    int8_t getHitNumber(const Unit& attacker, const Unit& defender, Firezone::RangeType range, int8_t terrainDRmod);
    int8_t getAttackRating(const Unit& attacker, Firezone::RangeType range);
    int8_t getDefenseRating(const Unit& defender, const Unit& attacker, Firezone::RangeType range);
    UnitTypes::DefenseType getDefenseType(const Unit& defender, const Unit& attacker, Firezone::RangeType range);


  }


}
