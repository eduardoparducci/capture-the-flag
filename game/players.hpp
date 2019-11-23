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
  Player(json player);
  void setId(unsigned id);
  void update(Square p);
  void resize(Square position);
  void toString();
  void setDirection(json keys);
  string getName();
  json serialize();
  unsigned getId();
  map<char, bool> getDirection();
  Square getPosition();
  RGB getColor();
};

class PlayerList {
private:
  vector<Player *> *players;

public:
  PlayerList();
  PlayerList(json players);
  unsigned addPlayer(Player *p);
  json serialize();
  vector<Player*> *getPlayers();
  Player *getPlayer(unsigned id);
  void update(json players);
};

#endif
