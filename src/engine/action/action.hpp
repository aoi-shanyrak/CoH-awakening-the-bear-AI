#pragma once

#include <cstdint>
#include <iterator>
#include <vector>

#include "../directions.hpp"
#include "../field/hex.hpp"


enum class ActionType : uint8_t { Attack, Move, Rally, Stall, PlayCard, Pass };


namespace ActionDetails {

  struct Target {
    uint8_t unitIdx;
    int8_t hitNumber;
  };

  struct Cost {
    int8_t action_points;
    int8_t needs_CAPs;
  };

}


struct Action {
  ActionType type;
  ActionDetails::Cost cost;

  uint8_t unitIndex;
  union {
    HexCoord target_hex;
    Direction new_direction;
  };
  std::vector<ActionDetails::Target> targets_for_attack;
};


inline std::vector<Action>& operator+=(std::vector<Action>& left, const std::vector<Action>& right) {
  left.reserve(left.size() + right.size());
  left.insert(left.end(), std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
  return left;
}
