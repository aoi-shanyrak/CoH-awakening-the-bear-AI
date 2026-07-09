#pragma once

#include <array>
#include <cstdint>


enum class Terrain : uint8_t {
  Open_terrain,
  Plowed_field,
  Light_wood,
  Heavy_wood,
  Wood_building,
  Stone_building,
  Water,
  Swamp
};

struct TerrainProps {
  Terrain type;
  int8_t foot_penalty_AP;
  int8_t bonus_DR;
  bool blocks_LOS;
};


static constexpr std::array<TerrainProps, 8> TERRAIN_DATA = {{{Terrain::Open_terrain, 0, 0, false},
                                                              {Terrain::Plowed_field, 0, 0, false},
                                                              {Terrain::Light_wood, 0, 1, true},
                                                              {Terrain::Heavy_wood, 1, 2, true},
                                                              {Terrain::Wood_building, 1, 1, true},
                                                              {Terrain::Stone_building, 1, 2, true},
                                                              {Terrain::Water, 5, -1, false},
                                                              {Terrain::Swamp, 2, 1, false}}};


inline constexpr const TerrainProps& get_terrain_props(Terrain terrain) {
  return TERRAIN_DATA[static_cast<uint8_t>(terrain)];
}
