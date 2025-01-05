#include "pipe_networking.h"
#include <time.h>
// 
// void cycle(int to_client) {
    // srand(time(NULL));
    // int num;
// 
    // while(1) {
		// printf("hello?\n");
        // num = rand() % 101;
// 
        // // server writes to client
        // if (write(to_client, &num, sizeof(num))==-1) {
            // perror("write server error");
            // exit(1);
        // }
        // printf("server sent number: %d\n", num);
        // sleep(1);
// 
    // }
// }

int main() {
    int to_client;
    int from_client;
    srand(time(NULL));

    while (1) {
        from_client = server_handshake( to_client );
        
        int client_name;
        mkfifo("WKP", 0644);
        int from_client2 = open("WKP", O_RDONLY);
        if (from_client2==-1) {
        	perror("error opening WKP");
        	exit(1);
        }
        if (read(from_client2, &client_name, sizeof(client_name))==-1) {
            perror("error reading from client");
            exit(1);
        }
        printf("Server connected to client %d\n", client_name);
        
        char pipe_name[256];
        sprintf(pipe_name, "%d", client_name);

        mkfifo(pipe_name, 0644);
        int to_client2 = open(pipe_name, O_WRONLY);
        printf("opening pipe??\n");

    	int num;
        // cycle(to_client2);

        while(1) {
        printf("hello?\n");
        num = rand() % 101;

        // server writes to client
        if (write(to_client2, &num, sizeof(num))==-1) {
            perror("write server error");
            exit(1);
        }
        printf("server sent number: %d\n", num);
        sleep(1);

    	}
        
        remove("WKP");
        printf("A client has exited\n");
        close(to_client2);
        close(from_client2);
    }

    return 0;
}

