all: ncserver.o board.o netchess

ncserver.o:
	$(CC) -c server/ncserver.c

board.o: board.c
	$(CC) -c board.c

netchess: server/ncserver.o board.o netchess.c
	$(CC) -onetchess ncserver.o board.o netchess.c

