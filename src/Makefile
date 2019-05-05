CC = g++
CPPFLAGS = -std=c++14 -O2

all: main

main: main.o client.o cond_t.o data_t.o database.o entry.o table.o tools.o
	$(CC) $^ -o $@

clean:
	-rm main *.o *.d -f

.PHONY: clean

sources = main.cpp client.cpp cond_t.cpp data_t.cpp database.cpp entry.cpp table.cpp tools.cpp

include $(sources:.cpp=.d)

%.d: %.cpp
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
