#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <optional>

#include "map.hpp"


Map::Map(int16_t width, int16_t height, const std::vector<Hex>& hexes) : width(width), height(height), hexes(hexes) {
  computeNeighbors();
}


void Map::computeNeighbors() {
  neighbors.resize(hexes.size());

  for (int16_t index = 0; index < hexes.size(); ++index) {
    for (int8_t dir = 0; dir < 6; ++dir) {
      neighbors[index][dir] = getNeighborIndexForCompute(index, static_cast<Direction>(dir));
    }
  }
}
int16_t Map::getNeighborIndexForCompute(int16_t index, Direction dir) const {
  auto [q, r] = toCoord(index);
  int16_t x = q, y = r, z = -q - r;

  x += CUBE_DELTAS[dir][0];
  y += CUBE_DELTAS[dir][1];
  z += CUBE_DELTAS[dir][2];

  assert(x + y + z == 0);
  int16_t nq = x, nr = y;

  if (!isValidCoord({nq, nr})) {
    return invalid_index;
  }
  return toIndex({nq, nr});
}
std::optional<int16_t> Map::getNeighborIndex(int16_t index, Direction dir) const {
  int16_t neighborIndex = neighbors[index][static_cast<int8_t>(dir)];
  if (neighborIndex == invalid_index) {
    return std::nullopt;
  }
  return std::optional<int16_t>(neighborIndex);
}


bool Map::hasLineOfSight(HexCoord start, HexCoord end) const {
  return true;  // TODO: Implement line of sight algorithm
}

uint8_t Map::distance(HexCoord a, HexCoord b) const {
  int x1 = a.q, y1 = a.r, z1 = -a.q - a.r;
  int x2 = b.q, y2 = b.r, z2 = -b.q - b.r;

  return (std::abs(x1 - x2) + std::abs(y1 - y2) + std::abs(z1 - z2)) / 2;
}


const Hex& Map::get(HexCoord coord) const {
  return hexes[toIndex(coord)];
}
const Hex& Map::get(int16_t index) const {
  return hexes[index];
}

int16_t Map::toIndex(HexCoord coord) const {
  assert(isValidCoord(coord));
  return coord.r * width + coord.q;
}
HexCoord Map::toCoord(int16_t index) const {
  assert(isValidIndex(index));
  return {static_cast<int16_t>(index % width), static_cast<int16_t>(index / width)};
}
bool Map::isValidCoord(HexCoord coord) const {
  return coord.q >= 0 && coord.q < width && coord.r >= 0 && coord.r < height;
}
bool Map::isValidIndex(int16_t index) const {
  return isValidCoord(toCoord(index));
}
