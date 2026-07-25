#pragma once

#include <vector>

#include "action/cards/card.hpp"
#include "nations.hpp"


class Player {
 private:
  static constexpr uint8_t max_limit {15};
  static constexpr uint8_t min_limit {3};

  Nation nation;
  uint8_t CAPs;
  uint8_t limit;
  std::vector<Card> hand;


 public:
  Player(Nation nation, uint8_t CAPs, uint8_t limit, std::vector<Card> hand);


  Nation getNation() const;
  uint8_t getCAPs() const;
  const std::vector<Card>& getHand() const;

  bool canPayCAPs(uint8_t cost) const;
  bool hasCard(Card card) const;

  void addCAPs(uint8_t caps);
  void payCAPs(uint8_t caps);
  void addCard(Card card);
  void removeCard(Card card);
  void decreaseLimitByOne();
};
