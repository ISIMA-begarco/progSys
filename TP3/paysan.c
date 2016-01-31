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
	int fd[3][2];
	int result = 0;
	pipe(fd[0]);
	pipe(fd[1]);
	pipe(fd[2]);
	
	/** FILS AINE MEURTRIER */
	fils[0] = fork();
	if(!fils[0]) {
		close(fd[0][1]);
		close(fd[1][1]);
		printf("Fils %d: Je suis le fils %d (le salaud).\n", getpid(), getpid());
		fflush(stdout);

		read(fd[0][0], &result, sizeof(result));	//lecture
		
		printf("Fils %d: Enfoire ! Meurs !\n", getpid());
		fflush(stdout);
		kill(pere, SIGKILL);
		pause();			
		
		return 0;
	}
	
	/** FILS CADET VENGEUR */
	fils[1] = fork();
	if(!fils[1]) {
		close(fd[0][1]);
		close(fd[1][1]);
		printf("Fils %d: Je suis le fils %d (le vengeur).\n", getpid(), getpid());

		fflush(stdout);
		
		/* le dernier fils est ne */
		close(fd[2][0]);	//libere la lecture
		write(fd[2][1], &result, sizeof(result));	//ecriture
		read(fd[1][0], &result, sizeof(result));	//lecture	
		
		printf("Fils %d: Enfoire ! Meurs !\n", getpid());
		fflush(stdout);
		kill(fils[0], SIGKILL);
		printf("Fils %d: Adieu monde cruel !\n", getpid());
		kill(fils[1], SIGKILL);

		return 0;
	}
	
	close(fd[2][1]);	// liberer l'ecriture
	close(fd[0][0]);	//libere la lecture
	close(fd[1][0]);	//libere la lecture
	read(fd[2][0], &result, sizeof(result));	//lecture	
	printf("PERE: Fils %d, j'ai tue ta mere !\n", fils[0]);
	fflush(stdout);
	write(fd[0][1], &result, sizeof(result));	//ecriture
	
	pause();
	
	return 0;
}
