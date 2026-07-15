#include <array>

#include "random_generator.hpp"


int8_t RandomGenerator::rollD6_2() {
  return rollD6_1() + rollD6_1();
}

int8_t RandomGenerator::rollD6_1() {
  return rollInt(1, 6);
}

int8_t RandomGenerator::rollD10() {
  static const std::array<int8_t, 10> d10_table {1, 1, 2, 3, 3, 4, 5, 5, 6, 7};
  return d10_table[rollInt(0, 9)];
}


int RandomGenerator::rollInt(int min, int max) {
  std::uniform_int_distribution<int> dist(min, max);
  return dist(rng);
}
