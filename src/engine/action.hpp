#pragma once

#include <cstdint>
#include <optional>

#include "field/hex.hpp"


enum class ActionType : uint8_t { Attack, Move, Rally, Stall, PlayCard, Pass };


struct Action {
  ActionType type;
  std::optional<HexCoord> unit_hex;
  std::optional<HexCoord> target_hex;
};
