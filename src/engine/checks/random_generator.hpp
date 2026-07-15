#pragma once

#include <cstdint>
#include <random>


class RandomGenerator {
 private:
  std::mt19937 rng;

  int rollInt(int min, int max);


 public:
  explicit RandomGenerator(int seed = std::random_device {}()) : rng(seed) {};

  int8_t rollD6_1();
  int8_t rollD6_2();
  int8_t rollD10();
};
