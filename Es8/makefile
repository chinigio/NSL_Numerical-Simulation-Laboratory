CC = g++
CFLAGS = -O3 --std=c++14
AFLAGS = -larmadillo
all: es8.1.exe es8.2.exe
es8.1.exe: es8.1.o state.o ../PRNG/random.o ../function.o
	$(CC) -o es8.1.exe es8.1.o state.o ../PRNG/random.o ../function.o 
es8.2.exe: es8.2.o SA_state.o state.o ../PRNG/random.o ../function.o
	$(CC) -o es8.2.exe es8.2.o SA_state.o state.o ../PRNG/random.o ../function.o 
es8.1.o: es8.1.cpp state.h
	$(CC) -c es8.1.cpp -o es8.1.o $(CFLAGS)
es8.2.o: es8.2.cpp state.h SA_state.h
	$(CC) -c es8.2.cpp -o es8.2.o $(CFLAGS)
SA_state.o: SA_state.cpp SA_state.h state.h
	$(CC) -c SA_state.cpp -o SA_state.o $(CFLAGS)
state.o: state.cpp state.h
	$(CC) -c state.cpp -o state.o $(CFLAGS)
run8.1: es8.1.exe
	time ./es8.1.exe
run8.2: es8.2.exe
	time ./es8.2.exe
clean:
	rm *.o *.exe