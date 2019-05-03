CC = g++
CFLAGS = -std=c++14 -O2

.PHONY: all
all: client.o data_t.o database.o table.o tools.o entry.o
	$(CC) -O2 -o simple-db client.o data_t.o database.o table.o tools.o entry.o

client.o: client.cpp client.h tools.h database.h table.h entry.h data_t.h cond_t.h set_t.h
	$(CC) $(CFLAGS) -o client.o -c client.cpp

data_t.o: data_t.cpp data_t.h
	$(CC) $(CFLAGS) -o data_t.o -c data_t.cpp

database.o: database.cpp database.h table.h entry.h data_t.h cond_t.h set_t.h
	$(CC) $(CFLAGS) -o database.o -c database.cpp

table.o: table.cpp table.h entry.h data_t.h cond_t.h set_t.h
	$(CC) $(CFLAGS) -o table.o -c table.cpp

tools.o: tools.cpp tools.h
	$(CC) $(CFLAGS) -o tools.o -c tools.cpp

entry.o: entry.cpp entry.h
	$(CC) $(CFLAGS) -o entry.o -c entry.cpp

.PHONY: clean
clean:
	rm -f *.o simple-db
