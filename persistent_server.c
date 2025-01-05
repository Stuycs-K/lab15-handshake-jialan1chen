#include "pipe_networking.h"
#include <time.h>
#include <signal.h>

void cycle() {
    int to_client;
    int from_client;
    from_client = server_handshake( to_client );

    int client_name;
    //mkfifo("WKP", 0644);
    
    int from_client2 = open("WKP", O_RDONLY);
    if (from_client2==-1) {
            perror("error opening WKP");
            return;
    }
    if (read(from_client2, &client_name, sizeof(client_name))==-1) {
        perror("error reading from client");
        return;
    }
    printf("Server connected to client %d\n", client_name);

    char pipe_name[256];
    sprintf(pipe_name, "%d", client_name);

    //mkfifo(pipe_name, 0644);
    int to_client2 = open(pipe_name, O_WRONLY);
    if (to_client2==-1) {
            perror("opening pipe error");
            return;
    }

    int num;

    while(1) {
    num = rand() % 101;

    // server writes to client
    if (write(to_client2, &num, sizeof(num))==-1) {
        perror("write server error");
        break;
    }
    printf("server sent number: %d\n", num);
    sleep(1);

    }
    close(to_client2);
    close(from_client2);

}

static void sighandler(int signo) {
  if (signo == SIGINT) {
  	remove("WKP");
    kill(getpid(), SIGTERM);
  }
}

int main() {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sighandler);

    srand(time(NULL));

    while (1) {
    	mkfifo("WKP", 0644);
        cycle();
        printf("A client has exited\n");
    }

    return 0;
}
