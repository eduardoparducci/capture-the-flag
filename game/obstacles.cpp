#include "obstacles.hpp"

Obstacle::Obstacle(Square p, Dynamic d) {
  this->position = p;
  this->dynamic = d;
}

Obstacle::Obstacle(json obstacle) {
  this->position = {
                    obstacle["position"]["x_max"].get<float>(),
                    obstacle["position"]["y_max"].get<float>(),
                    obstacle["position"]["x_min"].get<float>(),
                    obstacle["position"]["y_min"].get<float>()
  };
  this->dynamic = {
                   obstacle["dynamic"]["v_x"].get<float>(),
                   obstacle["dynamic"]["v_y"].get<float>(),
                   obstacle["dynamic"]["w"].get<float>()
  };
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

json Obstacle::serialize() {
  json obstacle;
  obstacle["position"]["x_max"] = this->position.x_max;
  obstacle["position"]["x_min"] = this->position.x_min;
  obstacle["position"]["y_max"] = this->position.y_max;
  obstacle["position"]["y_min"] = this->position.y_min;
  obstacle["dynamic"]["v_x"] = this->dynamic.v_x;
  obstacle["dynamic"]["v_y"] = this->dynamic.v_y;
  obstacle["dynamic"]["w"] = this->dynamic.w;
  return obstacle;
}

ObstacleList::ObstacleList() {
  this->obstacles = new vector<Obstacle *>(0);
}

ObstacleList::ObstacleList(json obstacles) {
  int i;
  Obstacle *o;
  
  // Create empty vector
  this->obstacles = new vector<Obstacle *>(0);

  // Fill vector with json list
  for(i=0 ; i<(int)obstacles.size() ; i++) {
    o = new Obstacle(obstacles[i]);
    addObstacle(o);
  }
}

json ObstacleList::serialize() {
  json obstacles;
  vector<Obstacle *> *o = this->obstacles;
  for (int i = 0 ; i < (int)(*o).size() ; i++) {
    obstacles.push_back((*o)[i]->serialize());
  }
  return obstacles;
}

void ObstacleList::addObstacle(Obstacle *o) {
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
