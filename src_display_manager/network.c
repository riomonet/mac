#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){

  struct addrinfo listen_hints;
  struct addrinfo *listen_shit;

  //  example of server setup for listening on port 2402
  memset(&listen_hints,0, sizeof listen_hints);
  listen_hints.ai_family  = AF_UNSPEC;
  listen_hints.ai_socktype = SOCK_STREAM;
  listen_hints.ai_flags = AI_PASSIVE;  // local host
  //notice we dont use ai-passive on teh client belwo becasue we specify
  // an address
  int x = getaddrinfo(NULL, "2402", &listen_hints, &listen_shit);

  //  example of server setup for connecting to a server
  // as aclient on port 3490 www.example.com
  struct addrinfo client_hints;
  struct addrinfo *client_shit;

  //  example of server setup for listening on port 2402
  memset(&client_hints,0, sizeof client_hints);
  client_hints.ai_family  = AF_UNSPEC;
  client_hints.ai_socktype = SOCK_STREAM;

  int y = getaddrinfo("www.example.net", "3490", &client_hints, &client_shit);  

  int s_descriptor = socket(client_shit->ai_family, client_shit->ai_socktype, client_shit->ai_protocol);

  int sfd = socket(listen_shit->ai_family, listen_shit->ai_socktype, listen_shit->ai_protocol);

  bind(sfd,listen_shit->ai_addr, listen_shit->ai_addrlen);
  listen(sfd,5);
  
  connect(s_descriptor,client_shit->ai_addr, client_shit->ai_addrlen);

  socklen_t addr_size;
  struct sockaddr_storage thier_addr;
  int new_fd = accept(sfd,(struct sockaddr*) &thier_addr, &addr_size);
  freeaddrinfo(listen_shit);
  freeaddrinfo(client_shit);
  return 0;
}






/*
  struct sockaddr_storage
  inet_pton() presentation to network
  inet_ntop() network to presentation
  htons()     host to network short
  struct addrinfo;
  struct sockaddr; //object returned in addrinfo
  struct sockaddr_in
  struct in_addr

  order of system calls.
  1. getaddrinfo();
  2. socket();
  3. bind(); servers call bindlistening on a port not as a client
  clients bind locally to kernel assigned open port.
  4. connect(); clients call connect
  5. listen(); servers call listen
  6. accept(); servers call accept need a storage struct and len for kernel
  to fill in describing the client
  7. send(), recv() blocking calls
  stream socket functions, tcp, udp sendto recvfrom
  

*/
/*
  
  
struct sockaddr; //ip address related fields

--short    sa_family; address familiy  (AF_INET ) for us.
--char     sa_data[14]; dest address and port number.

struct sockaddr_in  is kinda of a helper for struct sockaddr
we can fill it out and cast it as a pointer to a sockaddr to be
passed as an arg to connect().

struct sockaddr_in;
       short  int sin_family;
       uitni  sin port;   2 bytes BIG ENDIAN htons();
       struct sin_addr;    the internet address 4 bytes
       char sin_zero[8];  set to all 0's for padding  8 bytes of padding

       see how these all add up to sa_date[14] in sockaddr.
       2 port 4 addr 8 padding.

       struct sin_addr:
           32 s_addr.

           sin_addr.sin_addr

//getaddrinfo() fills out struct addrinfo for us


converts ipa 

*/


