#include "players.hpp"

using namespace std;
using json = nlohmann::json;

Player::Player(float x, float y, float height, float width, string name, RGB color, unsigned id) {
  this->name = name;
  this->id = id;
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

void Player::setId(unsigned id) {
  this->id = id;
}

void Player::resize(Square p) {
  this->position = p;
}

void Player::update(Square p) {
  this->position.x_max = p.x_max;
  this->position.y_max = p.y_max;
  this->position.x_min = p.x_min;
  this->position.y_min = p.y_min;
}

void Player::setDirection(json keys) {
  this->direction['a'] = keys["a"].get<bool>();
  this->direction['w'] = keys["w"].get<bool>();
  this->direction['s'] = keys["s"].get<bool>();
  this->direction['d'] = keys["d"].get<bool>();
}

void Player::toString() {
  cout << "Player: " << this->name << endl;
  cout << "  Color: RGB(" << this->color.r << "," << this->color.g << "," << this->color.b << ")" << endl;
  cout << "  Position: XYxy(" << this->position.x_max << \
                               "," << this->position.y_max << \
                               "," << this->position.x_min << \
                               "," << this->position.y_min << \
                               ")" << endl;
}

string Player::getName() {
  return this->name;
}

json Player::serialize() {
  json player;
  player["id"] = this->id;
  player["name"] = this->name;
  player["position"]["x_max"] = this->position.x_max;
  player["position"]["x_min"] = this->position.x_min;
  player["position"]["y_max"] = this->position.y_max;
  player["position"]["y_min"] = this->position.y_min;
  return player;
}

unsigned Player::getId() {
  return this->id;
}

map<char,bool> Player::getDirection() {
  return this->direction;
}

Square Player::getPosition() {
  return this->position;
}

RGB Player::getColor() {
  return this->color;
}
