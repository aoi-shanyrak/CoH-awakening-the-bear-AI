#pragma once

#include <cstdint>
#include <iterator>
#include <vector>

#include "field/hex.hpp"


enum class ActionType : uint8_t { Attack, Move, Rally, Stall, PlayCard, Pass };


namespace ActionDetails {

  struct Target {
    int8_t unitIdx;
    int8_t hitNumber;
  };

}


struct Action {
  ActionType type;
  int8_t unitIndex;

  int8_t action_check;

  HexCoord target_hex;
  std::vector<ActionDetails::Target> targets_for_attack;
};


inline std::vector<Action> operator+=(std::vector<Action>& left, const std::vector<Action>& right) {
  left.reserve(left.size() + right.size());
  left.insert(left.end(), std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
  return left;
}
