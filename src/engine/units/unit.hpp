#pragma once

#include "../directions.hpp"
#include "../field/hex.hpp"
#include "unit_types.hpp"


struct Unit {
  HexCoord position;
  UnitType type;
  Direction direction;
  bool is_fresh;
  bool is_stressed;
};
