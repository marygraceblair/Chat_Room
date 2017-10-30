/*-------------------------------------------------------------*/
/* server.c - Multi-threaded, concurrent time/date server.     */
/*-------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "inet.h"

#define DEBUG 1
#define	MAX 100

void *do_something(void	*); /* new thread's code */

int sockfd;
pthread_t *threads;
unsigned int *threads_ids;
pthread_mutex_t mutex;
char usernames[10][10];
int indexUsernames = -1; 

main(int argc, char **argv)
{
	struct sockaddr_in serv_addr;
	int	  num_threads = 5;
	int	  i;
        int       ret_val;
        pthread_attr_t attr;

	pthread_mutex_init(&mutex, NULL);
	/* Create communication	endpoint */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0))	< 0) {
		perror("server:	can't open stream socket");
		return(1);
	}

	/* Bind	socket to local	address	*/
	memset((char *)	&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family	  = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port	  = htons(SERV_TCP_PORT);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)	{
		perror("server:	can't bind local address");
		return(1);
	}

	listen(sockfd, 5);

	/* create worker threads */
        threads = (pthread_t*) malloc((num_threads + 1) * sizeof (pthread_t));
        threads_ids = (unsigned int*) malloc((num_threads + 1) * sizeof (unsigned int));

        /* Initialize and set thread detached attribute */
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        threads[0] = pthread_self(); // parent thread

        for (i = 1; i <= num_threads; i++) {
            ret_val = pthread_create(&threads[i], &attr, do_something, NULL);
            if (ret_val < 0) {
                printf("ERROR; return code from pthread_create() is %d\n", ret_val);
                exit(-1);
            }
        }

        /* Free attribute and wait for the worker threads to exit */
        pthread_attr_destroy(&attr);

	for (i=1; i <= num_threads; i++)
	{
	  pthread_join(threads[i],NULL);
#if DEBUG
          printf("\nThread %u joined with parent.\n", (int) threads[i]);
#endif
	}
        free(threads);

#if DEBUG
        printf("\nParent is exiting, sorry clients...\n");
#endif
	pthread_mutex_destroy(&mutex);
        exit(0);
}

void *do_something(void	*arg)
{
	int	    newsockfd, clilen, childpid;
	struct	    sockaddr_in  cli_addr, serv_addr;
	struct tm   *timeptr;  /* pointer to time structure*/
	time_t	    clock;	 /* clock value	(in secs)	*/
	char	    s[MAX];
	char	    request[MAX];
	int	    num_threads = 5;
	int	    i;
	unsigned    tid;		/* thread id */
        int         nread;

	tid = pthread_self();		/* who am i */

	for (i=1;i<=2;i++)  // each thread only processes two requests...
	{
		/* Accept a new	connection request. */
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd <	0) {
			perror("server:	accept error");
			exit(1);
		}

		/* Read	the request from the client. */
		nread = read(newsockfd,	&request, MAX);
#if DEBUG
                printf("Thread %u read request %s \n", tid, request);
#endif
		/* Generate an appropriate reply. */
                if (nread > 0)
                {
			clock =	time(0);
			timeptr	= localtime(&clock);
/*
			switch(request)	{

			 case '1': strftime(s,MAX,"%A, %B %d, %Y",timeptr);
						   break;

			 case '2': strftime(s,MAX,"%T",timeptr);
					   break;

			 case '3': strftime(s,MAX,"%A, %B %d, %Y - %T",timeptr);
						   break;

			 default: strcpy(s,"Invalid request\n");
						  break;
			} */

			if (request[0] == ' ')
			{
				char c = request[0];
				printf("this is the char:%c", c);
				fflush(stdout);
				printf("char was printed");
				fflush(stdout);

				strcpy(s, "username added");
				char username[10];
				strcpy(username, &(request[1]));

				int i;
				int lock_ret = 1;
				
				do {
					lock_ret = pthread_mutex_trylock(&mutex);
					
					if(lock_ret){
					printf("lock failed attempt again after 2secs..\n");
					fflush(stdout);
					sleep(2);
					}
					else{
					printf("lock success");	
					fflush(stdout);
					
					break;
					}

				} while(lock_ret);

					indexUsernames++;
					strcpy(usernames[indexUsernames], username);
					
					int x; 
					for (x = 0; x <= indexUsernames; x++)
					{
						printf("\n%s\n", usernames[x]);
						fflush(stdout);
					}
					lock_ret = pthread_mutex_unlock(&mutex);

			}
			else
			{
				strcpy(s, request);

			}


			/* Send	the reply to the client. */
			write(newsockfd, s, MAX);
		        nread = read(newsockfd,	&request, MAX);
		}
                close(newsockfd);
	}
	pthread_exit(0);
	return 1;
}
