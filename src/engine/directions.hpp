#pragma once

#include <cstdint>


enum Direction : uint8_t { North, NorthEast, SouthEast, South, SouthWest, NorthWest };


/*                    N
                NW         NE

                SW         SE
                      S
*/

inline constexpr Direction getOppositeTo(Direction direction) {
  switch (direction) {
    case North: return South;
    case NorthEast: return SouthWest;
    case SouthEast: return NorthWest;
    case South: return North;
    case SouthWest: return NorthEast;
    case NorthWest: return SouthEast;
  }
}
