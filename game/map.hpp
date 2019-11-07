#ifndef MAP_HPP
#define MAP_HPP

// external libraries
#include "../libraries.hpp"

// header files
#include "../data/structures.hpp"

class Map {
private:
  Square boundaries;
  Square basis;
  bool playing;
  bool victory;
  bool defeat;

public:
  Map(Square boundaries, Square basis);
  Square getBasis();
  Square getBoundaries();
  bool isValid(Square p);
  bool isVictory();
  bool isDefeat();
  bool isPlaying();
};

#endif
