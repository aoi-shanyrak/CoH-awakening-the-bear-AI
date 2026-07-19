#pragma once

#include <cstdint>
#include <optional>

#include "../directions.hpp"
#include "../field/hex.hpp"
#include "../markers/hits.hpp"
#include "unit_types.hpp"


class Unit {
 private:
  HexCoord position;
  Direction came_from;
  UnitTypes::UnitType type;
  Direction direction;
  bool is_fresh;
  bool is_stressed;
  std::optional<HitMarkers> hit;

  bool canPerformAction(ActionType action) const;


 public:
  Unit(UnitTypes::UnitType type, HexCoord position, Direction direction);


  void move_to(HexCoord new_pos, Direction direction, bool gotSpent);

  HexCoord getPosition() const;
  UnitTypes::UnitType getType() const;
  Direction getDirection() const;
  bool isFresh() const;
  bool isStressed() const;

  uint8_t getAttackRange() const;
  int8_t getFirepower() const;
  int8_t getFlankDefense() const;
  int8_t getFrontDefense() const;

  bool canAttack() const;
  bool canMove() const;
  bool canRally() const;

  int8_t getMoveAPCost() const;
  int8_t getPivotAPcost() const;
  int8_t getAttackAPCost() const;


  Nation getNation() const;
  bool isCrewedUnit() const;
};
