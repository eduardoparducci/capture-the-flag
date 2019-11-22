#ifndef GTK_HPP
#define GTK_HPP

// external libraries
#include "../libraries.hpp"
#include "../game/players.hpp"
#include "../network/client.hpp"
#include "../data/json.hpp"

using json = nlohmann::json;
using namespace std;

class Gtk {
private:
  Client *client;
  Square s;
  
  // Window info
  GLfloat xf;
  GLfloat yf;
  GLfloat win;
  GLint height;
  GLint width;

  // Keyboard info
  std::map<char, bool> pressed_keys;
  json keys;
  
public:
  void init(int argc, char **argv, Client *c);
  void frameHandler();
  void timeHandler();
  void resizeWindowHandler(GLsizei w, GLsizei h);
  void setX(GLfloat x);
  void setY(GLfloat y);
  void updateKeys(char key, bool is_pressed);
  void drawPlayer();
  void drawObstacles();
  void drawMap();
  void drawInfo();
  void gridOn();
  void update();
  GLfloat getX();
  GLfloat getY();
};
#endif

