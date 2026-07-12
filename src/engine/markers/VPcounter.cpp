#include <cstdint>
#include <cstdlib>

#include "VPcounter.hpp"
#include "nations.hpp"


void VPcounter::countSoviet(int8_t vp) {
  count_for(Nation::SovietUnion, vp);
}
void VPcounter::countGermany(int8_t vp) {
  count_for(Nation::Germany, vp);
}

void VPcounter::count_for(Nation nation, int8_t vp) {
  if (nation != leader && vp >= count) change_leader();
  count = std::abs(vp);
}

void VPcounter::change_leader() {
  leader = (leader == Nation::Germany) ? Nation::SovietUnion : Nation::Germany;
}


std::pair<Nation, int8_t> VPcounter::getVP() const {
  return {getLeader(), getCount()};
}
Nation VPcounter::getLeader() const {
  return leader;
}
int8_t VPcounter::getCount() const {
  return count;
}
