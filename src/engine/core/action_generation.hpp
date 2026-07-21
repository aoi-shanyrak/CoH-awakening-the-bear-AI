#pragma once

#include <map>

#include "../action/action.hpp"
#include "state.hpp"


using HexUnitsMap = std::map<HexCoord, std::vector<uint8_t>>;

struct ActionGenerationContext {
  const State& state;
  HexUnitsMap& units_in_hex;
};


HexUnitsMap getUnitsInHexes(const State& state);
bool isAnyEnemyUnitInHex(const ActionGenerationContext& context, HexCoord hex);

std::vector<Action> generateActionsForUnit(const ActionGenerationContext& context, uint8_t unitIdx);
