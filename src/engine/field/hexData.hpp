#include "terrain.hpp"


struct HexData {
  TerrainType terrain;
  uint8_t elevation;
  uint8_t walls;  // bitmask for walls in each direction <N, NE, SE, S, SW, NW, 0, 0>
  bool has_road;
};


enum Direction : uint8_t { North = 0, NorthEast = 1, SouthEast = 2, South = 3, SouthWest = 4, NorthWest = 5 };


inline bool hasWall(const HexData& hex, Direction dir) {
  return (hex.walls & (1 << static_cast<uint8_t>(dir))) != 0;
}
