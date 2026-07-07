#include <cstdint>


enum class Nation : uint8_t { Germany, SovietUnion };

enum class UnitType : uint8_t { Rifles_Squad_41, MG34, Pioneers_Squad, Rifles_41_Squad, MMG_Maxim_Squad };

struct UnitProps {
  Nation nation;
  UnitType type;
  bool is_crewed_unit;

  int8_t movement_AP_cost;

  int8_t attack_AP_cost;

  uint8_t attack_range;
  int8_t firepower;

  int8_t flank_defense;
  int8_t front_defense;
};


const UnitProps& get_unit_props(UnitType unit);
