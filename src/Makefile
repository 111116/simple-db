CC = g++
CFLAGS = -std=c++14

.PHONY: all
all:

client.o:

cond_t.o:

data_t.o:

database.o:

entry.o:

set_t.o:

table.o: entry.h data_t.h cond_t.h set_t.h
	g++ table.cpp -o table.o -c

.PHONY: clean
clean:
	rm -f *.o yoursql
