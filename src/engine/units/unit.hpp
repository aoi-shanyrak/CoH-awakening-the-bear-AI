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


  int8_t stressPenalty() const;

  bool canPerformAction(ActionType action) const;


 public:
  Unit(UnitTypes::UnitType type, HexCoord position, Direction direction);


  void move_to(HexCoord new_pos, Direction direction, bool gotSpent);

  HexCoord getPosition() const;
  UnitTypes::UnitType getType() const;
  Direction getDirection() const;
  bool isFresh() const;
  bool isStressed() const;
  bool hasHitmarker() const;


  bool isInArc(HexCoord target) const;

  uint8_t getAttackRange() const;
  int8_t getFirepower() const;
  int8_t getFlankDefense() const;
  int8_t getFrontDefense() const;


  bool canAttack() const;
  bool canMove() const;
  bool canRally() const;
  bool canStall() const;

  int8_t getMoveAPcost() const;
  int8_t getPivotAPcost() const;
  int8_t getAttackAPcost() const;
  int8_t getRallyAPcost() const;
  int8_t getStallAPcost() const;


  Nation getNation() const;
  bool isCrewedUnit() const;
};
