#pragma once

#include <cstdint>
#include <vector>

#include "../field/map.hpp"
#include "../markers/VPcounter.hpp"
#include "../markers/control.hpp"
#include "../nations.hpp"
#include "../units/unit.hpp"


struct State {
  uint8_t round;
  Nation turn;
  VPcounter vp_count;

  const Map* map;
  std::vector<Unit> soviet_units;
  std::vector<Unit> germany_units;
  std::vector<ControlMarker> control_markers;
};
