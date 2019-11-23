#include "map.hpp"

Map::Map(Square boundaries, Square basis) {
  this->playing = true;
  this->victory = false;
  this->defeat = false;
  this->boundaries = boundaries;
  this->basis = basis;
}

Map::Map(json map) {
  this->playing = map["playing"].get<bool>();
  this->victory = map["victory"].get<bool>();
  this->defeat = map["defeat"].get<bool>();
  this->boundaries = {
                      map["boundaries"]["x_max"].get<float>(),
                      map["boundaries"]["y_max"].get<float>(),
                      map["boundaries"]["x_min"].get<float>(),
                      map["boundaries"]["y_min"].get<float>()
  };
  this->basis = {
                 map["basis"]["x_max"].get<float>(),
                 map["basis"]["y_max"].get<float>(),
                 map["basis"]["x_min"].get<float>(),
                 map["basis"]["y_min"].get<float>()
  };
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

json Map::serialize() {
  json map;
  map["playing"] = this->playing;
  map["victory"] = this->victory;
  map["defeat"] = this->defeat;
  map["boundaries"]["x_max"] = this->boundaries.x_max;
  map["boundaries"]["y_max"] = this->boundaries.y_max;
  map["boundaries"]["x_min"] = this->boundaries.x_min;
  map["boundaries"]["y_min"] = this->boundaries.y_min;
  map["basis"]["x_max"] = this->basis.x_max;
  map["basis"]["y_max"] = this->basis.y_max;
  map["basis"]["x_min"] = this->basis.x_min;
  map["basis"]["y_min"] = this->basis.y_min;
  return map;
}
