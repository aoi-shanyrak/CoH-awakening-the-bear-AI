#pragma once

#include "../checks/random_generator.hpp"
#include "../field/map.hpp"
#include "../markers/VPcounter.hpp"
#include "../markers/control.hpp"
#include "../nations.hpp"
#include "../player.hpp"
#include "../units/unit.hpp"


struct State {
  uint8_t round;
  Nation turn;
  VPcounter vp_count;

  Player soviet;
  Player germany;

  std::vector<Unit> units;

  const Map* map;
  std::vector<ControlMarker> control_markers;


  Random::RandomGenerator rng;
};
