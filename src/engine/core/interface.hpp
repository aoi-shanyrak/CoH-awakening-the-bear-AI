#pragma once

#include "../action/action.hpp"
#include "state.hpp"


namespace GameEngine {


  std::vector<Actions::Action> getBaseActions(const State& state);

  State applyAction(const State& state, const Actions::Action& action);

  State cloneState(const State& state);

  bool isGameOver(const State& state);

  double evaluateState(const State& state, Nation perspective);

  int8_t getCAPs(const State& state);
  const std::vector<Card>& getHand(const State& state);


  Random::RandomGenerator& getRandomGenerator(const State& state);


};
