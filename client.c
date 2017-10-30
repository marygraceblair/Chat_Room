/*-------------------------------------------------------------*/
/* client.c - sample time/date client.                         */
/*-------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "inet.h"

#define MAX 100

int get_response(char *);
int readn(int, char *, int);

main(int argc, char **argv)
{
    int                 sockfd;
    struct sockaddr_in  serv_addr;
    char                s[MAX];      /* array to hold output */
    int                 response;    /* user response        */
    int                 nread;       /* number of characters */ 

    /* Set up the address of the server to be contacted. */
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr.sin_port        = htons(SERV_TCP_PORT);

    /* Create a socket (an endpoint for communication). */
        if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("client: can't open stream socket");
            return(1);
        }

        /* Connect to the server. */
        if (connect(sockfd, (struct sockaddr *) &serv_addr,
          sizeof(serv_addr)) < 0) {
            perror("client: can't connect to server");
            return(1);
        }



    char   username[10];
    printf("===========================================\n");
    printf("                   Enter Username: \n");
    printf("-------------------------------------------\n");
    fgets(username, 100, stdin);
    printf("===========================================\n");

    printf("%s", username);
    char sendUsername[11];
    char character = ' ';
    int i; 
    int usernameIndex = -1; 



    sendUsername[0] = character; 
    for(i = 0; i < 10; i++)
    {
	    sendUsername[i+1] = username[i];
    }

    char coolResponse[100];
    printf("client username:%s", sendUsername);

    write (sockfd, sendUsername, 10);

	/* Read the server's reply. */
	nread = readn (sockfd, coolResponse, MAX);
	if (nread > 0) {
		printf("   %s\n", coolResponse);
	} else {
		printf("Nothing read. \n");
	}


	nread = 0; 

    
    //close(sockfd); 
	
    /* Display the menu, read user's response, and send it to the server. */
    while( get_response(&s)) {

        /* Create a socket (an endpoint for communication). */
	    /*
        if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("client: can't open stream socket");
            return(1);
        }
	/*

	printf("i am in here now");
	fflush(stdout);
	
        /* Connect to the server. */
	    /*
        if (connect(sockfd, (struct sockaddr *) &serv_addr,
          sizeof(serv_addr)) < 0) {
            perror("client: can't connect to server");
            return(1);
        }
	*/
	

	printf("it worked in here");
	fflush(stdout);

        //printf("%s", s);       
	


         
        /* Send the user's request to the server. */
        write (sockfd, s, 100);
	printf("you did it");
	fflush(stdout);
        
        /* Read the server's reply. */
        nread = readn (sockfd, s, MAX);
        if (nread > 0) {
        	printf("   %s\n", s);
	} else {
		printf("Nothing read. \n");
	}
        close(sockfd);
	nread = 0;
	strcpy(s, "");
    }
    return(0);  /* Exit if response is 4  */
}

/* Display menu and retrieve user's response */
int get_response(char * response)
{
    int choice;

    printf("===========================================\n");
    printf("                   Message: \n");
    printf("-------------------------------------------\n");
    fgets(response, 100, stdin);
    printf("===========================================\n");
    return(1);
}

/*
 * Read up to "n" bytes from a descriptor.
 * Use in place of read() when fd is a stream socket.
 */

int
readn(fd, ptr, nbytes)
register int	fd;
register char	*ptr;
register int	nbytes;
{
	int	nleft, nread;

	nleft = nbytes;
	while (nleft > 0) {
		nread = read(fd, ptr, nleft);
		if (nread < 0)
			return(nread);		/* error, return < 0 */
		else if (nread == 0)
			break;			/* EOF */

		nleft -= nread;
		ptr   += nread;
	}
	return(nbytes - nleft);		/* return >= 0 */
}

