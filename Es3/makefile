CC = g++
CFLAGS = -Wall -O3 --std=c++11

es3.o: es3.cpp 
	$(CC) -c es3.cpp -o es3.o $(CFLAGS)

es3.exe: es3.o ../PRNG/random.o ../function.o
	$(CC) ../PRNG/random.o ../function.o es3.o -o es3.exe

run: es3.exe
	time ./es3.exe