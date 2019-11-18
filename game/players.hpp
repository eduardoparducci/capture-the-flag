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
  unsigned id;
  map<char, bool> direction;

public:
  Player(float x, float y, float height, float width, string name, RGB color, unsigned id);
  void setId(unsigned id);
  void update(json p);
  void resize(Square position);
  void toString();
  void setDirection(char c, bool value);
  string getName();
  string serialize();
  unsigned getId();
  map<char, bool> getDirection();
  json toJson();
  Square getPosition();
  RGB getColor();
};
#endif
