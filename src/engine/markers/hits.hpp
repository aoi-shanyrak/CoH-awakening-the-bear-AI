#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include "../action/action.hpp"


enum class HitMarkers : uint8_t { Stunned, Unnerved, Destroyed, Panicked, Pinned, Suppressed, Cowering, Berserk };

struct HitMarkerProps {
  HitMarkers type;
  int8_t rally_number;

  char allowed_actions;

  int8_t diff_move_AP_cost;
  int8_t diff_attack_AP_cost;

  std::optional<uint8_t> new_attack_range;
  int8_t firepower_diff;

  int8_t flank_defense_diff;
  int8_t front_defense_diff;
};


namespace Action_masks {

  static constexpr char ATTACK = 1 << static_cast<char>(Actions::ActionType::Attack);
  static constexpr char MOVE = 1 << static_cast<char>(Actions::ActionType::Move);
  static constexpr char RALLY = 1 << static_cast<char>(Actions::ActionType::Rally);
  static constexpr char STALL = 1 << static_cast<char>(Actions::ActionType::Stall);

  static constexpr std::array<char, 8> ALLOWED_ACTION_MASKS {
      RALLY | STALL, /*                 Stunned    */
      ATTACK | MOVE | RALLY | STALL, /* Unnerved   */
      0, /*                     Destroyed  */
      MOVE | RALLY | STALL, /*          Panicked   */
      ATTACK | RALLY | STALL, /*        Pinned     */
      ATTACK | MOVE | RALLY | STALL, /* Suppressed */
      ATTACK | MOVE | RALLY | STALL, /* Cowering   */
      ATTACK | MOVE | RALLY | STALL /*  Berserk    */
  };

}

static constexpr std::array<HitMarkerProps, 8> HIT_MARKER_PROPS {
    HitMarkerProps {HitMarkers::Stunned, 7, Action_masks::ALLOWED_ACTION_MASKS[0], 0, 0, std::nullopt, 0, 0, 0},
    HitMarkerProps {HitMarkers::Unnerved, 7, Action_masks::ALLOWED_ACTION_MASKS[1], 0, 0, std::nullopt, 0, 0, 0},
    HitMarkerProps {HitMarkers::Destroyed, 0, Action_masks::ALLOWED_ACTION_MASKS[2], 0, 0, std::nullopt, 0, 0, 0},
    HitMarkerProps {HitMarkers::Panicked, 8, Action_masks::ALLOWED_ACTION_MASKS[3], 0, 0, std::nullopt, 0, +1, -2},
    HitMarkerProps {HitMarkers::Pinned, 7, Action_masks::ALLOWED_ACTION_MASKS[4], 0, 0, std::nullopt, 0, 0, 0},
    HitMarkerProps {HitMarkers::Suppressed, 7, Action_masks::ALLOWED_ACTION_MASKS[5], 0, +1, std::nullopt, -2, 0, 0},
    HitMarkerProps {HitMarkers::Cowering, 8, Action_masks::ALLOWED_ACTION_MASKS[6], +1, +2, 1, 0, +1, +1},
    HitMarkerProps {HitMarkers::Berserk, 8, Action_masks::ALLOWED_ACTION_MASKS[7], 0, -1, 1, +1, +1, +2}};


inline constexpr const HitMarkerProps& getHitMarkerProps(HitMarkers type) {
  return HIT_MARKER_PROPS[static_cast<uint8_t>(type)];
}
