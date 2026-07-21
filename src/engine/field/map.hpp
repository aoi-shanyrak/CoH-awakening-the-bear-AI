#pragma once

#include <array>
#include <optional>
#include <vector>

#include "hex.hpp"


class Map {
 private:
  static constexpr int16_t invalid_index = -1;

  static constexpr int16_t CUBE_DELTAS[6][3] = {
      {0, -1, 1},  // North
      {1, -1, 0},  // NorthEast
      {1, 0, -1},  // SouthEast
      {0, 1, -1},  // South
      {-1, 1, 0},  // SouthWest
      {-1, 0, 1}  // NorthWest
  };

  int16_t width;
  int16_t height;
  std::vector<Hex> hexes;
  std::vector<std::array<int16_t, 6>> neighbors;

  void computeNeighbors();
  int16_t getNeighborIndexForCompute(int16_t index, Direction dir) const;


 public:
  Map(int16_t width, int16_t height, const std::vector<Hex>& hexes);


  const Hex& get(HexCoord coord) const;
  const Hex& get(int16_t index) const;
  std::optional<int16_t> getNeighborIndex(int16_t index, Direction dir) const;


  bool hasLineOfSight(HexCoord start, HexCoord end) const;

  uint8_t distance(HexCoord a, HexCoord b) const;


  int16_t toIndex(HexCoord coord) const;
  HexCoord toCoord(int16_t index) const;
  bool isValidCoord(HexCoord coord) const;
  bool isValidIndex(int16_t index) const;
};
