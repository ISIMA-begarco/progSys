#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     
#include <time.h>       
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char ** argv) {
	kill(atoi(argv[1]), SIGUSR1);
	return 0;
}
