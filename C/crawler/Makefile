
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50

OBJS = crawler.o 
LIB = ../libcs50/libcs50.a
COMMON = ../common/common.a
EXEC = crawler



$(EXEC): $(OBJS) $(LIB) $(COMMON)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) $(COMMON) -o $(EXEC)

crawler.o: crawler.c

$(COMMON):
	make -C ../common

$(LIB):
	make -C ../libcs50

.PHONY: clean test

clean:
	rm -f *.o $(EXEC)
	make -C ../common clean
	make -C ../libcs50 clean

test: $(EXEC)
	bash -v testing.sh
