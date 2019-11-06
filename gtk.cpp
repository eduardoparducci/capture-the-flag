#include "gtk.hpp"
#include "game.hpp"
#include "client.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <map>
#include <thread>

// Needs to be static because glut library has no access to internal GTK members
static Gtk *gtk;

/*===== CALLBACKS =====*/
// Need to be global function because because glut has no access to local methods

void keyboardPressDownCallback(unsigned char key, int x, int y) {
  //std::cout << "Key down callback!" << std::endl;
  gtk->updateKeys(key, true);
}

void keyboardPressUpCallback(unsigned char key, int x, int y) {
  //std::cout << "Key up callback!" << std::endl;
  gtk->updateKeys(key, false);
}

void frameCallback() {
  //std::cout << "Frame callback!" << std::endl;
  gtk->frameHandler();
}

void timerCallback(int i) {
  gtk->timeHandler();
}

void resizeWindowCallback(GLsizei w, GLsizei h) {
  //std::cout << "Resize callback!" << std::endl;
  gtk->resizeWindowHandler(w,h);
}

/*===== GTK methods =====*/

// Resize window item according to window size
void Gtk::resizeWindowHandler(GLsizei w, GLsizei h) {
  // Viewport size
  this->width = w;
  this->height = h;
  glViewport(0, 0, this->width, this->height);
  // coordinate system init
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D (-this->win, this->win, -this->win, this->win);
}

// Update frame
void Gtk::frameHandler() {
  glClear(GL_COLOR_BUFFER_BIT);
  this->drawPlayer();
  this->drawObstacles();
  this->drawInfo();
  this->drawMap();
  // Update frame
  glutSwapBuffers();
}

// Draw grid on the screen
void Gtk::gridOn() {
  glColor3f(0.5f,0.5f,0.5f);
  glLineWidth(1.0f);
  glBegin(GL_LINES);
  float x = -this->win;
  for(;x<=this->win; x+=10) {
    glVertex2f(-this->win,x);
    glVertex2f( this->win,x);
    glVertex2f(x,-this->win);
    glVertex2f(x, this->win);
  }
  glEnd();
  glColor3f(0.0f,0.0f,0.0f);
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex2f(-this->win,0);
  glVertex2f( this->win,0);
  glVertex2f(0,-this->win);
  glVertex2f(0, this->win);
  glEnd();

}

// Draw player
void Gtk::drawPlayer() {
  //Square s = this->s;
  RGB color = this->client->getPlayer()->getColor();
  Square s = this->client->getPlayer()->getPosition();

  // Cofigure player appearence
  glColor3f(color.r, color.g, color.b); // Color

  // Draw dots on the screen
  glBegin(GL_QUADS);
  glVertex2i(s.x_min,s.y_min);
  glVertex2i(s.x_min,s.y_max);
  glVertex2i(s.x_max,s.y_max);
  glVertex2i(s.x_max,s.y_min);
  glEnd();

  // Player name
  glRasterPos2i(s.x_min, s.y_max+2);
  glColor3f(0.0f, 0.0f, 0.0f);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)(this->client->getPlayer()->getName().c_str()));
}

// Draw player
void Gtk::drawObstacles() {

  std::vector<Obstacle *> *o = this->client->getObstacleList()->getObstacles();
  Square s;

  // Cofigure player appearence
  glColor3f(0.0f, 0.0f, 0.0f); // Black
  for (int i = 0 ; i < (int)(*o).size() ; i++) {
    s = (*o)[i]->getPosition();
    glBegin(GL_QUADS);
    glVertex2i(s.x_min,s.y_min);
    glVertex2i(s.x_min,s.y_max);
    glVertex2i(s.x_max,s.y_max);
    glVertex2i(s.x_max,s.y_min);
    glEnd();
  }
}

// Draw map
void Gtk::drawMap() {
  Square s = this->client->getMap()->getBoundaries();
  RGB color = this->client->getPlayer()->getColor();
  
  glLineWidth(4.0f); //width of line
  glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2i(s.x_min,s.y_min);
	glVertex2i(s.x_min,s.y_max);
	glVertex2i(s.x_max,s.y_max);
	glVertex2i(s.x_max,s.y_min);
  glEnd();

  // Basis
  glColor4f(color.r, color.g, color.b, 0.25f); // Color
  s = this->client->getMap()->getBasis();
  glBegin(GL_QUADS);
  glVertex2i(s.x_min,s.y_min);
  glVertex2i(s.x_min,s.y_max);
  glVertex2i(s.x_max,s.y_max);
  glVertex2i(s.x_max,s.y_min);
  glEnd();


}

// Draw info at the bottom of the screen
void Gtk::drawInfo() {
  glRasterPos2i(-this->win+3, -this->win+3);
  glColor3f(0.0f, 0.0f, 0.0f);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Player: ");
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)this->client->getPlayer()->getName().c_str());
}

// Update position every 20ms
void Gtk::timeHandler() {
  std::string data = this->client->getString();
  std::string Xmax, Ymax, Xmin, Ymin;
  unsigned first, last;
  if(data.length()) {
    first = data.find("a")+1;
    last = data.find("b");
    Xmax = data.substr(first,last-first);
    first = data.find("b")+1;
    last = data.find("c");
    Ymax = data.substr(first,last-first);
    first = data.find("c")+1;
    last = data.find("d");
    Xmin = data.substr(first,last-first);
    first = data.find("d")+1;
    Ymin = data.substr(first);
    //std::cout << "XM=" << Xmax << " YM=" << Ymax << " Xm=" << Xmin << " Ym=" << Ymin << std::endl;
    if(Xmax.length() && Xmin.length() && Ymin.length() && Ymax.length()) {
      Square s = {std::stof(Xmax),std::stof(Ymax),std::stof(Xmin),std::stof(Ymin)};
      this->client->getPlayer()->update(s);
    }
  }
  glutPostRedisplay();
  glutTimerFunc(20, timerCallback, 1);
}

// Initial config
void Gtk::init(int argc, char **argv, Client *c) {
  gtk = this;
  this->xf = 3;
  this->yf = 3;
  this->win = 150.0f;
  this->height = 500;
  this->width = 700;
  this->client = c;
  this->s = this->client->getPlayer()->getPosition();
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(this->width,this->height);
  glutInitWindowPosition(0,00);
  glutCreateWindow("Capture the flag");
  glutDisplayFunc(frameCallback);
  std::cout << "GTK: Frame callback created!" << std::endl;
  glutReshapeFunc(resizeWindowCallback);
  std::cout << "GTK: Resize callback created!" << std::endl;
  glutIgnoreKeyRepeat(1);
  glutKeyboardFunc(keyboardPressDownCallback);
  std::cout << "GTK: Keydown callback created!" << std::endl;
  glutKeyboardUpFunc(keyboardPressUpCallback);
  std::cout << "GTK: Keyup callback created!" << std::endl;
  glutTimerFunc(33, timerCallback, 1);
  std::cout << "GTK: Time callback created!" << std::endl;
  //std::thread newthread(timerThread, &(this->client));
  //(this->time_thread).swap(newthread);
  
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLoadIdentity();
  gluOrtho2D(-this->win,this->win,-this->win,this->win);
  glMatrixMode(GL_MODELVIEW);
  glutMainLoop();
  std::cout << "GTK: successfully initiated!" << std::endl;
}

// Update keys
void Gtk::updateKeys(char key, bool is_pressed) {
  std::string data(1,key);
  if(is_pressed) data.append("+");
  else           data.append("-");
  this->client->sendString(data);
}
