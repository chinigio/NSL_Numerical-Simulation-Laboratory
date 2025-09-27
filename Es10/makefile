CC = mpicxx
CC_GA = g++
CFLAGS = -O3 --std=c++14
AFLAGS = -larmadillo
all: es10.exe
es10.exe: es10.o ../PRNG/random.o PTA_pop.o travel.o 
	$(CC) es10.o ../PRNG/random.o PTA_pop.o travel.o -o es10.exe $(AFLAGS)
es10.o: es10.cpp travel.h PTA_pop.h
	$(CC) -c es10.cpp -o es10.o $(CFLAGS)
GA_data.exe: GA_data.o ../PRNG/random.o travel.o population.o
	$(CC_GA) ../PRNG/random.o GA_data.o travel.o population.o -o GA_data.exe $(AFLAGS)
GA_data.o: GA_data.cpp travel.h population.h
	$(CC_GA) -c GA_data.cpp -o GA_data.o $(CFLAGS)
travel.o : travel.cpp travel.h
	$(CC) -c travel.cpp -o travel.o $(CFLAGS)
PTA_pop.o : PTA_pop.cpp PTA_pop.h
	$(CC) -c PTA_pop.cpp -o PTA_pop.o $(CFLAGS)
population.o : population.cpp population.h
	$(CC) -c population.cpp -o population.o $(CFLAGS)
clean :
	rm *.o *.exe 
run:
	rm data/best_travel.txt
	time mpiexec -np 8 es10.exe
run_GA:
	time ./GA_data.exe