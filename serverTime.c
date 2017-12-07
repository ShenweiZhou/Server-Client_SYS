/*
  Taken from:
  http://www.thegeekstuff.com/2011/12/c-socket-programming/
  
  Also check the relevant Linux Programmer's Manual:
  http://man7.org/linux/man-pages/man7/ip.7.html

  and other related manpages...
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 


int main( int argc, char *argv[])
{
  
  // socket file descriptor to listen on 
  int socketFD = 0;

  // file descriptor for the accepted connection
  int connfd = 0;

  // the maximum number of connections to accept
  int backlog = 5;


  /*
    The following structure holds the internet address and the port number.
  */
  struct sockaddr_in serv_addr;
  struct sockaddr_in* clientAddress;
  unsigned int length=sizeof(struct sockaddr_in);
  unsigned int* clientLength;
	clientLength=&length;
	clientAddress=(struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
	if(clientAddress==NULL){
	  
	  	printf("clientAddress error!\nMemory allocation failed!\n");
		return 0;
  	}

  char* clAddress=NULL;
  clAddress=(char*)malloc(17*sizeof(char));
 if(clAddress==NULL){
	  
	  printf("Memory allocation failed!\n");
	return 0;
  }
  

  // the buffer to be used for sending content to client(s)
  char sendBuff[1025];
  time_t ticks; 

  // create a TCP socket (Note: not bound yet)
  socketFD = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // some initialization
  memset( &serv_addr, '0', sizeof( serv_addr));
  memset( sendBuff, '0', sizeof( sendBuff)); 

  // specify the socket type
  serv_addr.sin_family = AF_INET; // always set to AF_INET
  serv_addr.sin_addr.s_addr = htonl( INADDR_ANY); // means: bind to all local interfaces
  serv_addr.sin_port = htons( 21878); // Port number
  // Question: Why use htons?

  // assign the address we just specified in serv_addr to the socket
  // referred to by the file descriptor socketFD
  if(bind( socketFD, (struct sockaddr*)&serv_addr, sizeof( serv_addr))<0)
  {
	perror("bind() error ");
	return 0;
  }


  /*
    the following marks the socket referred to by socketFD as a
    passive socket, that is, as a socket that will be used to
    accept incoming connection requests using accept( see below).

    The second argument (called backlog argument) in listen()
    defines the maximum length to which the queue of pending
    connections for socketFD may grow.  If a connection request
    arrives when the queue is full, the client may receive an error
    with an indication of ECONNREFUSED or, if the underlying
    protocol supports retransmission, the request may be ignored so
    that a later reattempt at connection succeeds.
  */
  listen( socketFD, backlog); 

  while( 1) // repear 'forever'
    {

      // accept a new connection on a socket
      connfd = accept( socketFD, (struct sockaddr*)clientAddress, (unsigned int*)clientLength);
	if(connfd<0){
		  printf("Failed to connect!\n");
		  return 0;
	  }
	  if(inet_ntop(AF_INET,&(((struct sockaddr_in*)clientAddress)->sin_addr),clAddress,16*sizeof(char))<0){
		  printf("Address convertion failed!\n");
		  return 0;
	  }


     // printf( "Accepted a connection from %ld : %d .\n",((struct sockaddr_in*)clientAddress)->sin_addr.s_addr ,clientAddress->sin_port);
       printf( "Accepted a connection from %s : %d .\n", clAddress,clientAddress->sin_port);
      // returns the time passed since the epoch of Jan 1st, 1970, in seconds
      ticks = time( NULL);

      // the ctime function turns the seconds passes since epoch to a string representation
      snprintf( sendBuff, sizeof( sendBuff), "%.24s\r\n", ctime( &ticks));

      // send the buffer content to the client
      write( connfd, sendBuff, strlen( sendBuff)); 

      // do not forget to close the connection once the task is done
      close( connfd);
      printf( "Closed the connection %d \n", connfd);

      // the following is a nice way of preventing the server to use all of the
      // CPU time for itself
      sleep( 1);
    }

}