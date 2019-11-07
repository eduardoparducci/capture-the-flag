#include "obstacles.hpp"

Obstacle::Obstacle(Square p, Dynamic d) {
  this->position = p;
  this->dynamic = d;
}

Square Obstacle::getPosition() {
  return this->position;
}

Dynamic Obstacle::getDynamic() {
  return this->dynamic;
}

void Obstacle::update(Square p, Dynamic d) {
  this->position = p;
  this->dynamic = d;
}

bool Obstacle::hit(Square position) {
  // 0.4 added empirically
  if(position.x_max-0.4>=this->position.x_min &&
     position.x_min+0.4<=this->position.x_max &&
     position.y_max-0.4>=this->position.y_min &&
     position.y_min+0.4<=this->position.y_max) {
    return true;
  } else {
  return false;
  }
}

ObstacleList::ObstacleList() {
  this->obstacles = new vector<Obstacle *>(0);
}

void ObstacleList::add_obstacle(Obstacle *o) {
  (this->obstacles)->push_back(o);
}

bool ObstacleList::hit(Square position) {
  vector<Obstacle *> *o = this->obstacles;
  for (int i = 0 ; i < (int)(*o).size() ; i++)
    if((*o)[i]->hit(position)) return true;
  return false;
}

vector<Obstacle*> *ObstacleList::getObstacles(){
  return this->obstacles;
}
