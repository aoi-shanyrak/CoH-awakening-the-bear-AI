#pragma once

#include <optional>

#include "../field/map.hpp"
#include "../units/unit.hpp"


namespace FireZone {


  enum class RangeType { Close, Short, Medium, Long };


  std::optional<RangeType> isInRange(const Unit& attacker, HexCoord target, const Map& map);

  bool isInArc(const Unit& attacker, HexCoord target, const Map& map);

  bool isInFireZone(const Unit& attacker, HexCoord target, const Map& map);


}
