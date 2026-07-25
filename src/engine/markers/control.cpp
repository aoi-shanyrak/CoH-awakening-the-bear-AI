#include "control.hpp"


void ControlMarker::ownSoviet() {
  check_own(Nation::SovietUnion);
}
void ControlMarker::ownGermany() {
  check_own(Nation::Germany);
}

void ControlMarker::check_own(Nation nation) {
  if (owner != nation) change_owner();
}
void ControlMarker::change_owner() {
  owner = (owner == Nation::Germany) ? Nation::SovietUnion : Nation::Germany;
}


HexCoord ControlMarker::getPosition() const {
  return position;
}
Nation ControlMarker::getOwner() const {
  return owner;
}
