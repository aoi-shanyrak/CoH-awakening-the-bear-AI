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


bool Firezone::isInArc(const Unit& attacker, HexCoord target) {
  HexCoord attacker_pos {attacker.getPosition()};

  int dq {target.q - attacker_pos.q};
  int dr {target.r - attacker_pos.r};

  switch (attacker.getDirection()) {
    case Direction::North: return dq >= 0 && dr <= 0;
    case Direction::NorthEast: return dq + dr >= 0 && dq >= 0;
    case Direction::NorthWest: return dq + dr <= 0 && dr <= 0;
    case Direction::South: return dq <= 0 && dr >= 0;
    case Direction::SouthEast: return dq + dr <= 0 && dq <= 0;
    case Direction::SouthWest: return dq + dr >= 0 && dr >= 0;
  }
}


bool Firezone::hasLineOfSight(const Map& map, HexCoord start, HexCoord end) {
  return true;  // TODO: make an implementation
}


std::optional<Firezone::RangeType> Firezone::isInFirezone(const Map& map, const Unit& attacker, HexCoord target) {
  if (isInArc(attacker, target) && hasLineOfSight(map, attacker.getPosition(), target)) {
    std::optional<RangeType> range {isInRange(map, attacker, target)};
    if (range.has_value()) {
      return range.value();
    }
  }
  return std::nullopt;
}
