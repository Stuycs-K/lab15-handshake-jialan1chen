#include "pipe_networking.h"
#include <signal.h>

static void sighandler(int signo) {
  if (signo == SIGINT) {
    char pipe_name[256];
    sprintf(pipe_name, "%d", getpid());
    remove(pipe_name);
    kill(getpid(), SIGTERM);
  }
}

int main() {
  signal(SIGINT, sighandler);
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  printf("from_server FD: %d\n", from_server);

  int num;
  int bytes;
  while (1) {
    if (bytes = read(from_server, &num, 4)==-1) {
      perror("client reading error");
    }
    printf("2 Bytes: %d", bytes);
    printf("Client received number: %d\n", num);
  }

  return 0;
}
