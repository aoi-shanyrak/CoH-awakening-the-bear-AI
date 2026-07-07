#pragma once

#include <cstdint>


enum class TerrainType : uint8_t {
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
  int8_t foot_penalty_AP;
  int8_t bonus_DR;
  bool blocks_LOS;
};


const TerrainProps& get_terrain_props(TerrainType terrain);
