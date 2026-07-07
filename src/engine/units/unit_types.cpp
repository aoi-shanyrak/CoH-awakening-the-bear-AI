#include <array>

#include "unit_types.hpp"


const std::array<UnitProps, 5> UNIT_DATA = {
    {{Nation::Germany, UnitType::Rifles_Squad_41, false, 1, 3, 5, 2, 11, 12},
     {Nation::Germany, UnitType::MG34, true, 1, 2, 9, 4, 10, 12},
     {Nation::Germany, UnitType::Pioneers_Squad, true, 1, 2, 3, 4, 12, 12},
     {Nation::SovietUnion, UnitType::Rifles_41_Squad, true, 1, 4, 5, 3, 11, 12},
     {Nation::SovietUnion, UnitType::MMG_Maxim_Squad, false, 2, 3, 9, 3, 10, 12}}};


const UnitProps& get_unit_props(UnitType unit) {
  return UNIT_DATA[static_cast<uint8_t>(unit)];
}
