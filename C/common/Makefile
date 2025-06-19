CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
LIB = common.a
OBJS = pagedir.o word.o index.o scoreboard.o union.o


$(LIB):$(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h 
word.o: word.h
index.o: index.h
union.o: union.h
scoreboard.o: scoreboard.h

.PHONY: clean

clean:
	rm -f *.a *.o
