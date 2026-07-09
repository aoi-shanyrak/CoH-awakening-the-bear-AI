#include <cassert>
#include <optional>

#include "map.hpp"


const int CUBE_DELTAS[6][3] = {
    {1, -1, 0},  // NorthEast
    {1, 0, -1},  // East
    {0, 1, -1},  // SouthEast
    {-1, 1, 0},  // SouthWest
    {-1, 0, 1},  // West
    {0, -1, 1}  // NorthWest
};


void Map::computeNeighbors() {
  neighbors.resize(hexes.size());

  for (int index = 0; index < hexes.size(); ++index) {
    for (int dir = 0; dir < 6; ++dir) {
      neighbors[index][dir] = getNeighborIndexForCompute(index, static_cast<Direction>(dir));
    }
  }
}

std::optional<int> Map::getNeighborIndexForCompute(int index, Direction dir) const {
  auto [q, r] = toCoord(index);
  int x = q, y = r, z = -q - r;

  x += CUBE_DELTAS[dir][0];
  y += CUBE_DELTAS[dir][1];
  z += CUBE_DELTAS[dir][2];

  assert(x + y + z == 0);

  int nq = x, nr = y;

  if (!isValidCoord({nq, nr})) {
    return std::nullopt;
  }

  return toIndex({nq, nr});
}

std::optional<int> Map::getNeighborIndex(int index, Direction dir) const {
  return neighbors[index][static_cast<int>(dir)];
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


bool Map::isValidCoord(HexCoord coord) const {
  return coord.q >= 0 && coord.q < width && coord.r >= 0 && coord.r < height;
}

bool Map::isValidIndex(int index) const {
  return isValidCoord(toCoord(index));
}
