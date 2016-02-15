# definition des cibles particulieres
.PHONY: clean

# definition des variables
CXX = gcc
CXXFLAGS = -O2 -Wall -Wextra -g -std=c99

all : flou horloge test-ipc clean

flou : flou.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

test-ipc : test-ipc.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)
	
horloge : horloge.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

clean :
	rm -rf *.bak; rm -rf *.o

