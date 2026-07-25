#pragma once

#include <array>
#include <cstdint>


enum class CardType : uint8_t { Action, Bonus };

enum class Card : uint8_t {
  Adrenaline,
  CommandAction,
  FollowMe,
  RallyUp,
  RapidDeployment,
  BattlefieldConfusion,
  VeteranNCO,
  FrontlineOfficer,
  SeekCover,
  SniperFire,
  CarefulAiming,
  SwiftAction,
  Luck
};


struct CardProps {
  CardType type;
  Card card;
  uint8_t number_in_deck;

  int8_t CAP_cost;
};

static constexpr std::array<CardProps, 13> CARD_DATA = {{{CardType::Action, Card::Adrenaline, 4, 0},
                                                         {CardType::Action, Card::CommandAction, 8, 0},
                                                         {CardType::Action, Card::FollowMe, 2, 2},
                                                         {CardType::Action, Card::RallyUp, 2, 0},
                                                         {CardType::Bonus, Card::RapidDeployment, 2, 0},
                                                         {CardType::Bonus, Card::BattlefieldConfusion, 2, 0},
                                                         {CardType::Bonus, Card::VeteranNCO, 3, 0},
                                                         {CardType::Action, Card::FrontlineOfficer, 2, 2},
                                                         {CardType::Action, Card::SeekCover, 2, 0},
                                                         {CardType::Action, Card::SniperFire, 2, 0},
                                                         {CardType::Bonus, Card::CarefulAiming, 2, 0},
                                                         {CardType::Bonus, Card::SwiftAction, 4, 1},
                                                         {CardType::Bonus, Card::Luck, 3, 0}}};


inline constexpr const CardProps& get_card_props(Card card) {
  return CARD_DATA[static_cast<uint8_t>(card)];
}
