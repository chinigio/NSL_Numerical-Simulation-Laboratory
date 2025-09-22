CC = g++
CFLAGS = -Wall -O3 --std=c++11
OBJDIR = ../PRNG
SRCDIR = ../

TARGETS = es1.1.exe es1.2.exe es1.3.exe

all: $(TARGETS)

%.exe: %.o $(OBJDIR)/random.o $(SRCDIR)/function.o
	$(CC) $^ -o $@  

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)  

clean:
	rm -f *.o *.exe  

run%:
	time ./es$*.exe  
