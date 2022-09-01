CXX = g++
CXXFLAGS = -Wall -g

mqueue.o: mqueue.h mqueue.cpp
	$(CXX) $(CXXFLAGS) -c mqueue.cpp

mytest: mqueue.o mytest.cpp
	$(CXX) $(CXXFLAGS) mqueue.o mytest.cpp -o mytest

run:
	./mytest

val:
	valgrind ./mytest

gdb:
	gdb ./mytest

submit:
	cp mqueue.h mqueue.cpp mytest.cpp ~/cs341proj/proj3/