#include "pipe_networking.h"
#include <time.h>

int main() {
  int to_client;
  int from_client;
  int to_server; //from client
  int from_server; //to client

  while(1) {
  	from_client = server_handshake( to_client );
  	from_server = client_handshake( to_server );

  	int n = rand() % 101;
  	write(to_client, &n, sizeof(n));
  	printf("server sent number: %d\n", n);
  	srand(time(NULL));
  	sleep(1);
  }

  return 0;
}
