#pragma once

#include <array>
#include <vector>

#include "hex.hpp"


struct HexCoord {
  int q;
  int r;
};


class Map {
 private:
  int width;
  int height;
  std::vector<Hex> hexes;
  std::vector<std::array<int, 6>> neighbors;

  void computeNeighbors();


 public:
  const Hex& get(HexCoord coord) const;
  const Hex& get(int index) const;

  int toIndex(HexCoord coord) const;
  HexCoord toCoord(int index) const;

  int getNeighborIndex(int index, Direction dir) const;
};
