#pragma once

#include <cstdint>
#include <vector>

#include "../checks/random_generator.hpp"
#include "../field/map.hpp"
#include "../markers/VPcounter.hpp"
#include "../markers/control.hpp"
#include "../nations.hpp"
#include "../units/unit.hpp"


struct State {
  uint8_t round;
  Nation player;
  VPcounter vp_count;

  std::vector<Unit> units;

  const Map* map;
  std::vector<ControlMarker> control_markers;


  Random::RandomGenerator rng;
};
