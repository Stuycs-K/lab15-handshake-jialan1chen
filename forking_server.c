#include "pipe_networking.h"
#include <time.h>
#include <signal.h>

int server_handshake_half(int* to_client, int from_client) {
    char client_name[256];
    int r = read(from_client, client_name, sizeof(client_name));
    if (r==-1) {
        perror("error subserver reading from WKP");
        exit(1);
    }
    printf("5. Subserver reading SYN: %s\n", client_name);

    *to_client = open(client_name, O_WRONLY);
    printf("1 to_client FD: %d\n", *to_client);
    printf("6. Subserver connects to private pipe\n");
    char buffer[256];
    strcpy(buffer, "SYN_ACK");

    int w = write(*to_client, buffer, sizeof(buffer));
    if (w==-1) {
        perror("error subserver writing to private pipe");
        exit(1);
    }
    printf("7. Subserver sent SYN_ACK to private pipe\n");

    r = read(from_client, buffer, sizeof(buffer));
    printf("9. Subserver reading ACK: %s.\nComplete handshake\n", buffer);

    return from_client;
}

void comm(int to_client, int from_client) {

    while(1) {
    int num = rand() % 101;
    int bytes;
    // server writes to client
    if (bytes = write(to_client, &num, 4)==-1) {
        perror("write server error");
        close(to_client);
        close(from_client);
        break;
    }
    printf("1 Bytes: %d", bytes);
    printf("server sent number: %d\n", num);
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
        printf("Created subserver\n");
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
