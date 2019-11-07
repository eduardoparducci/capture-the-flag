#include "map.hpp"

Map::Map(Square boundaries, Square basis) {
  this->playing = true;
  this->victory = false;
  this->defeat = false;
  this->boundaries = boundaries;
  this->basis = basis;
}

Square Map::getBoundaries() {
  return this->boundaries;
}

Square Map::getBasis() {
  return this->basis;
}

bool Map::isValid(Square p) {
  if(p.y_max-1>this->boundaries.y_max) return false;
  if(p.y_min+1<this->boundaries.y_min) return false;
  if(p.x_max-1>this->boundaries.x_max) return false;
  if(p.x_min+1<this->boundaries.x_min) return false;
  return true;
}

bool Map::isVictory() {
  return this->victory;
}

bool Map::isDefeat() {
  return this->defeat;
}

bool Map::isPlaying() {
  return this->playing;
}
