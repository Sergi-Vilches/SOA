#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


// C program for array implementation of queue
#include <limits.h>
#define MAX 100000000

typedef int semaphore;  //Semaphores ae a special kind of int
sem_t mutex; //Controls access to critical region 1

int intArray[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;
int num_threads = 8;
int peek() {
   return intArray[front];
}

isEmpty() {
   return itemCount == 0;
}

isFull() {
   return itemCount == MAX;
}

int size() {
   return itemCount;
}  

void insert(int data) {

   if(!isFull()) {
	
      if(rear == MAX-1) {
         rear = -1;            
      }       

      intArray[++rear] = data;
      itemCount++;
   }
}

int removeData() {
   int data = intArray[front++];
	
   if(front == MAX) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}
char buff2[800];
doService(int fd) {
int i = 0;
char buff[800];

int ret;
int socket_fd = (int) fd;

	ret = read(socket_fd, buff, sizeof(buff));
	while(ret > 0) {
		buff[ret]='\0';
		sprintf(buff2, "Server [%d] received: %s\n", getpid(), buff);
		write(1, buff2, strlen(buff2));
		sprintf(buff2,"The ID of this thread is: %u\n", (unsigned int)pthread_self(), buff);
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

}
//sem_t semaforo;
doServiceThread() {
	while(1) {
    while (isEmpty());
		sem_wait(&mutex);
		int fd = removeData();
		sprintf(buff2,"The ID of this thread is: %u\n", (unsigned int)pthread_self(), buff2);
		write(1, buff2, strlen(buff2));
		
		doService(fd);
		sem_post(&mutex);
		close(fd);
		/*Leer de la cola
		  Coger peticion
		  tratar peticion (doService)*/
	}
}
/*
doServiceFork(int fd) {
	int pid = fork();
	if(pid < 0) {
		perror("Error with fork");
		exit(1);
	}
	if(pid == 0) {
		doService(fd);
		exit(0);
	}
}
*/

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
	 for (int i = 0; i < num_threads; i++){
  	pthread_t thread;
		int ret = pthread_create(&thread, NULL, doServiceThread,NULL);
		if (ret != 0) 
				exit(1);
  	}
  sem_init(&mutex,0,1);
  if (socketFD < 0)
    {
      perror ("Error creating socket\n");
      exit (1);
    }

  while (1) {
	  connectionFD = acceptNewConnections (socketFD);
	  if (connectionFD < 0)
	  {
		  perror ("Error establishing connection \n");
		  deleteSocket(socketFD);
		  exit (1);
	  }

	  insert(connectionFD);
  }

}
