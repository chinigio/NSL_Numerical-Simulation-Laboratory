CC = g++
CFLAGS = -O3 --std=c++14
AFLAGS = -larmadillo
all: es9.exe
es9.exe: es9.o ../PRNG/random.o population.o travel.o
	$(CC) es9.o ../PRNG/random.o population.o travel.o -o es9.exe $(AFLAGS)
	rm -f *.o
es9.o: es9.cpp travel.h
	$(CC) -c es9.cpp -o es9.o $(CFLAGS)
travel.o : travel.cpp travel.h
	$(CC) -c travel.cpp -o travel.o $(CFLAGS)
population.o : population.cpp population.h
	$(CC) -c population.cpp -o population.o $(CFLAGS)
clean :
	rm *.o es9.exe
run:
	time ./es9.exe
