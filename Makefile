CC	= g++
CFLAGS	= -std=c++11 -pg -O2 -fopenmp -ftree-vectorizer-verbose=3 -mavx -mfpmath=sse -floop-optimize
LIB_PATH = /mnt/d/PAC2020/PAC2020_wch/libcheckresult.so
all: logVS

logVS: main.o
	$(CC) -o $@ $^ $(CFLAGS) ${LIB_PATH}

main.o: main.cpp
	$(CC) -c $(CFLAGS) $<
	
.PHONY: clean

clean: 
	rm -f *.o
	rm -f logVS
