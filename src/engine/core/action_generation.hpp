#pragma once

#include <map>

#include "../action/action.hpp"
#include "state.hpp"


using HexUnitsMap = std::map<HexCoord, std::vector<uint8_t>>;

struct GenerationContext {
  const State& state;
  HexUnitsMap& units_in_hex;
  std::vector<Actions::Action>& actions;
};


HexUnitsMap getUnitsInHexes(const State& state);
bool isAnyEnemyUnitInHex(const GenerationContext& context, HexCoord hex);

void addActionsForUnit(const GenerationContext& context, uint8_t unitIdx);
void removeActionsCantPay(const GenerationContext& context);
