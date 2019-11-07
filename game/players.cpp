#include "players.hpp"

Player::Player(float x, float y, float height, float width, string name, RGB color) {
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

string Player::getName() {
  return this->name;
}

RGB Player::getColor() {
  return this->color;
}

void Player::update(Square p) {
  this->position = p; 
}

map<char,bool> Player::getDirection() {
  return this->direction;
}

json Player::getDirectionJson() {
  return this->directionJson;
}

void Player::setDirection(char c, bool value) {
  this->direction[c] = value;
}

void Player::setDirectionJson(json movement) {
  this->directionJson[movement.begin().key()] = movement.begin().value();
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

string Player::toJson() {
  json data;
  data["name"] = this->name;
  return data.dump();
}
