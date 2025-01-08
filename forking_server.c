#include "pipe_networking.h"
#include <time.h>
#include <signal.h>

int server_handshake_half(int* to_client, int from_client) {
  char client_name[256];

  //server reads SYN from WKP
  int r = read(from_client, client_name, sizeof(client_name));
  if (r==-1) {
    perror("error server reading from WKP");
    exit(1);
  }
  printf("5. Server reading SYN: %s\n", client_name);

  //server opens and writes SYN_ACK to private pipe
  *to_client = open(client_name, O_WRONLY);
  printf("6. Server connects to private pipe\n");

  //char buffer[256];
  //strcpy(buffer, "SYN_ACK");
  int rand_num = 3417;

  int w = write(*to_client, &rand_num, sizeof(int));
  if (w==-1) {
    perror("error server writing to private pipe");
    exit(1);
  }
  printf("7. Server sent SYN_ACK to private pipe: %d\n", rand_num);

  int received_num;
  //server receives the final ACK through PP
  r = read(from_client, &received_num, sizeof(int));
  printf("9. Server reading ACK: %d.\nComplete handshake\n", received_num);

  return from_client;

}


void comm(int to_client, int from_client) {

    while(1) {
    int num = rand() % 101;
    char values[20];
    sprintf(values, "%d", num);
    int bytes;
    // server writes to client
    if (bytes = write(to_client, values, sizeof(values))==-1) {
        //perror("write server error");
        close(to_client);
        close(from_client);
        break;
    }
    //printf("1 Bytes: %d", bytes);
    printf("server sent number: %d\n", num);
    fflush(NULL);
    sleep(1);

    }
}

void server() {
    int to_client;
    int from_client;
    pid_t f;

    while (1) {

    from_client = server_setup();
    f = fork();

    //WKP, server read
    //child-- subserver
    if (f==0) {
        printf("---------Created subserver---------\n");
        server_handshake_half(&to_client, from_client);
        comm(to_client, from_client);
        printf("A client has exited\n");
        }
    }
}

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove(WKP);
    kill(getpid(), SIGTERM);
  }
}

int main() {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sighandler);

    srand(time(NULL));

    server();

    return 0;
}
