#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;

  //server makes the WKP
  if (mkfifo(WKP, 0644)==-1) {
    perror("WKP mkfifo error");
  }
  printf("1. Server created WKP\n");

  //server opens the WKP
  from_client = open(WKP, O_RDONLY);
  printf("2. Server opened WKP\n");

  //server removes WKP after reading
  remove(WKP);
  printf("4. Server removed WKP\n");

  return from_client; //the WKP is opened by the client
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int to_client) {
  //server WKP setup
  char client_name[256];
  int from_client = server_setup();

  //server reads SYN from client PP
  int r = read(from_client, client_name, sizeof(client_name));
  if (r==-1) {
    perror("error server reading from private pipe");
  }
  printf("5. Server reading SYN: %s\n", client_name);
  
  //server opens and writes SYN_ACK to WKP
  to_client = open(WKP, O_WRONLY);
  printf("6. Server connects to private pipe\n");

  int w = write(to_client, "SYN_ACK", sizeof("SYN_ACK"));
  if (w==-1) {
    perror("error server writing to WKP");
  }
  printf("7. Server sent SYN_ACK to WKP\n");

  //server receives the final ACK?

  
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int to_server) {
  //client makes PP
  char pipe_name[256];
  sprintf(pipe_name, "%d", getpid());
  if (mkfifo(pipe_name, 0644)==-1) {
    perror("private pite mkfifo");
  }
  printf("3. Client created PP %s\n", pipe_name);

  //client opens and writes to WKP
  to_server = open(WKP, O_WRONLY);
  printf("3. Client connects to WKP\n");

  int w = write(to_server, pipe_name, sizeof(pipe_name));
  if (w==-1) {
    perror("error client writing to WKP");
  }
  printf("3. Client wrote to server through WKP\n");
  
  //client removes PP
  remove(pipe_name);
  printf("8. Client removed private pipe\n");

  //client reads SYN_ACK from WKP
  int from_server;
  char* buffer; //for SYN_ACK
  from_server = open(WKP, O_RDONLY);
  int r = read(from_server, buffer, sizeof(buffer));
  if (r==-1) {
    perror("error client reading from private pipe");
  }
  printf("8. Client reading SYN_ACK\n");

  //client sends ACK (+1)
  w = write(to_server, buffer, sizeof(buffer)+1);
  if (w==-1) {
    perror("error client writing back to server");
  }
  printf("8. Client sent server ACK\n");



  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
