#pragma once

#include <array>
#include <cstdint>

#include "../nations.hpp"


enum class UnitType : uint8_t { Rifles_Squad_41, MG34, Pioneers_Squad, Rifles_41_Squad, MMG_Maxim_Squad };

struct UnitProps {
  Nation nation;
  UnitType type;
  bool is_crewed_unit;

  int8_t move_AP_cost;
  int8_t attack_AP_cost;

  uint8_t attack_range;
  int8_t firepower;

  int8_t flank_defense;
  int8_t front_defense;
};


static constexpr std::array<UnitProps, 5> UNIT_DATA {
    {{Nation::Germany, UnitType::Rifles_Squad_41, false, 1, 3, 5, 2, 11, 12},
     {Nation::Germany, UnitType::MG34, true, 1, 2, 9, 4, 10, 12},
     {Nation::Germany, UnitType::Pioneers_Squad, true, 1, 2, 3, 4, 12, 12},
     {Nation::SovietUnion, UnitType::Rifles_41_Squad, true, 1, 4, 5, 3, 11, 12},
     {Nation::SovietUnion, UnitType::MMG_Maxim_Squad, false, 2, 3, 9, 3, 10, 12}}};


inline constexpr const UnitProps& get_unit_props(UnitType unit) {
  return UNIT_DATA[static_cast<uint8_t>(unit)];
}
