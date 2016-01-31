# definition des cibles particulieres
.PHONY: clean

# definition des variables
CXX = gcc
CXXFLAGS = -O2 -Wall -Wextra -g -std=c99 -lpthread

all : horloge paysan clean

paysan : paysan.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)
	
horloge : horloge.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

clean :
	rm -rf *.bak; rm -rf *.o

