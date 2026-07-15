#pragma once

#include "../directions.hpp"
#include "terrain.hpp"


struct HexCoord {
  int16_t q;
  int16_t r;
};


class Hex {
 private:
  HexCoord position;
  Terrain terrain;
  uint8_t elevation;
  uint8_t walls_bitmask;
  bool has_road;


 public:
  Hex(HexCoord pos, Terrain terrain, uint8_t elevation, uint8_t walls_bitmask, bool has_road);

  bool hasWallatDirection(Direction dir);

  HexCoord getPosition() const;
  int8_t getFootPenaltyAP() const;
  int8_t getDRmodifier() const;
  bool blocksLOS() const;
  uint8_t getElevation() const;
  bool hasRoad() const;
};
