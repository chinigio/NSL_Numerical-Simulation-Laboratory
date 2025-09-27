CC = g++
CFLAGS = -O3 --std=c++14
AFLAGS = -larmadillo
all: es2.1.exe es2.2.exe
es2.1.exe: es2.1.o position.o ../PRNG/random.o ../function.o
	$(CC) -o es2.1.exe es2.1.o position.o ../PRNG/random.o ../function.o $(AFLAGS)
es2.2.exe: es2.2.o position.o ../PRNG/random.o ../function.o
	$(CC) -o es2.2.exe es2.2.o position.o ../PRNG/random.o ../function.o $(AFLAGS)
es2.1.o: es2.1.cpp position.h
	$(CC) -c es2.1.cpp -o es2.1.o $(CFLAGS)
es2.2.o: es2.2.cpp position.h
	$(CC) -c es2.2.cpp -o es2.2.o $(CFLAGS)
position.o: position.cpp position.h
	$(CC) -c position.cpp -o position.o $(CFLAGS)
clean :
	rm *.o *.exe
run2.1: es2.1.exe
	time ./es2.1.exe
run2.2: es2.2.exe
	time ./es2.2.exe