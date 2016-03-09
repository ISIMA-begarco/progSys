# definition des cibles particulieres
.PHONY: clean

# definition des variables
CXX = gcc
CXXFLAGS = -O2 -Wall -Wextra -g -std=c99

all : serveur bavardage flou clean

flou : flou.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

bavardage : bavardage.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

serveur : serveur.c file.c
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

clean :
	rm -rf *.bak; rm -rf *.o

