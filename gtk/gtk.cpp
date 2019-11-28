#include "gtk.hpp"

static Gtk *gtk;

/*===== CALLBACKS =====*/
void keyboardPressDownCallback(unsigned char key, int x, int y) {
  gtk->updateKeys(key, true);
}

void keyboardPressUpCallback(unsigned char key, int x, int y) {
  gtk->updateKeys(key, false);
}

void frameCallback() {
  gtk->frameHandler();
}

void timerCallback(int i) {
  gtk->timeHandler();
}

void resizeWindowCallback(GLsizei w, GLsizei h) {
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

  vector<Player *> *pl = this->client->getPlayerList()->getPlayers();
  Square s;
  RGB color;
  // Iterate every player
  for (int i = 0 ; i < (int)(*pl).size() ; i++) {

    // Update position
    s = (*pl)[i]->getPosition();

    // Cofigure player appearence
    color = (*pl)[i]->getColor();
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
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)((*pl)[i]->getName().c_str()));
  }
}

// Draw Obstacles
void Gtk::drawObstacles() {

  vector<Obstacle *> *o = this->client->getObstacleList()->getObstacles();
  Square s;
  
  // Cofigure player appearence
  glColor3f(0.5f, 0.5f, 0.5f); // Grey
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
  
  glLineWidth(4.0f);
  glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2i(s.x_min,s.y_min);
	glVertex2i(s.x_min,s.y_max);
	glVertex2i(s.x_max,s.y_max);
	glVertex2i(s.x_max,s.y_min);
  glEnd();

  // Team score
  glRasterPos2i(s.x_min, s.y_max+5);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Blue score: ");
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)to_string(this->client->getMap()->getBlueScore()).c_str());

  // Team score
  glRasterPos2i(s.x_max-20, s.y_max+5);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Red score: ");
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)to_string(this->client->getMap()->getRedScore()).c_str());

  // Red Basis (transparent area)
  glColor4f(1.0f, 0.0f, 0.0f, 0.25f); // Color
  s = this->client->getMap()->getRedBasis();
  glBegin(GL_QUADS);
  glVertex2i(s.x_min,s.y_min);
  glVertex2i(s.x_min,s.y_max);
  glVertex2i(s.x_max,s.y_max);
  glVertex2i(s.x_max,s.y_min);
  glEnd();

  // Blue Basis (transparent area)
  glColor4f(0.0f, 0.0f, 1.0f, 0.25f); // Color
  s = this->client->getMap()->getBlueBasis();
  glBegin(GL_QUADS);
  glVertex2i(s.x_min,s.y_min);
  glVertex2i(s.x_min,s.y_max);
  glVertex2i(s.x_max,s.y_max);
  glVertex2i(s.x_max,s.y_min);
  glEnd();

}

// Draw info
void Gtk::drawInfo() {
  // Player name at the bottom
  glRasterPos2i(-this->win+3, -this->win+3);
  glColor3f(0.0f, 0.0f, 0.0f);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Player: ");
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)this->client->getMyself()->getName().c_str());
}

// Update position every 20ms
void Gtk::timeHandler() {

  json data = this->client->getPackage();

  // Check if data has game content
  if(!data.empty()) {
    if(!data["is_game_status"].empty()) {    
      //Update players positions and update frame
      if(!data["map"].empty() && !data["players"].empty()) {
        this->client->getPlayerList()->update(data["players"]);
        this->client->getMap()->update(data["map"]);
        glutPostRedisplay();
      }
    }
  }
  
  // Set new callback
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
  this->keys["a"] = false;
  this->keys["w"] = false;
  this->keys["s"] = false;
  this->keys["d"] = false;
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(this->width,this->height);
  glutInitWindowPosition(0,00);
  glutCreateWindow("Capture the flag");
  glutDisplayFunc(frameCallback);
  cout << "GTK: Frame callback created!" << endl;
  glutReshapeFunc(resizeWindowCallback);
  cout << "GTK: Resize callback created!" << endl;
  glutIgnoreKeyRepeat(1);
  glutKeyboardFunc(keyboardPressDownCallback);
  cout << "GTK: Keydown callback created!" << endl;
  glutKeyboardUpFunc(keyboardPressUpCallback);
  cout << "GTK: Keyup callback created!" << endl;
  glutTimerFunc(33, timerCallback, 1);
  cout << "GTK: Time callback created!" << endl;
  
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLoadIdentity();
  gluOrtho2D(-this->win,this->win,-this->win,this->win);
  glMatrixMode(GL_MODELVIEW);
  glutMainLoop();
  cout << "GTK: successfully initiated!" << endl;
}

// Update keys
void Gtk::updateKeys(char key, bool is_pressed) {

  Player *p;
  json data;
  
  switch(key) {
  case 'a':
    if(is_pressed) this->keys["a"] = true;
    else this->keys["a"] = false;
    break;
  case 'w':
    if(is_pressed) this->keys["w"] = true;
    else this->keys["w"] = false;
    break;
  case 's':
    if(is_pressed) this->keys["s"] = true;
    else this->keys["s"] = false;
    break;
  case 'd':
    if(is_pressed) this->keys["d"] = true;
    else this->keys["d"] = false;
    break;
  default:
    cout << "GTK: Invalid key pressed!" << endl;
    return;
  }

  p = this->client->getMyself();
  p->setDirection(this->keys);

  data["id"] = p->getId();
  data["keys"] = this->keys;
  cout << "GTK: new key signal:" << endl;
  cout << data.dump(4) << endl;
  this->client->sendPackage(data.dump());
}
