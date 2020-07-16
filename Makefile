CC	= g++
CFLAGS	= -std=c++11 -pg -O2 -fopenmp -ftree-vectorizer-verbose=3 -mavx -mfpmath=sse -floop-optimize

all: logVS

logVS: main.o check_result.o
	$(CC) -o $@ $^ $(CFLAGS)

main.o: main.cpp
	$(CC) -c $(CFLAGS) $<
check_result.o: check_result.cpp
	$(CC) -c $(CFLAGS) $<
.PHONY: clean

clean: 
	rm -f *.o
	rm -f logVS
