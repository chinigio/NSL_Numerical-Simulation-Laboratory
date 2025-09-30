CC = g++
CFLAGS = -Wall -O3 --std=c++11

function.o : function.cpp
	$(CC) -c function.cpp -o function.o $(CFLAGS)
clean :
	rm *.o 
