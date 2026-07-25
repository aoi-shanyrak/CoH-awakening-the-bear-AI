#include <algorithm>
#include <cassert>

#include "player.hpp"


Player::Player(Nation nation, uint8_t CAPs, uint8_t limit, std::vector<Card> hand)
    : nation {nation}, CAPs {CAPs}, limit {limit}, hand {hand} {
}


Nation Player::getNation() const {
  return nation;
}
uint8_t Player::getCAPs() const {
  return CAPs;
}
const std::vector<Card>& Player::getHand() const {
  return hand;
}

bool Player::canPayCAPs(uint8_t cost) const {
  return cost <= CAPs;
}
bool Player::hasCard(Card card) const {
  return std::find(hand.begin(), hand.end(), card) != hand.end();
}

void Player::addCAPs(uint8_t caps) {
  auto new_caps {static_cast<uint8_t>(CAPs + caps)};
  CAPs = (new_caps > limit) ? limit : new_caps;
}
void Player::payCAPs(uint8_t caps) {
  assert(canPayCAPs(caps));
  CAPs -= caps;
}
void Player::addCard(Card card) {
  hand.push_back(std::move(card));
}
void Player::removeCard(Card card) {
  auto it {std::find(hand.begin(), hand.end(), card)};
  if (it != hand.end()) hand.erase(it);
}
void Player::decreaseLimitByOne() {
  auto new_limit {static_cast<uint8_t>(limit - 1)};
  limit = (new_limit < min_limit) ? min_limit : new_limit;
}
