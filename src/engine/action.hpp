#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "field/hex.hpp"


enum class ActionType : uint8_t { Attack, Move, Rally, Stall, PlayCard, Pass };


struct Action {
  ActionType type;
  int8_t unitIndex;

  int8_t action_check;

  HexCoord target_hex;
  std::vector<std::pair<int8_t, int8_t>> targets_and_checks_for_attack;
};
