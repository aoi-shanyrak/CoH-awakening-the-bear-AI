#include "unit.hpp"
#include <cstdint>


using namespace UnitTypes;
using namespace Action_masks;


Unit::Unit(UnitType type, HexCoord position, Direction direction)
    : position(position), type(type), direction(direction), is_fresh(true), is_stressed(false), hit(std::nullopt) {
}


void Unit::move_to(HexCoord new_pos, Direction direction, bool gotSpent) {
  is_fresh = (gotSpent) ? false : true;
  came_from = getOppositeTo(direction);
  position = new_pos;
}


HexCoord Unit::getPosition() const {
  return position;
}
UnitType Unit::getType() const {
  return type;
}
Direction Unit::getDirection() const {
  return direction;
}
bool Unit::isFresh() const {
  return is_fresh;
}
bool Unit::isStressed() const {
  return is_stressed;
}
bool Unit::hasHitmarker() const {
  return hit.has_value();
}

int8_t Unit::stressPenalty() const {
  return (isStressed() ? 1 : 0);
}


bool Unit::isInArc(HexCoord target) const {
  HexCoord attacker_pos {getPosition()};

  int dq {target.q - attacker_pos.q};
  int dr {target.r - attacker_pos.r};

  switch (getDirection()) {
    case Direction::North: return dq >= 0 && dr <= 0;
    case Direction::NorthEast: return dq + dr >= 0 && dq >= 0;
    case Direction::NorthWest: return dq + dr <= 0 && dr <= 0;
    case Direction::South: return dq <= 0 && dr >= 0;
    case Direction::SouthEast: return dq + dr <= 0 && dq <= 0;
    case Direction::SouthWest: return dq + dr >= 0 && dr >= 0;
  }
}

uint8_t Unit::getAttackRange() const {
  if (hit.has_value()) return getHitMarkerProps(hit.value()).new_attack_range.value_or(getUnitProps(type).attack_range);
  return getUnitProps(type).attack_range;
}
int8_t Unit::getFirepower() const {
  int8_t base_firepower {getUnitProps(type).firepower};
  if (hit.has_value()) {
    base_firepower += getHitMarkerProps(hit.value()).firepower_diff;
  }
  return base_firepower;
}
int8_t Unit::getFlankDefense() const {
  int8_t base_flank_defense {getUnitProps(type).flank_defense};
  if (hit.has_value()) {
    base_flank_defense += getHitMarkerProps(hit.value()).flank_defense_diff;
  }
  return base_flank_defense;
}
int8_t Unit::getFrontDefense() const {
  int8_t base_front_defense {getUnitProps(type).front_defense};
  if (hit.has_value()) {
    base_front_defense += getHitMarkerProps(hit.value()).front_defense_diff;
  }
  return base_front_defense;
}


bool Unit::canAttack() const {
  return canPerformAction(ActionType::Attack);
}
bool Unit::canMove() const {
  return canPerformAction(ActionType::Move);
}
bool Unit::canRally() const {
  return canPerformAction(ActionType::Rally);
}
bool Unit::canStall() const {
  return canPerformAction(ActionType::Stall);
}
bool Unit::canPerformAction(ActionType action) const {
  return (hit.has_value()) ? (getHitMarkerProps(hit.value()).allowed_actions & (1 << static_cast<uint8_t>(action))) != 0
                           : true;
}


int8_t Unit::getAttackAPcost() const {
  int8_t atk_ap_cost {getUnitProps(type).attack_AP_cost};
  if (hit.has_value()) {
    atk_ap_cost += getHitMarkerProps(hit.value()).diff_attack_AP_cost;
  }
  return atk_ap_cost + stressPenalty();
}
int8_t Unit::getMoveAPcost() const {
  int8_t mv_ap_cost {getUnitProps(type).move_AP_cost};
  if (hit.has_value()) {
    mv_ap_cost += getHitMarkerProps(hit.value()).diff_move_AP_cost;
  }
  return mv_ap_cost + stressPenalty();
}
int8_t Unit::getPivotAPcost() const {
  return 1 + stressPenalty();
}
int8_t Unit::getRallyAPcost() const {
  return 5 + stressPenalty();
}
int8_t Unit::getStallAPcost() const {
  return 1 + stressPenalty();
}


Nation Unit::getNation() const {
  return getUnitProps(type).nation;
}
bool Unit::isCrewedUnit() const {
  return getUnitProps(type).is_crewed_unit;
}
