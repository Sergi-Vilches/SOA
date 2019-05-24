#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h> 
#include <signal.h>

#define MAX_T 4
int count = 0;
int max = 4;


char buff3[80];
pthread_t vector_t[MAX_T];
sem_t semafor;
sem_t semaforP;
int canals[1000];
int num = 0;


*doService() {
	sem_wait(&semaforP);
	sem_wait(&semafor); //S'ha de posar per no tractar més d'un service a la vegada
	if(num > 0) {
		int i = 0;
		char buff[80];
		char buff2[80];
		int ret;
		int fd = canals[0];
		int socket_fd = (int) fd;
		int j;
		for (j = 0; j < num-1; ++j) {
		canals[j] = canals[j+1];
		}
		--num;

		ret = read(socket_fd, buff, sizeof(buff));
		while(ret > 0) {
			buff[ret]='\0';
			sprintf(buff2, "Server [%d] received: %s\n", getpid(), buff);
			write(1, buff2, strlen(buff2));
			ret = write(fd, "caracola ", 8);
			if (ret < 0) {
				perror ("Error writing to socket");
				exit(1);
			}
			ret = read(socket_fd, buff, sizeof(buff));
		}
		if (ret < 0) {
				perror ("Error reading from socket");

		}
		sprintf(buff2, "Server [%d] ends service\n", getpid());
		write(1, buff2, strlen(buff2));
		close(fd);
	}
	sem_post(&semafor);

}

/*doServiceFork(int fd) {
	
	int pid = fork();
	if(pid < 0) {
		perror("Error with fork");
		exit(1);
	}
	if(pid == 0) {
		count++;
		doService(fd);
		count--;
		sprintf(buff3, "Acabo hijo [%d]\n", count);
		write(1, buff3, strlen(buff3));
		exit(0);
	}
}*/


main (int argc, char *argv[])
{
  int socketFD;
  int connectionFD;
  char buffer[80];
  int ret;
  int port;


  if (argc != 2)
    {
      strcpy (buffer, "Usage: ServerSocket PortNumber\n");
      write (2, buffer, strlen (buffer));
      exit (1);
    }

  port = atoi(argv[1]);
  socketFD = createServerSocket (port);
  if (socketFD < 0)
    {
      perror ("Error creating socket\n");
      exit (1);
    }
  int i;
  for(i = 0;i<max; i++) {
	  pthread_t thread;
	  vector_t[i] = thread;
  }
  ret = sem_init(&semafor, 0, 1);
  if(ret < 0) printf(buff3, "error en la creacio del semafor\n");
  ret = sem_init(&semaforP, 0, 0);
  if(ret < 0) printf(buff3, "error en la creacio del semafor\n");
  
  for (i = 0; i  < max; ++i){
		int iret = pthread_create (&vector_t[i] ,NULL ,doService , NULL);
		 if(iret) {
			 perror ("Error en la creacio del thread\n");
			exit (1);
		 }
  }
  

  while (1) {
	  connectionFD = acceptNewConnections (socketFD);
	  ++num;
	  canals[num-1] = connectionFD;
	  if (connectionFD < 0)
	  {
		  perror ("Error establishing connection \n");
		  deleteSocket(socketFD);
		  exit (1);
	  }
	  sem_post(&semaforP);
  }
  sem_destroy(&semafor);
  sem_destroy(&semaforP);

}
