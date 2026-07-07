#pragma once

#include "../field/direction.hpp"
#include "unit_types.hpp"


struct Unit {
  UnitType type;
  Direction direction;
  bool is_fresh;
  bool is_stressed;
};
