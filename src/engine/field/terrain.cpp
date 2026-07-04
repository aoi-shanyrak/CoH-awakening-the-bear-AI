#include <array>

#include "terrain.hpp"


const std::array<TerrainProps, 8> TERRAIN_DATA = {{
    {0, 0, false},  // Open_terrain
    {0, 0, false},  // Plowed_field
    {0, 1, false},  // Light_wood
    {1, 2, false},  // Heavy_wood
    {1, 1, false},  // Wood_building
    {1, 2, false},  // Stone_building
    {5, -1, false},  // Water
    {2, 1, false}  // Swamp
}};


const TerrainProps& get_terrain_props(TerrainType terrain) {
  return TERRAIN_DATA[static_cast<uint8_t>(terrain)];
}
