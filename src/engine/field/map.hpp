#include <array>
#include <vector>

#include "hexData.hpp"


struct HexCoord {
  int q;
  int r;
};


class Map {
 private:
  int width;
  int height;
  std::vector<HexData> hexes;
  std::vector<std::array<int, 6>> neighbors;

 public:
  const HexData& get(HexCoord coord) const;
  const HexData& get(int index) const;

  int toIndex(HexCoord coord) const;
  HexCoord toCoord(int index) const;

  int getNeighborIndex(int index, Direction dir) const;
};
