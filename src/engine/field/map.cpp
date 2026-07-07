#include <cassert>

#include "map.hpp"


const int CUBE_DELTAS[6][3] = {
    {1, -1, 0},  // NorthEast
    {1, 0, -1},  // East
    {0, 1, -1},  // SouthEast
    {-1, 1, 0},  // SouthWest
    {-1, 0, 1},  // West
    {0, -1, 1}  // NorthWest
};


int Map::getNeighborIndex(int index, Direction dir) const {
  auto [q, r] = toCoord(index);
  int x = q, y = r, z = -q - r;

  x += CUBE_DELTAS[dir][0];
  y += CUBE_DELTAS[dir][1];
  z += CUBE_DELTAS[dir][2];

  assert(x + y + z == 0);

  int nq = x, nr = y;

  if (nq < 0 || nq >= width || nr < 0 || nr >= height) {
    return -1;
  }

  return toIndex({nq, nr});
}


const Hex& Map::get(HexCoord coord) const {
  return hexes[toIndex(coord)];
}

const Hex& Map::get(int index) const {
  return hexes[index];
}


int Map::toIndex(HexCoord coord) const {
  return coord.r * width + coord.q;
}

HexCoord Map::toCoord(int index) const {
  return {index % width, index / width};
}
