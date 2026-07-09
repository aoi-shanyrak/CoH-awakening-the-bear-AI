#pragma once

#include <array>
#include <optional>
#include <vector>

#include "hex.hpp"


class Map {
 private:
  int width;
  int height;
  std::vector<Hex> hexes;
  std::vector<std::array<std::optional<int>, 6>> neighbors;

  void computeNeighbors();
  std::optional<int> getNeighborIndexForCompute(int index, Direction dir) const;


 public:
  const Hex& get(HexCoord coord) const;
  const Hex& get(int index) const;

  std::optional<int> getNeighborIndex(int index, Direction dir) const;


  int toIndex(HexCoord coord) const;
  HexCoord toCoord(int index) const;

  bool isValidCoord(HexCoord coord) const;
  bool isValidIndex(int index) const;
};
