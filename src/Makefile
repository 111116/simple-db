CC = g++
CFLAGS = -std=c++14 -O2

.PHONY: all
all: client.o data_t.o database.o table.o
	g++ client.o data_t.o database.o table.o -o simple-db -O2

client.o: client.cpp client.h tools.h database.h table.h entry.h data_t.h cond_t.h set_t.h
	g++ client.cpp -o client.o -c -std=c++14 -O2

data_t.o: data_t.cpp data_t.h
	g++ data_t.cpp -o data_t.o -c -std=c++14 -O2

database.o: database.cpp database.h table.h entry.h data_t.h cond_t.h set_t.h
	g++ database.cpp -o database.o -c -std=c++14 -O2

table.o: table.cpp table.h entry.h data_t.h cond_t.h set_t.h
	g++ table.cpp -o table.o -c -std=c++14 -O2

.PHONY: clean
clean:
	rm -f *.o simple-db
