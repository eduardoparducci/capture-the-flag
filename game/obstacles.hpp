#ifndef OBSTACLES_HPP
#define OBSTACLES_HPP

// external libraries
#include "../libraries.hpp"

// header files
#include "../data/structures.hpp"
#include "../data/json.hpp"

using namespace std;
using json = nlohmann::json;

class Obstacle {
private:
  Square position;
  Dynamic dynamic;
public:
  Obstacle(Square p, Dynamic d);
  Obstacle(json obstacle);
  Dynamic getDynamic();
  Square getPosition();
  void update(Square p, Dynamic d);
  bool hit(Square position);
  json serialize();
};

class ObstacleList {
private:
  vector<Obstacle *> *obstacles;

public:
  ObstacleList();
  ObstacleList(json obstacles);
  void addObstacle(Obstacle *o);
  bool hit(Square position);
  json serialize();
  vector<Obstacle*> *getObstacles();
};


#endif
