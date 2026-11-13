
CXX = g++
CXX_FLAGS = -std=c++20 -Ofast

all: runMovies

runMovies: main.o movies.o utilities.o
	$(CXX) $(CXX_FLAGS) -o runMovies main.o movies.o utilities.o

main.o: main.cpp movies.h utilities.h
	$(CXX) $(CXX_FLAGS) -c main.cpp

movies.o: movies.cpp movies.h
	$(CXX) $(CXX_FLAGS) -c movies.cpp

utilities.o: utilities.cpp utilities.h
	$(CXX) $(CXX_FLAGS) -c utilities.cpp

clean:
	rm -f *.o runMovies
