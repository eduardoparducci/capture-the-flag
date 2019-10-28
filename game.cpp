/* Desenvolvido por: Eduardo  (170272)
 *                   Henrique (174638)
 */

#include <vector>
#include "game.hpp"
#include <iostream>
#include <map>
#include <cmath>
#include <ncurses.h>
#include <chrono>
#include <thread>

Player::Player(float x, float y, float height, float width, std::string name, RGB color) {
  this->name = name;
  this->color = color;
  this->direction['a'] = false;
  this->direction['w'] = false;
  this->direction['s'] = false;
  this->direction['d'] = false;

  //Calculate coordinates
  this->position.y_max = y+height/2;
  this->position.y_min = y-height/2;
  this->position.x_max = x+width/2;
  this->position.x_min = x-width/2;
}

void Player::resize(Square p) {
  this->position = p;
}

Square Player::getPosition() {
  return this->position;
}

std::string Player::getName() {
  return this->name;
}

RGB Player::getColor() {
  return this->color;
}

void Player::update(Square p) {
  this->position = p; 
}

std::map<char,bool> Player::getDirection() {
  return this->direction;
}

void Player::setDirection(char c, bool value) {
  this->direction[c] = value;
}

void Player::toString() {
  std::cout << "Player: " << this->name << std::endl;
  std::cout << "  Color: RGB(" << this->color.r << "," << this->color.g << "," << this->color.b << ")" << std::endl;
  std::cout << "  Position: XYxy(" << this->position.x_max << \
                               "," << this->position.y_max << \
                               "," << this->position.x_min << \
                               "," << this->position.y_min << \
                               ")" << std::endl;
}

std::string Player::serialize() {
  std::string data("a");
  data.append(std::to_string(this->position.x_max));
  data.append("b");
  data.append(std::to_string(this->position.y_max));
  data.append("c");
  data.append(std::to_string(this->position.x_min));
  data.append("d");
  data.append(std::to_string(this->position.y_min));
  return data;
}

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
  this->obstacles = new std::vector<Obstacle *>(0);
}

void ObstacleList::add_obstacle(Obstacle *o) {
  (this->obstacles)->push_back(o);
}

bool ObstacleList::hit(Square position) {
  std::vector<Obstacle *> *o = this->obstacles;
  for (int i = 0 ; i < (int)(*o).size() ; i++)
    if((*o)[i]->hit(position)) return true;
  return false;
}

std::vector<Obstacle*> *ObstacleList::getObstacles(){
  return this->obstacles;
}

Map::Map(Square boundaries, Square basis) {
  this->playing = true;
  this->victory = false;
  this->defeat = false;
  this->boundaries = boundaries;
  this->basis = basis;
}

Square Map::getBoundaries() {
  return this->boundaries;
}

Square Map::getBasis() {
  return this->basis;
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

Physics::Physics(Player *player, Map *map, ObstacleList *obstacles) {
   this->player = player;
   this->map = map;
   this->obstacles = obstacles;
 }

void Physics::update(char key, bool value) {
  this->player->setDirection(key, value);
  Square s = this->player->getPosition();
  std::map<char, bool> d = this->player->getDirection();
  if(d['a']) {
    s.x_min -= 0.8;
    s.x_max -= 0.8;
  }
  if(d['d']) {
    s.x_min += 0.8;
    s.x_max += 0.8;
  }
  if(d['w']) {
    s.y_min += 0.8;
    s.y_max += 0.8;
  }
  if(d['s']) {
    s.y_min -= 0.8;
    s.y_max -= 0.8;
  }
  if(this->map->isValid(s) && !this->obstacles->hit(s)) {
    // std::cout << "Player position: (" << s.y_max << "," << s.x_max << ")" << std::endl; 
    this->player->update(s);
  } // else std::cout << "Player blocked at: (" << s.y_max << "," << s.x_max << ")" << std::endl;
}

Player *Physics::getPlayer(){
  return this->player;
}
