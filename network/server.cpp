#include "server.hpp"

void wait_package(char **buffer, int buffer_size, bool *buffer_status, bool *running, int connection_fd) {
  while(*running) {
    if(*buffer_status==FREE) {
      *buffer[0]= '\0';
      recv(connection_fd, *buffer, buffer_size, 0);
      *buffer_status = BUSY;
    }
    this_thread::sleep_for (chrono::milliseconds(10));
  }
  return;
}

Server::Server(unsigned int gate, string ip, unsigned int buffer_size) {
  // Buffer setup
  this->buffer_size = buffer_size;
  this->buffer_status = FREE;
  this->buffer = (char *)malloc(buffer_size * sizeof(char));
  
  // General Server setup
  this->running = false;
  this->client_size = (socklen_t)sizeof(this->client);
  this->gate = gate;
  this->ip = ip;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  
  cout << "Server: socket created " << endl;
}

bool Server::init(Physics *physics) {
  // Game setup
  this->physics = physics;
  
  // General Server setup
  this->myself.sin_family = AF_INET; //protocol
  this->myself.sin_port = htons(this->gate); //gate
  inet_aton(this->ip.c_str(), &(this->myself.sin_addr)); //address

  cout << "Server: trying to open gate " << this->gate <<endl;
  if (bind(this->socket_fd, (struct sockaddr*)&this->myself, sizeof(this->myself)) != 0) {
    cout << "Server: problems occured, exiting..." << endl;
    return false;
  }
  cout << "Server: opened gate "<< this->gate << " successfully!" << endl;
  return true;
}

void Server::sclose() {
  this->running = false;
  (this->pkg_thread).join();
  free(this->buffer);
  close(this->socket_fd);
}

void Server::slisten() {
  listen(this->socket_fd, 2);
  cout << "Server: listening on gate " << this->gate << endl;
  cout << "Server: waiting packages..." << endl;
  this->connection_fd = accept(this->socket_fd, (struct sockaddr*)&this->client, &this->client_size);
  this->running = true;
  thread newthread(wait_package, &(this->buffer), this->buffer_size, &(this->buffer_status), &(this->running), this->connection_fd);
  (this->pkg_thread).swap(newthread);
}

json Server::getPackage() {
  json pkg;
  if(this->buffer_status==BUSY) {
    string buffer_copy(this->buffer);
    pkg = json::parse(buffer_copy);
    this->buffer_status = FREE;
    cout << "Server: received:" << endl;
    cout << pkg.dump(4) << endl;
  }
  return pkg;
}

bool Server::sendPackage(string data) {
  if(send(this->connection_fd, data.c_str(), data.size()+1, 0) < 0) {
    return false;
  }
  return true;
}

bool Server::addClient(json data) {
    json game_state;
    
    cout << "Server: player " << data["name"].dump() << " joined the server." << endl;

    // Create game state package to send to the client
    game_state["id"] = this->physics->addPlayer(data["name"].get<string>());
    game_state["players"] = this->physics->getPlayers()->serialize();
    game_state["map"] = this->physics->getMap()->serialize();
    game_state["obstacles"] = this->physics->getObstacles()->serialize();

    // Sending confirmation to client
    cout << "Server: replying game: " << endl;
    cout << game_state.dump(4) << endl;
    sendPackage(game_state.dump());

    return true;
}

void Server::updateGame(json state) {
  // Checking empy state
  if(state.empty()) {
    cout << "Server: nothing to do, returning." << endl;
    return;
  }

  // Updating physics
  this->physics->update(state);

  // Broadcasting result
  json new_state;
  new_state["players"] = this->physics->getPlayers()->serialize();
  sendPackage(new_state.dump());
}

Physics *Server::getPhysics() {
  return this->physics;
}
