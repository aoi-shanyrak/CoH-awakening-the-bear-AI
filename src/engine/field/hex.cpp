#include "hex.hpp"


Hex::Hex(HexCoord pos, Terrain terrain, uint8_t elevation, uint8_t walls_bitmask, bool has_road)
    : position(pos), terrain(terrain), elevation(elevation), walls_bitmask(walls_bitmask), has_road(has_road) {
}


bool Hex::hasWallatDirection(Direction dir) {
  return (walls_bitmask & (1 << static_cast<uint8_t>(dir))) != 0;
}


HexCoord Hex::getPosition() const {
  return position;
}
Terrain Hex::getTerrain() const {
  return terrain;
}
bool Hex::blocksLOS() const {
  return get_terrain_props(terrain).blocks_LOS;
}
uint8_t Hex::getElevation() const {
  return elevation;
}
bool Hex::hasRoad() const {
  return has_road;
}


int8_t Hex::getFootPenaltyAP() const {
  return get_terrain_props(terrain).foot_penalty_AP;
}
int8_t Hex::getDRmodifier() const {
  return get_terrain_props(terrain).DR_modifier;
}
int8_t Hex::getCoverModifier() const {
  return get_terrain_props(terrain).cover_modifier;
}
