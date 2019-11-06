/* Desenvolvido por: Eduardo  (170272)
 *                   Henrique (174638)
 */

#ifndef OO_MODEL_HPP
#define OO_MODEL_HPP

#include <thread>
#include <vector>
#include <map>
#include <unistd.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "json.hpp"

struct Dynamic {
  float v_x;
  float v_y;
  float w;
};

struct Coordinate {
  float x;
  float y;
};

struct RGB {
  float r;
  float g;
  float b;
};

struct Square {
  float x_max;
  float y_max;
  float x_min;
  float y_min;
};

class Player {
private:
  Square position;
  RGB color;
  std::string name;
  std::map<char, bool> direction;

  nlohmann::json directionJson;

public:
  Player(float x, float y, float height, float width, std::string name, RGB color);
  void update(Square p);
  void resize(Square position);
  void toString();
  Square getPosition();
  RGB getColor();
  void setDirection(char c, bool value);
  void setDirectionJson(nlohmann::json movement);
  nlohmann::json getDirectionJson();
  std::map<char, bool> getDirection();
  std::string getName();
  std::string serialize();
  std::string toJson();  

};

class Obstacle {
private:
  Square position;
  Dynamic dynamic;
public:
  Obstacle(Square p, Dynamic d);
  Dynamic getDynamic();
  Square  getPosition();
  void update(Square p, Dynamic d);
  bool hit(Square position);
};

class ObstacleList {
private:
  std::vector<Obstacle *> *obstacles;

public:
  ObstacleList();
  void add_obstacle(Obstacle *o);
  bool hit(Square position);
  std::vector<Obstacle*> *getObstacles();
};

class Map {
private:
  // Map Dimension
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

class Physics {
private:
  Player *player;
  Map *map;
  ObstacleList *obstacles;
public:
  Physics(Player *player, Map *map, ObstacleList *obstacles);
  Player *getPlayer();
  void update(char key, bool value);
  void updateJson(nlohmann::json movement);
};
#endif
