#pragma once

#include <cstdint>
#include <utility>

#include "../nations.hpp"


class VPcounter {
 private:
  Nation leader;
  int8_t count;

  void count_for(Nation nation, int8_t vp);
  void change_leader();


 public:
  VPcounter(Nation leader, int8_t count);


  void countSoviet(int8_t vp);
  void countGermany(int8_t vp);

  std::pair<Nation, int8_t> getVP() const;
  Nation getLeader() const;
  int8_t getCount() const;
};
