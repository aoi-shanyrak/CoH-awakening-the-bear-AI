#pragma once

#include "../../core/action_generation.hpp"
#include "firezone.hpp"


namespace Actions::Combat {


  void addCombatActionsForUnit(const GenerationContext& action_context, uint8_t attackerIdx);


  namespace CombatImpl {


    struct AttackContext {
      const Unit& attacker;
      Firezone::RangeType range;
      int8_t terrainDRmod;
    };

    std::vector<ActionDetails::Target> getTargets(const State& state, const AttackContext& context,
                                                  const std::vector<uint8_t>& unit_indices);

    int8_t getHitNumber(const AttackContext& context, const Unit& defender);
    int8_t getAttackRating(const Unit& attacker, Firezone::RangeType range);
    int8_t getDefenseRating(const AttackContext& context, const Unit& defender);
    UnitTypes::DefenseType getDefenseType(const AttackContext& context, const Unit& defender);


  }


}
