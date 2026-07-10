#include "../field/hex.hpp"
#include "../nations.hpp"


class ControlMarker {
 private:
  HexCoord position;
  Nation owner;

  void check_own(Nation nation);
  void change_owner();


 public:
  void ownSoviet();
  void ownGermany();

  HexCoord getPosition() const;
  Nation getOwner() const;
};
