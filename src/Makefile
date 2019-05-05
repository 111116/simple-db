CC = g++
CPPFLAGS = -std=c++14 -O2

.PHONY: all
all: main

main: main.o cond_t.o data_t.o database.o entry.o table.o tools.o
	$(CC) $^ -o $@

main.o: main.cpp database.h table.h entry.h data_t.h cond_t.h set_t.h tools.h
	$(CC) $(CPPFLAGS) $< -o $@ -c

cond_t.o: cond_t.cpp cond_t.h entry.h data_t.h
	$(CC) $(CPPFLAGS) $< -o $@ -c

data_t.o: data_t.cpp data_t.h
	$(CC) $(CPPFLAGS) $< -o $@ -c

database.o: database.cpp database.h table.h entry.h data_t.h cond_t.h set_t.h tools.h
	$(CC) $(CPPFLAGS) $< -o $@ -c

entry.o: entry.cpp entry.h data_t.h
	$(CC) $(CPPFLAGS) $< -o $@ -c

table.o: table.cpp table.h entry.h data_t.h cond_t.h set_t.h tools.h
	$(CC) $(CPPFLAGS) $< -o $@ -c

tools.o: tools.cpp tools.h
	$(CC) $(CPPFLAGS) $< -o $@ -c

.PHONY: clean
clean:
	rm main *.o -f
