#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

// int getaddrinfo(const char *node, // domain or IP address
//                 const char *service , //http or port number
//                 const struct addrinfo *hints ,
//                 struct addrinfo **res );
int main(){
  int status;
  struct addrinfo hints;
  struct addrinfo *serviceinfo;
  //addr info has the size of 48bit so fill the hints instance with 48 0s
  memset( &hints,0 ,sizeof(hints) );
  //protocol family for socket. AF_UNSPEC means donot care about IPv4 or IPv6.
  // IPV4- AF_INET IPV6- AF_INET6
  hints.ai_family= AF_UNSPEC;
  //Stream socket for TCP connection
  hints.ai_socktype=SOCK_STREAM;
  //Fill in IP for me. Will be loopback address if used will NULL
  hints.ai_flags=AI_PASSIVE;

  status = getaddrinfo(NULL, "3490",&hints , &serviceinfo);
  //connecting to google.com
  // getaddrinfo("www.google.com","3490" ,&hints ,&serviceinfo );
  printf("%d",status);
  //service info is the linked list. Free the link list
  if (status!=0){
    printf("There has been an error");
    return 2;
  }
  freeaddrinfo(serviceinfo);
  return 0;
}
