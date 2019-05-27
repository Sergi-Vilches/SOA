#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


// Create a socket and initialize it to be able to accept 
// connections.
// It returns the virtual device associated to the socket that should be used 
// in the accept system call, to get the virtual device associated to 
// the connection
//


int createServerSocket (int port)
{
	int fd = socket(PF_INET,SOCK_STREAM, 0);
	if(fd < 0) return -1;
	struct sockaddr_in server_addr;
	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	
	int retbind = bind(fd, (struct sockaddr_in*) &server_addr, sizeof(server_addr));
	if(retbind < 0) return -1;
	
	int retlisten = listen(fd, 15);
	if(retlisten < 0) return -1;
	
	return fd; 
}


// Returns the file descriptor associated to the connection.
// accept system call will fill the socketAddr parameter
// with the address of the socket for the client which is requesting the
// connection, and the addrSize parameter with the size of that address.

int acceptNewConnections (int socket_fd)
{
	struct sockaddr_in server_addr;
	int lenght = sizeof(server_addr);
	int fd = accept(socket_fd, (struct sockaddr_in *) &server_addr, &lenght);
	if(fd < 0) return -1;
	
	return fd;
}

// Returns the socket virtual device that the client should use to access 
// the socket, if the connection is successfully established 
// and -1 in case of error
//
// Connect system call requires the address of the 
// server socket to request the connection and the size of that address.
//

int
clientConnection (char *host_name, int port)
{

  struct sockaddr_in serv_addr;
  struct hostent * hent;
  int socket_fd;
  int ret;

  //creates the virtual device for accessing the socket
  socket_fd = socket (AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
    return socket_fd;

  memset((char *) &serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  hent = gethostbyname(host_name);
  if (hent == NULL) {
        close (socket_fd);
	return -1;

  }
  memcpy((char *)&serv_addr.sin_addr.s_addr, (char *) hent->h_addr, hent->h_length);
  serv_addr.sin_port = htons(port);
  serv_addr.sin_family = PF_INET; 

  ret = connect (socket_fd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));
  if (ret < 0)
  {
	  close (socket_fd);
	  return (ret);
  } 

  return socket_fd;

}


int deleteSocket (int socket_fd)
{
  close (socket_fd);
}

