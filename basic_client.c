#include "pipe_networking.h"

int main() {

  int to_server; //from client
  int from_server; //to client

  from_server = client_handshake( to_server );

  // char buffer[256];
  // read(from_server, buffer, sizeof(buffer));
  // printf("client received message: %s\n", buffer);
  // char response[256];
  // write(to_server, response, sizeof(response));
  // printf("client sent message: %s\n", response);

  return 0;
}
