CXX = g++
CXXFLAGS = -Wall

hist.o: hist.h hist.cpp
	$(CXX) $(CXXFLAGS) -c hist.cpp
mytest: hist.o mytest.cpp
	$(CXX) $(CXXFLAGS) hist.o mytest.cpp -o mytest
run:
	./mytest

val:
	valgrind ./mytest
clean:
	rm *.o
submit:
	cp hist.h hist.cpp mytest.cpp ~/cs341proj/proj0/