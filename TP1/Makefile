# definition des cibles particulieres
.PHONY: clean

# definition des variables
CXX = gcc
CXXFLAGS = -O2 -Wall -pedantic -ansi -Wextra -g -std=c99
OBJ = main.o

traitement : $(OBJ)
	$(CXX) $^ -o ./$@ $(CXXFLAGS)

main.o : main.c
	$(CXX) -c $< -o $@ $(CXXFLAGS)
	
#Chaine.o : Chaine.cpp Chaine.hpp
#	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean :
	rm -rf *.bak; rm -rf *.o
