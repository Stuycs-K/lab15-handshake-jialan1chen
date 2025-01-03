#include "pipe_networking.h"
#include <time.h>

void cycle(int to_client, int from_client) {
    srand(time(NULL));
    int num;

    while(1) {

        num = rand() % 101;

        // server writes to client
        if (write(to_client, &num, sizeof(num))==-1) {
            perror("write server error");
            exit(1);
        }
        printf("server sent number: %d\n", num);
        sleep(1);

    }
}

int main() {
    int to_client;
    int from_client;

    while (1) {
        from_client = server_handshake( to_client );
        char client_name[256];
        sprintf(client_name, "%d", getpid()+1);
 
        to_client = open(client_name, O_WRONLY);
        printf("client_name: %s\n", client_name);
        if (to_client==-1) {
        	perror("error opening client pipe");
        }
   
        printf("Server connected to client %s\n", client_name);
        cycle(to_client, from_client);
        printf("A client has exited\n");
        close(to_client);
        close(from_client);
    }

    return 0;
}

