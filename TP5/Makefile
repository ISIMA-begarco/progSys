# definition des cibles particulieres
.PHONY: clean

# definition des variables
CXX = gcc
CXXFLAGS = -O2 -Wall -Wextra -g -std=c99

all : flou clean

flou : flou.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

bavardage : bavardage.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

clean :
	rm -rf *.bak; rm -rf *.o

