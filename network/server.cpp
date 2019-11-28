#include "server.hpp"

void wait_connections(bool *running, int *socket_fd, struct sockaddr_in *client, socklen_t *client_size, Server *s) {
  int conn_fd;
  while(*running) {
    conn_fd = accept(*socket_fd, (struct sockaddr*)client, client_size);
    s->addConnection(conn_fd);
  }
}

void wait_package(char **buffer, int buffer_size, bool *buffer_status, bool *running, int connection_fd, int i) {
  int bytes_recv;
  cout << "Server: (pkg thread) new thread initialized." << endl;
  while(*running) {
    if(*buffer_status==FREE) {
      bytes_recv = 0;
      *buffer[0]= '\0';
      bytes_recv = recv(connection_fd, *buffer, buffer_size, 0);
      if(bytes_recv < 0) {
        *buffer[0]= '\0';
        *buffer_status=FREE;
      } else {
        *buffer_status = BUSY;
      }
    }
    std::this_thread::sleep_for (chrono::milliseconds(40));
  }
}

int Server::addConnection(int connection_fd) {
  int i;

  // Check if there is capacity for new client
  for (i=0 ; i<MAX_CONNECTIONS ; i++) {
    if (this->used_connections[i] == false) {
      cout << "Server: new user joined the server, launching package thread for user(" << i << ")" << endl;

      // Configuring buffer and launching thread for new user
      this->used_connections[i] = true;
      this->connection_fd[i] = connection_fd;
      this->buffer_status[i] = FREE;
      buffer[i] = (char *)malloc(this->buffer_size * sizeof(char));
      thread newthread(wait_package, &(this->buffer[i]), this->buffer_size, &(this->buffer_status[i]), &(this->running), this->connection_fd[i], i);
      (this->pkg_thread[i]).swap(newthread);

      return i;
    }
  }
  cout << "Server: ERROR maximum users reached." << endl;  
  return -1;
}

void Server::removeConnection(int user) {
  cout << "Server: user(" << user << ") left the server" << endl;
  if (this->used_connections[user]==true) {
    this->used_connections[user] = false;
    free(buffer[user]);
    close(this->connection_fd[user]);
  }
}

Server::Server(unsigned int gate, string ip, unsigned int buffer_size) {
  int i;

  this->buffer_size = buffer_size;
  for(i=0 ; i<MAX_CONNECTIONS ; i++) {
    this->buffer_status[i] = FREE;
  }

  // General Server setup
  this->running = false;
  this->client_size = (socklen_t)sizeof(this->client);
  this->gate = gate;
  this->ip = ip;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Client list setup
  for(i=0 ; i<MAX_CONNECTIONS ; i++) {
    this->used_connections[i] = false;
  }
  
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
  int i;

  this->running = false;

  // Close every connection
  for(i=0 ; i<MAX_CONNECTIONS ; i++) {
    removeConnection(i);
    (this->pkg_thread[i]).join();
  }
  this->connection_thread.join();
}

void Server::slisten() {
  listen(this->socket_fd, 2);
  cout << "Server: listening on gate " << this->gate << endl;
  cout << "Server: waiting connections..." << endl;

  this->running = true;

  // Listen to new connections
  thread connectionthread(
                          wait_connections,
                          &(this->running),
                          &(this->socket_fd),
                          &(this->client),
                          &(this->client_size),
                          this
                          );
  (this->connection_thread).swap(connectionthread);

}

json Server::getPackage() {
  int i;
  json pkg;
  json buffers;
  
  for(i=0 ; i<MAX_CONNECTIONS ; i++) {
    if(this->buffer_status[i]==BUSY) {
      string buffer_copy(this->buffer[i]);
      this->buffer_status[i] = FREE;

      // Verify package integrity
      try {
        pkg = json::parse(buffer_copy);
        //cout << "Server: got package" << endl;
        //cout << pkg.dump(4) << endl;
      }
      catch(json::parse_error &e) {
        cout << "Server: ERROR parsing json. returning" << endl;
        pkg = {};
        continue;
      }

      // Append client id to package
      if(!pkg.empty()) {
        pkg["client"] = i;
        buffers.push_back(pkg);
      }
      
      cout << "Server: buffer(" << i << ") received:" << endl;
      cout << pkg.dump(4) << endl;
    }
  }
  return buffers;
}

bool Server::sendPackage(string data) {
  int i, res;
  for(i=0 ; i<MAX_CONNECTIONS ; i++) {
    if(this->used_connections[i]) {
      res = write(this->connection_fd[i], data.c_str(), data.length()+1);
      if(res<0) {
        return false;
      }
    }
  }
  return true;
}

bool Server::addClient(json data) {
    json game_state;
    
    cout << "Server: player " << data["name"].dump() << " joined the server." << endl;

    // Create game state package to send to the client
    game_state["init"] = true;
    game_state["id"] = this->physics->addPlayer(data["name"].get<string>(), data["team"].get<string>());
    game_state["players"] = this->physics->getPlayers()->serialize();
    game_state["map"] = this->physics->getMap()->serialize();
    game_state["obstacles"] = this->physics->getObstacles()->serialize();

    // Sending confirmation to client
    cout << "Server: replying game: " << endl;
    cout << game_state.dump(4) << endl;
    sendPackage(game_state.dump());

    return true;
}

void Server::removeClient(json data) {
  this->physics->removePlayer(data["id"].get<unsigned>());
  removeConnection(data["client"].get<int>());
}

void Server::updateGame(json state) {
  // Checking empy state (no players joined the server)
  if(state.empty()) {
    return;
  }
  if(!state["init"].empty() && state["init"].get<bool>()) {
    return;
  }

  // Updating physics
  this->physics->update(state);

  // Broadcasting result
  json new_state;
  new_state["is_game_status"] = true;
  new_state["players"] = this->physics->getPlayers()->serialize();
  new_state["map"] = this->physics->getMap()->serialize();
  //cout << "Broadcasting physics" << endl;
  //cout << new_state.dump(4) << endl;
  sendPackage(new_state.dump());
}

Physics *Server::getPhysics() {
  return this->physics;
}
