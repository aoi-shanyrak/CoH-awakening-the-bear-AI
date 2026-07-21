#pragma once

#include <vector>

#include "action/cards/card.hpp"
#include "nations.hpp"


class Player {
 private:
  Nation nation;
  uint8_t CAPs;
  std::vector<Card> hand;


 public:
  Player(Nation nation, uint8_t CAPs);
};
