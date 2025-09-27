CC = g++
CFLAGS = -O3 -std=c++14
AFLAGS = -larmadillo

es5.exe: es5.o position.o q_state.o ../PRNG/random.o ../function.o
	$(CC) -o es5.exe es5.o position.o q_state.o ../PRNG/random.o ../function.o $(AFLAGS)
es5.o: es5.cpp position.h q_state.h
	$(CC) -c es5.cpp -o es5.o $(CFLAGS)
position.o: position.cpp position.h
	$(CC) -c position.cpp -o position.o $(CFLAGS)
q_state.o: q_state.cpp q_state.h position.h ../PRNG/random.h
	$(CC) -c q_state.cpp -o q_state.o $(CFLAGS)
clean:
	rm -f *.o es5.exe
run: es5.exe
	time ./es5.exe
