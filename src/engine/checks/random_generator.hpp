#pragma once

#include <cstdint>
#include <random>


namespace Random {


  enum class CheckType : uint8_t { D6_1, D6_2, D10 };


  class RandomGenerator {
   private:
    std::mt19937 rng;

    int rollInt(int min, int max);
    int8_t rollD6_1();
    int8_t rollD6_2();
    int8_t rollD10();


   public:
    explicit RandomGenerator(int seed = std::random_device {}()) : rng(seed) {};

    int8_t roll(CheckType dice);
  };

  struct Check {
    RandomGenerator& rng;
    int8_t number;
    CheckType dice;


    bool isSuccess();
  };


}
