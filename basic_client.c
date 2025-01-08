#include "pipe_networking.h"
#include <signal.h>

static void sighandler(int signo) {
  if (signo == SIGINT || signo==SIGPIPE) {
    char pipe_name[256];
    sprintf(pipe_name, "%d", getpid());
    remove(pipe_name);
    kill(getpid(), SIGTERM);
  }
}

int main() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  //printf("from_server FD: %d\n", from_server);

  int num;
  char values[20];
  int bytes;
  while (1) {
  	bytes = read(from_server, values, sizeof(values));
    if (bytes!=0) {
    	//printf("Bytes read: %d ", bytes);
    	printf("Client received number: %s\n", values);
    }
    else {
   		char pipe_name[256];
    	sprintf(pipe_name, "%d", getpid());
    	remove(pipe_name);
    	exit(1);
    }
  }

  return 0;
}
