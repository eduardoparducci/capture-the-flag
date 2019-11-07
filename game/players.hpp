#ifndef PLAYER_HPP
#define PLAYER_HPP

// external libraries
#include "../libraries.hpp"
#include "../data/json.hpp"
#include "../data/structures.hpp"

using json = nlohmann::json;
using namespace std;

class Player {
private:
  Square position;
  RGB color;
  string name;
  map<char, bool> direction;
  json directionJson;

public:
  Player(float x, float y, float height, float width, string name, RGB color);
  void update(Square p);
  void resize(Square position);
  void toString();
  void setDirection(char c, bool value);
  void setDirectionJson(json movement);
  string getName();
  string serialize();
  string toJson();  
  json getDirectionJson();
  map<char, bool> getDirection();
  Square getPosition();
  RGB getColor();
};
#endif
