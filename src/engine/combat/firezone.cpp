#include "firezone.hpp"


std::optional<FireZone::RangeType> FireZone::isInRange(const Unit& attacker, HexCoord target, const Map& map) {
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
