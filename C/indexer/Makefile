
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50

OBJS = indexer.o
TOBJS = indextest.o

LIBS = ../common/common.a ../libcs50/libcs50.a

EXEC = indexer
TEXEC = indextest

# Main target
all: $(EXEC) $(TEXEC)

# Makes indexer executable
$(EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS)  -o $(EXEC)

# Makes indextest executable
$(TEXEC): $(TOBJS) $(LIBS) 
	$(CC) $(CFLAGS) $(TOBJS) $(LIBS)  -o $(TEXEC)


.PHONY: all test clean

indexer.o: indexer.c
indextest.o: indextest.c

../common/common.a:
	make -C ../common common.a

../libcs50/libcs50.a:
	make -C ../libcs50 libcs50.a


.PHONY: clean test


# clean target
clean:
	rm -f *.o $(EXEC) $(TEXEC) indexcmp
	make -C ../common clean
	make -C ../libcs50 clean

# Test target
test: all
	bash -v testing.sh

# Uses indexcmp to validate indexed an reindexed files
# Might not be able to copy indexcmp if directory names are 
# different, change target above
validate: indexcmp
	bash -v validate.sh

indexcmp:
	cp ~/cs50-dev/shared/tse/indexcmp indexcmp