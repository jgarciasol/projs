CXX = g++
CXXFLAGS = -Wall -g

swarm.o: swarm.h swarm.cpp
	$(CXX) $(CXXFLAGS) -c swarm.cpp

mytest: swarm.o mytest.cpp
	$(CXX) $(CXXFLAGS) swarm.o mytest.cpp -o mytest

run:
	./mytest

val:
	valgrind ./mytest

gdb:
	gdb ./mytest
clean:
	rm *.o
submit:
	cp swarm.h swarm.cpp mytest.cpp ~/cs341proj/proj2/