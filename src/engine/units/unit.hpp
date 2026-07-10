#pragma once

#include <optional>

#include "../directions.hpp"
#include "../field/hex.hpp"
#include "../markers/hits.hpp"
#include "unit_types.hpp"


class Unit {
 private:
  HexCoord position;
  UnitType type;
  Direction direction;
  bool is_fresh;
  bool is_stressed;
  std::optional<HitMarkers> hit;


 public:
  Unit(HexCoord position, UnitType type, Direction direction);


  HexCoord getPosition() const;
  UnitType getType() const;
  Direction getDirection() const;
  bool isFresh() const;
  bool isStressed() const;
  std::optional<HitMarkers> getHit() const;

  Nation getNation() const;
  bool isCrewedUnit() const;
  int8_t getMoveAPCost() const;
  int8_t getAttackAPCost() const;
  uint8_t getAttackRange() const;
  int8_t getFirepower() const;
  int8_t getFlankDefense() const;
  int8_t getFrontDefense() const;
};
