#pragma once

#include <vector>

#include "../field/map.hpp"
#include "../markers/control_markers.hpp"
#include "../units/unit.hpp"


struct GameState {
  const Map& map;
  std::vector<Unit> units;
  std::vector<ControlMarker> control_markers;
};
