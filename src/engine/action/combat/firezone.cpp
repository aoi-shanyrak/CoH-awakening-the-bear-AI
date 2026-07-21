#include "firezone.hpp"
#include <optional>


using namespace UnitTypes;
using namespace Firezone;


std::optional<RangeType> Firezone::isInRange(const Map& map, const Unit& attacker, HexCoord target) {
  uint8_t dist {map.distance(attacker.getPosition(), target)};
  uint8_t attack_range {attacker.getAttackRange()};

  if (dist == 0) {
    return RangeType::Close;
  } else if (dist == 1) {
    return RangeType::Short;
  } else if (dist <= attack_range) {
    return RangeType::Medium;
  } else if (dist <= 2 * attack_range) {
    return RangeType::Long;
  } else {
    return std::nullopt;
  }
}


std::optional<Firezone::RangeType> Firezone::isInFirezone(const Map& map, const Unit& attacker, HexCoord target) {
  if (!attacker.isInArc(target)) return std::nullopt;
  if (!map.hasLineOfSight(attacker.getPosition(), target)) return std::nullopt;
  return isInRange(map, attacker, target);
}
