#pragma once

#include <optional>
#include <vector>

#include "../directions.hpp"
#include "../field/hex.hpp"
#include "cards/card.hpp"


namespace Actions {


  enum class ActionType : uint8_t { Attack, Move, Rally, Stall, PlayCard, Pass };


  namespace ActionDetails {

    struct Target {
      uint8_t unitIdx;
      int8_t hitNumber;
    };

    struct Cost {
      int8_t action_points;
      int8_t needs_CAPs;


      Cost() : action_points {}, needs_CAPs {} {}
      Cost(int8_t ap, bool fresh)
          : action_points {fresh ? ap : static_cast<int8_t>(0)}, needs_CAPs {fresh ? static_cast<int8_t>(0) : ap} {}
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
    int8_t rallyNumber;
    std::vector<ActionDetails::Target> targets_for_attack;

    std::optional<Card> card;
    ActionType base_action;
  };


}


inline std::vector<Actions::Action> operator+=(std::vector<Actions::Action>& left,
                                               const std::vector<Actions::Action>& right) {
  left.reserve(left.size() + right.size());
  left.insert(left.end(), std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
  return left;
}

inline std::vector<Actions::Action>::iterator erase_unordered(std::vector<Actions::Action>& vec,
                                                              std::vector<Actions::Action>::iterator it) {
  if (it == vec.end()) return vec.end();
  *it = std::move(vec.back());
  vec.pop_back();
  return it;
}
