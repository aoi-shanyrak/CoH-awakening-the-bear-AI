#include "combat.hpp"


using namespace Actions;
using namespace Firezone;
using namespace ActionDetails;
using namespace Combat::CombatImpl;


namespace Actions::Combat {


  void addCombatActionsForUnit(const GenerationContext& action_context, uint8_t attackerIdx) {
    const Unit& attacker {action_context.state.units[attackerIdx]};

    for (const auto& [hex, unit_indices] : action_context.units_in_hex) {
      auto range {isInFirezone(*action_context.state.map, attacker, hex)};
      if (!range || !isAnyEnemyUnitInHex(action_context, hex)) continue;

      int8_t terrainDRmod {action_context.state.map->get(hex).getDRmodifier()};
      const AttackContext context {attacker, *range, terrainDRmod};
      std::vector<Target> targets_and_checks {getTargets(action_context.state, context, unit_indices)};

      if (!targets_and_checks.empty()) {
        Action action {
            ActionType::Attack,           {attacker.getAttackAPcost(), attacker.isFresh()}, attackerIdx, hex, {},
            std::move(targets_and_checks)};
        action_context.actions.push_back(std::move(action));
      }
    }
  }


  namespace CombatImpl {


    std::vector<Target> getTargets(const State& state, const AttackContext& context,
                                   const std::vector<uint8_t>& unit_indices) {
      std::vector<Target> targets {};
      for (uint8_t enemyIdx : unit_indices) {
        const Unit& defender {state.units[enemyIdx]};
        int8_t hitNumber {getHitNumber(context, defender)};
        targets.push_back({enemyIdx, hitNumber});
      }
      return targets;
    }


    int8_t getHitNumber(const AttackContext& context, const Unit& defender) {
      auto AR {getAttackRating(context.attacker, context.range)};
      auto DR {getDefenseRating(context, defender)};
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

    int8_t getDefenseRating(const AttackContext& context, const Unit& defender) {
      auto& [attacker, range, terrainDRmod] {context};

      auto defense_type {getDefenseType(context, defender)};
      auto defense {(defense_type == UnitTypes::DefenseType::Front) ? defender.getFrontDefense()
                                                                    : defender.getFlankDefense()};
      return defense + context.terrainDRmod;
    }

    UnitTypes::DefenseType getDefenseType(const AttackContext& context, const Unit& defender) {
      if (context.range == Firezone::RangeType::Close) {
        return UnitTypes::DefenseType::Flank;
      }
      return (defender.isInArc(context.attacker.getPosition())) ? UnitTypes::DefenseType::Front
                                                                : UnitTypes::DefenseType::Flank;
    }


  }


}
