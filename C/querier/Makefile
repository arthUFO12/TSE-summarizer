
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50

OBJS = querier.o 
WOBJS = wordDriver.o

LIBS = ../common/common.a ../libcs50/libcs50.a 
EXEC = querier

WEXEC = wordDriver


$(EXEC): $(OBJS) $(LIBS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(EXEC)

$(WEXEC): $(WOBJS) $(LIBS)
	$(CC) $(CFLAGS) $(WOBJS) $(LIBS) -o $(WEXEC)

querier.o: querier.c
wordDriver.o: wordDriver.c 


../common/common.a:
	make -C ../common

../libcs50/libcs50.a:
	make -C ../libcs50

.PHONY: clean test

clean:
	rm -f *.o $(EXEC) $(WEXEC)
	make -C ../common clean
	make -C ../libcs50 clean

test: $(EXEC)
	bash -v testing.sh

testWord: $(WEXEC)
	bash -v wordtest.sh
