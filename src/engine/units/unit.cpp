#include "unit.hpp"


Unit::Unit(HexCoord position, UnitType type, Direction direction)
    : position(position), type(type), direction(direction), is_fresh(true), is_stressed(false), hit(std::nullopt) {
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
std::optional<HitMarkers> Unit::getHit() const {
  return hit;
}


Nation Unit::getNation() const {
  return getUnitProps(type).nation;
}
bool Unit::isCrewedUnit() const {
  return getUnitProps(type).is_crewed_unit;
}

int8_t Unit::getMoveAPCost() const {
  int8_t base_mv_ap_cost {getUnitProps(type).move_AP_cost};
  if (hit.has_value()) {
    base_mv_ap_cost += getHitMarkerProps(hit.value()).diff_move_AP_cost;
  }
  return base_mv_ap_cost + (is_stressed ? 1 : 0);
}

int8_t Unit::getAttackAPCost() const {
  int8_t base_atk_ap_cost {getUnitProps(type).attack_AP_cost};
  if (hit.has_value()) {
    base_atk_ap_cost += getHitMarkerProps(hit.value()).diff_attack_AP_cost;
  }
  return base_atk_ap_cost;
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
