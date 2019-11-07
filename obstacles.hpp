#ifndef OBSTACLES_HPP
#define OBSTACLES_HPP

// external libraries
#include "libraries.hpp"

// header files
#include "data/structures.hpp"

using namespace std;

class Obstacle {
private:
  Square position;
  Dynamic dynamic;
public:
  Obstacle(Square p, Dynamic d);
  Dynamic getDynamic();
  Square getPosition();
  void update(Square p, Dynamic d);
  bool hit(Square position);
};

class ObstacleList {
private:
  vector<Obstacle *> *obstacles;

public:
  ObstacleList();
  void add_obstacle(Obstacle *o);
  bool hit(Square position);
  vector<Obstacle*> *getObstacles();
};


#endif
