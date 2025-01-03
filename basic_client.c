#include "pipe_networking.h"
#include <signal.h>

static void sighandler(int signo) {
  if (signo == SIGINT) {
    kill(getpid(), SIGTERM);
  }
}

int main() {
  signal(SIGINT, sighandler);
  int to_server; //from client
  int from_server; //to client

  from_server = client_handshake( to_server );

  int num;

  while (1) {
    if (read(from_server, &num, sizeof(num))==-1) {
      perror("client reading error");
    }
    printf("Client received number: %d\n", num);
  }

  return 0;
}
