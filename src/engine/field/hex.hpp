#pragma once

#include "../directions.hpp"
#include "terrain.hpp"


struct HexCoord {
  int q;
  int r;
};


struct Hex {
  Terrain terrain;
  uint8_t elevation;
  uint8_t walls_bitmask;
  bool has_road;
};


inline bool hasWallatDirection(const Hex& hex, Direction dir) {
  return (hex.walls_bitmask & (1 << static_cast<uint8_t>(dir))) != 0;
}
