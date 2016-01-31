#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     
#include <time.h>       
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define	NOMBRE_FILS	2


int main() {

	int fils[NOMBRE_FILS] = {0,0};
	int pere = getpid();
	int fd[2][2];
	int result = 0;
	pipe(fd[0]);
	pipe(fd[1]);
	
	for(int i = 0 ; i < NOMBRE_FILS ; ++i) {
		fils[i] = fork();
		if(!fils[i]) {
			close(fd[i][1]);
			if(i==0) {
				printf("Fils %d: Je suis le fils %d (le salaud).\n", getpid(), getpid());
			} else {
				printf("Fils %d: Je suis le fils %d (le vengeur).\n", getpid(), getpid());
			}

			fflush(stdout);

			read(fd[i][0], &result, sizeof(result));	//lecture
			
			if(1==i)
				pere = fils[0];		
			
			printf("Fils %d: Enfoire ! Meurs !\n", getpid());
			fflush(stdout);
			kill(pere, SIGTERM);			
			pause();			
			

			return 0;
		}
	}
close(fd[0][0]);
close(fd[1][0]);
	printf("PERE: Fils %d, j'ai tue ta mere !\n", fils[0]);
	fflush(stdout);
	close(fd[0][0]);	//libere la lecture
	write(fd[0][1], &result, sizeof(result));	//ecriture

	for(int i = 0 ; i < NOMBRE_FILS ; ++i) {
		close(fd[i][0]);	//libere la lecture
		//read(fd[i][0], &result, sizeof(result));	//lecture
	}
pause();
	return 0;
}
