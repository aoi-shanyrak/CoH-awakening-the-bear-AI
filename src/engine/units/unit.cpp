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
bool Unit::canPerformAction(ActionType action) const {
  return (hit.has_value()) ? (getHitMarkerProps(hit.value()).allowed_actions & (1 << static_cast<uint8_t>(action))) != 0
                           : true;
}


int8_t Unit::getMoveAPCost() const {
  int8_t mv_ap_cost {getUnitProps(type).move_AP_cost};
  if (hit.has_value()) {
    mv_ap_cost += getHitMarkerProps(hit.value()).diff_move_AP_cost;
  }
  return mv_ap_cost + (isStressed() ? 1 : 0);
}

int8_t Unit::getPivotAPcost() const {
  return 1 + (isStressed() ? 1 : 0);
}

int8_t Unit::getAttackAPCost() const {
  int8_t atk_ap_cost {getUnitProps(type).attack_AP_cost};
  if (hit.has_value()) {
    atk_ap_cost += getHitMarkerProps(hit.value()).diff_attack_AP_cost;
  }
  return atk_ap_cost + (isStressed() ? 1 : 0);
}


Nation Unit::getNation() const {
  return getUnitProps(type).nation;
}
bool Unit::isCrewedUnit() const {
  return getUnitProps(type).is_crewed_unit;
}
