#ifndef MAP_HPP
#define MAP_HPP

// external libraries
#include "../libraries.hpp"
#include "../data/json.hpp"

// header files
#include "../data/structures.hpp"

using namespace std;
using json = nlohmann::json;

class Map {
private:
  Square boundaries;
  Square basis_blue;
  Square basis_red;
  bool playing;
  bool victory;
  bool defeat;
  unsigned score_blue;
  unsigned score_red;

public:
  Map(Square boundaries, Square basis_blue, Square basis_red);
  Map(json map);
  Square getBlueBasis();
  Square getRedBasis();
  unsigned getBlueScore();
  unsigned getRedScore();
  Square getBoundaries();
  bool isValid(Square p);
  bool isVictory();
  bool isDefeat();
  bool isPlaying();
  json serialize();
};

#endif
