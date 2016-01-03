# definition des cibles particulieres
.PHONY: clean

# definition des variables
CXX = gcc
CXXFLAGS = -O2 -Wall -pedantic -ansi -Wextra -g -pg -std=c99

all : original parallele ameliore clean

ameliore : ameliore.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

parallele : parallele.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

original : original.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

clean :
	rm -rf *.bak; rm -rf *.o
