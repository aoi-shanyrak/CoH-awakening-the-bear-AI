#pragma once

#include "direction.hpp"
#include "terrain.hpp"


struct Hex {
  TerrainType terrain;
  uint8_t elevation;
  uint8_t walls_bitmask;
  bool has_road;
};


inline bool hasWall(const Hex& hex, Direction dir) {
  return (hex.walls_bitmask & (1 << static_cast<uint8_t>(dir))) != 0;
}
