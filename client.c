#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define PORT "3490"
#define MAXDATASIZE 100
void *get_in_addr(struct sockaddr *sa){
  if (sa->sa_family == AF_INET){
    return &((struct sockaddr_in *)sa)->sin_addr;
  }
  return &((struct sockaddr_in6 *)sa)->sin6_addr;
}
int main(int argc,char *argv[]){
  int sockfd,numbytes;
  char buf[MAXDATASIZE];
  struct addrinfo hints,*servinfo,*p;
  int rv;
  char s[INET6_ADDRSTRLEN];

  if (argc != 2){
    fprintf(stderr,"usage: client hostname\n" );
    exit(1);
  }
  memset(&hints,0,sizeof(hints));
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype=SOCK_STREAM;
  if ((rv=getaddrinfo(argv[1],PORT ,&hints ,&servinfo ))!=0){
    fprintf(stderr,"getaddrinfo %s\n",gai_strerror(rv));
    return 1;
  }

  for (p=servinfo;p!=NULL;p=p->ai_next){
    if((sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
      perror("client socket");
      return 1;
    }
    if(connect(sockfd,p->ai_addr,p->ai_addrlen)==-1){
      close(sockfd);
      perror("Client:connect");
      continue;
    }
    break;
  }
  if (p==NULL){
    fprintf(stderr,"Client connect error" );
    return 1;
  }
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s ,sizeof(s) );
  printf("Client:connecting to %s",s);
  if((numbytes=recv(sockfd,buf ,MAXDATASIZE-1 ,0 ))==-1){
    perror("recv");
    return 1;
  }
  buf[numbytes]='\0';
  printf("Received %s",buf);
  close(sockfd);
  return 0;
}
