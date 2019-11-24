#include "map.hpp"

Map::Map(Square boundaries, Square basis_blue, Square basis_red) {
  this->playing = true;
  this->victory = false;
  this->defeat = false;
  this->boundaries = boundaries;
  this->basis_blue = basis_blue;
  this->basis_red = basis_red;
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
  this->basis_blue = {
                 map["basis_blue"]["x_max"].get<float>(),
                 map["basis_blue"]["y_max"].get<float>(),
                 map["basis_blue"]["x_min"].get<float>(),
                 map["basis_blue"]["y_min"].get<float>()
  };
  this->basis_red = {
                 map["basis_red"]["x_max"].get<float>(),
                 map["basis_red"]["y_max"].get<float>(),
                 map["basis_red"]["x_min"].get<float>(),
                 map["basis_red"]["y_min"].get<float>()
  };
}

Square Map::getBoundaries() {
  return this->boundaries;
}

Square Map::getBlueBasis() {
  return this->basis_blue;
}

Square Map::getRedBasis() {
  return this->basis_red;
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
  map["basis_blue"]["x_max"] = this->basis_blue.x_max;
  map["basis_blue"]["y_max"] = this->basis_blue.y_max;
  map["basis_blue"]["x_min"] = this->basis_blue.x_min;
  map["basis_blue"]["y_min"] = this->basis_blue.y_min;
  map["basis_red"]["x_max"] = this->basis_red.x_max;
  map["basis_red"]["y_max"] = this->basis_red.y_max;
  map["basis_red"]["x_min"] = this->basis_red.x_min;
  map["basis_red"]["y_min"] = this->basis_red.y_min;

  return map;
}
