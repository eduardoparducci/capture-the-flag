/* Desenvolvido por: Eduardo  (170272)
 *                   Henrique (174638)
 */

#include <vector>
#include "game.hpp"
#include <iostream>
#include <cmath>
#include <ncurses.h>
#include <chrono>
#include <thread>

Player::Player(float x, float y, float height, float width, std::string name, RGB color) {
  this->name = name;
  this->color = color;

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
  // 0.8 added empirically
  if(position.x_max-0.8>=this->position.x_min &&
     position.x_min+0.8<=this->position.x_max &&
     position.y_max-0.8>=this->position.y_min &&
     position.y_min+0.8<=this->position.y_max) {
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

// Physics::Physics(Player *p1, Map *m1, ObstacleList *obs) {
//   this->p1 = p1;
//   this->m1 = m1;
//   this->obs = obs;
// }

// void Physics::walk(char direction) {
//   int x = this->p1->getX();
//   int y = this->p1->getY();
//   if(m1->is_playing()) {
//     switch(direction) {
//     case 's':
//       if(m1->is_valid(x, y+1)) this->p1->update(x, ++y);
//       break;
//     case 'w':
//       if(m1->is_valid(x, y-1)) this->p1->update(x, --y);
//       break;
//     case 'd':
//       if(m1->is_valid(x+1, y)) this->p1->update(++x, y);
//       break;
//     case 'a':
//       if(m1->is_valid(x-1, y)) this->p1->update(--x, y);
//       break;
//     }
//   }

//   // Verify victory
//   if(y > this->m1->get_victory_line())
//     this->m1->terminate(true);

//   // Verify defeat
//   if(this->obs->hit(x,y))
//     this->m1->terminate(false);
// }

// void Physics::update(float deltaT) {

//   std::vector<Obstacle *> *o = this->obs->get_obstacles();

//   for (int i = 0; i < (int)(*o).size(); i++) {

//     float vx  = (*o)[i]->get_vx();
//     float vy  = (*o)[i]->get_vy();    
//     float new_x = (*o)[i]->get_x() + deltaT * vx/1000;
//     float new_y = (*o)[i]->get_y() + deltaT * vy/1000;

//     if(this->m1->is_valid((int)new_x, (int)new_y)==false) {
//       vx  = -vx;
//       vy  = -vy;
//       new_x = (*o)[i]->get_x();
//       new_y = (*o)[i]->get_y();
//     }
//     (*o)[i]->update(new_x, new_y, vx, vy);   
//   }
// }
