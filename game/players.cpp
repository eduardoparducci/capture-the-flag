#include "players.hpp"

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

void Player::update(json p) {
  this->position.x_max = p.at("x_max");
  this->position.x_max = p.at("y_max");
  this->position.x_max = p.at("x_min");
  this->position.x_max = p.at("y_min");
}

void Player::setDirection(char c, bool value) {
  this->direction[c] = value;
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

string Player::serialize() {
  string data("a");
  data.append(to_string(this->position.x_max));
  data.append("b");
  data.append(to_string(this->position.y_max));
  data.append("c");
  data.append(to_string(this->position.x_min));
  data.append("d");
  data.append(to_string(this->position.y_min));
  return data;
}

unsigned Player::getId() {
  return this->id;
}

map<char,bool> Player::getDirection() {
  return this->direction;
}

json Player::toJson() {
  json player;
  player["id"] = this->id;
  player["name"] = this->name;
  player["position"]["x_max"]  = this->position.x_max;
  player["position"]["x_min"]  = this->position.x_min;
  player["position"]["y_max"]  = this->position.y_max;
  player["position"]["y_min"]  = this->position.y_min;
  player["direction"]["up"]    = this->direction['w']; 
  player["direction"]["down"]  = this->direction['s'];
  player["direction"]["left"]  = this->direction['a'];
  player["direction"]["right"] = this->direction['d'];
  return player;
}

Square Player::getPosition() {
  return this->position;
}

RGB Player::getColor() {
  return this->color;
}
