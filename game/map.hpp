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
  Square basis;
  bool playing;
  bool victory;
  bool defeat;

public:
  Map(Square boundaries, Square basis);
  Map(json map);
  Square getBasis();
  Square getBoundaries();
  bool isValid(Square p);
  bool isVictory();
  bool isDefeat();
  bool isPlaying();
  json serialize();
};

#endif
