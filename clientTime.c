/*
  Taken from:
  http://www.thegeekstuff.com/2011/12/c-socket-programming/
  
  Also check the relevant Linux Programmer's Manual:
  http://man7.org/linux/man-pages/man7/ip.7.html

  and other related manpages...
*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main( int argc, char *argv[])
{
  // socket file descriptor to use in communication
  int socketFD = 0;

  // a place-holder to mark the end of the buffer (see below for how it is used)
  int n = 0;

  // the buffer to receive content from
  char recvBuff[1024];

  /*
    The following structure holds the internet address and the port number.
  */
  struct sockaddr_in serv_addr; 

  if( argc != 2)
    {
      fprintf( stderr, "\n Usage: %s <ip of server> \n", argv[0]);
      return 1;
    } 

  // initialize the buffer content to zeros
  memset( recvBuff, '0', sizeof( recvBuff));

  // the following checks are very useful, with informative messages
  if( ( socketFD = socket( AF_INET, SOCK_STREAM, 0)) < 0)
    {
      fprintf( stderr, "\n Error : Could not create socket \n");
      return 1;
    } 

  // again, just clear the serv_addr
  memset( &serv_addr, '0', sizeof( serv_addr)); 

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons( 5000); 

  /*
    Note the use of inet_pton for converting the string argv[ 1],
    which was given as input, to a binary format. The result is stored
    in serv_addr.sin_addr
   */
  if( inet_pton( AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
      fprintf( stderr, "\n inet_pton error occured\n");
      return 1;
    } 

  if(  connect( socketFD, ( struct sockaddr *)&serv_addr, sizeof( serv_addr)) < 0)
    {
      fprintf( stderr, "\n Error : Connect Failed \n");
      return 1;
    } 

  while ( ( n = read( socketFD, recvBuff, sizeof( recvBuff) - 1)) > 0)
    {
      recvBuff[ n] = 0;
      if( fputs( recvBuff, stdout) == EOF)
        {
	  fprintf( stderr, "\n Error : Fputs error\n");
        }
    } 

  if( n < 0)
    {
      fprintf( stderr, "\n Read error \n");
    } 

  return 0;
}
