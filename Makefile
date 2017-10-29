#
# Makefile for Example
#
LIBS	= -pthread
CFLAGS	= 

all:	tcp 

#
# Internet stream version (TCP protocol).
#

tcp:	server client 

client.o server.o: inet.h

server:	server.c 
	gcc $(CFLAGS) -o server5 server.c $(LIBS)

client:	client.c 
	gcc $(CFLAGS) -o client5 client.c $(LIBS)


#
# Clean up the mess we made
#
clean:
	rm *.o \
	server5 client5
 
