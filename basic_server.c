#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( to_client );

  // char message[] = "chicken";
  // write(to_client, message, sizeof(message));
  // printf("server sent message: %s\n", message);
  // char buffer[256];
  // read(from_client, buffer, sizeof(buffer));
  // printf("server received message: %s\n", buffer);

  return 0;
}
