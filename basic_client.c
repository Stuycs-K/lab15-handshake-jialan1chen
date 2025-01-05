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
  int to_server; //from client
  int from_server; //to client

  
  from_server = client_handshake( to_server );

  // mkfifo("WKP", 0644);
  int to_server2 = open("WKP", O_WRONLY);
  
  int pid = getpid();
  printf("writing to server pid %d...\n", pid);
  if (write(to_server2, &pid, sizeof(pid))==-1) {
    perror("error client writing to WKP");
    exit(1);
  }

  int num;
  char pipe_name[256];
  sprintf(pipe_name, "%d", pid);

  mkfifo(pipe_name, 0644);
  int from_server2 = open(pipe_name, O_RDONLY);
  

  while (1) {
    if (read(from_server2, &num, sizeof(num))==-1) {
      perror("client reading error");
    }
    printf("Client received number: %d\n", num);
    sleep(1);
  }

  return 0;
}
