/* Desenvolvido por: Eduardo  (170272)
 *                   Henrique (174638)
 */

#ifndef OO_MODEL_HPP
#define OO_MODEL_HPP

#include <thread>
#include <vector>
#include <unistd.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

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
  
public:
  Player(float x, float y, float height, float width, std::string name, RGB color);
  void update(Square p);
  void resize(Square position);
  Square getPosition();
  RGB getColor();
  std::string getName();
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

// class Physics {
// private:
//   Player *p1;
//   Map *m1;
//   ObstacleList *obs;

// public:
//   Physics(Player *p1, Map *m1, ObstacleList *obs);
//   void walk(char direction);
//   void update(float deltaT);
// };
#endif
