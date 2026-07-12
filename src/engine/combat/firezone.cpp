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


bool isInArc(const Unit& attacker, HexCoord target) {
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
