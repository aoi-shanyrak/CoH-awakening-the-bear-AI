#pragma once

#include <cstdint>
#include <vector>

#include "../VPcounter.hpp"
#include "../field/map.hpp"
#include "../markers/control.hpp"
#include "../nations.hpp"
#include "../units/unit.hpp"


struct State {
  uint8_t round;
  Nation turn;
  VPcounter vp_count;

  const Map* map;
  std::vector<Unit> units;
  std::vector<ControlMarker> control_markers;
};
