CXX = g++
CXXFLAGS = -g -Wall

graph.o: graph.h graph.cpp
	$(CXX) $(CXXFLAGS) -c graph.cpp

mytest: graph.o mytest.cpp
	$(CXX) $(CXXFLAGS) graph.o mytest.cpp -o mytest

run:
	./mytest

val:
	valgrind ./mytest

gdb:
	gdb ./mytest

clean: 
	rm *.o
	
submit:
	cp graph.h graph.cpp mytest.cpp testdata.txt ~/cs341proj/proj1/