#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#define BACKLOG 10
int main(){
  char ipstr[INET6_ADDRSTRLEN];
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  struct addrinfo hints,*res;
  memset(&hints,0 ,sizeof(hints) );

  hints.ai_family=AF_UNSPEC; //fill the Ip for me either IPv4 or IPv6
  hints.ai_socktype=SOCK_STREAM; //TCP socket
  hints.ai_flags=AI_PASSIVE;//fill in the IP for me of my host

  int status=getaddrinfo(NULL,"3490" , &hints, &res);
  if (status!=0){
    fprintf(stderr,"There has been an error" );
    return 1;
  }
  int socketfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  //s=-1 means there is error binding the socket
  if (socketfd==-1){
    fprintf(stderr, "There is error binding the socket");
    return 1;
  }
  int bindStatus =bind(socketfd,res->ai_addr ,res->ai_addrlen );
  if (bindStatus==-1){
    perror("socket");
  }
  listen(socketfd,BACKLOG);
  addr_size = sizeof their_addr;
  int new_fd = accept(socketfd, (struct sockaddr *)&their_addr,&addr_size );

  if (their_addr.ss_family == AF_INET){
    struct sockaddr_in *addr=(struct sockaddr_in *)&their_addr;
    //sockaddr_in will be in buffer need to convert it to human readable form
    inet_ntop(AF_INET, &(addr->sin_addr),ipstr , sizeof(ipstr));
  } else { // IPv6
        struct sockaddr_in6 *addr = (struct sockaddr_in6 *)&their_addr;
        inet_ntop(AF_INET6, &(addr->sin6_addr), ipstr, sizeof ipstr);
    }

  printf("Got connection from %s\n", ipstr);
  close(socketfd);
  return 0;
}
