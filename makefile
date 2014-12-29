CC = gcc
INC = -I .
CFLAGS = -Wall
LDFLAGS = -lnanomsg -lconfig -llog4c

rescueIM: rescueIM.o config.o
	$(CC) -o rescueIM rescueIM.o config.o $(INC) $(CFLAGS) $(LDFLAGS)
rescueIM.o: rescueIM.c
	$(CC) rescueIM.c $(CFLAGS) -c
config.o: config.c
	$(CC) config.c $(CFLAGS) -c
clean:
	@rm -rf *.o
