#pragma once

#include "../field/map.hpp"
#include "../units/unit.hpp"


namespace Firezone {


  enum class RangeType { Close, Short, Medium, Long };


  std::optional<RangeType> isInRange(const Map& map, const Unit& attacker, HexCoord target);

  bool isInArc(const Unit& attacker, HexCoord target);

  bool hasLineOfSight(const Map& map, HexCoord start, HexCoord end);


  std::optional<RangeType> isInFirezone(const Map& map, const Unit& attacker, HexCoord target);


}
