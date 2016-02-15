#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     
#include <time.h>       
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define	NBFILS	4
#define	HMAX	2
#define	MMAX	6
#define	SMAX	6

int heure = 0;
int pere = 0;
int result = 0;
int fils[NBFILS];
int fd[3][2];

void display1(int s);
void display2(int s);
void display3(int s);
void reset(int s);
void inc1(int s);
void inc2(int s);

int main() {
	time_t t[2];
	int choix = 1;
	pipe(fd[0]);
	pipe(fd[1]);
	pipe(fd[2]);

	int sem = semget(IPC_PRIVATE, 3, 0600); // création
	unsigned short values[] = {SMAX, MMAX, HMAX};
	semctl(sem, 0, SETALL, values);	// initialisation
	
	pere = getpid();
	
	for(int i = 0; i  < NBFILS ; i++) {
		fils[i] = fork();
		if(!fils[i]) {	/// les fils
			if(i==0) { 		/// heures
				signal(SIGUSR1, display1);
				signal(SIGUSR2, reset);
				signal(SIGALRM, inc1);
				close(fd[0][1]);	// fermeture ecriture
				while(1) {
					close(fd[0][1]);	// fermeture ecriture
					result = 0;
					read(fd[0][0], &result, sizeof(result));	//lecture
					if(result==5) {
						heure++;
						if(heure>=HMAX) {
							heure=0;
						}
					}
				}
			} else if(i==1) {	/// minutes
				signal(SIGUSR1, display2);
				signal(SIGUSR2, reset);
				signal(SIGALRM, inc2);
				heure=0;
				close(fd[0][0]);	// fermeture lecture
				close(fd[1][1]);	// fermeture ecriture
				while(1) {
					struct sembuf cmd[] = {{0,0,0},{1,+1,0},{2,0,0}};
					semop(sem, &cmd, 0);	// attente
					heure++;
					semop(sem, &cmd+1, 1);	// attente
					if(heure>=MMAX) {
						heure=0;
					}
				}
			} else if(i==2) {	/// secondes
				signal(SIGUSR1, display3);
				signal(SIGUSR2, reset);
				signal(SIGALRM, inc2);
				heure=0;
				time(&t[0]);
				time(&t[1]);
				close(fd[1][0]);	// fermeture lecture
				struct sembuf cmd[] = {{0,+1,0},{1,0,0},{2,0,0}};
				while(1) {
					while(1.00 > difftime(t[1],t[0])) {
						time(&t[1]);
					}
					heure++;
					t[0] = t[1];
					semop(sem, &cmd, 0);	// increment
					semctl(sem, 0, GETALL, values);
					printf("GETALL–> %d %d %d\n", values[0], values[1], values[2]);
					if(heure>=SMAX) {
						heure=0;
						semctl(sem, 0, SETVAL, -SMAX);
					}
				}
			} else if(i==3) {
				while(choix) {
					puts("Horloge Suisse\n--------------\n");
					puts("1 - Afficher l'heure");
					puts("2 - Incrementer les secondes");
					puts("3 - Incrementer les minutes");
					puts("4 - Incrementer les heures");
					puts("5 - Reset");
					puts("0 - Quitter");
					fflush(stdout);
					scanf("%d", &choix);

					if(1==choix) {
						printf("Il est : ");
						fflush(stdout);
						for(int j = 0 ; j < 3 ; ++j) {
							kill(fils[j], SIGUSR1);
							fflush(stdout);
							close(fd[2][1]);	// fermeture ecriture
							read(fd[2][0], &result, sizeof(result));	//lecture
						}
					} else if(2==choix) {
						kill(fils[2], SIGALRM);
						puts("Secondes incrementees.\n");
					} else if(3==choix) {
						kill(fils[1], SIGALRM);
						puts("Minutes incrementees.\n");
					} else if(4==choix) {
						kill(fils[0], SIGALRM);
						puts("Heures incrementees.\n");
					} else if(5==choix) {
						for(int j = 0 ; j < 3 ; ++j) {
							kill(fils[j], SIGUSR2);
						}
						puts("Horloge remise a zero.\n");
					} else if(0==choix) {
						kill(pere, SIGTERM);
						for(int j = 0 ; j < NBFILS ; j++) {
							kill(fils[j], SIGTERM);
						}
					}
				}
			}
			return 0;
		}
	}
	
	pause();
	
	return 0;
}


void display1(int s) {
	printf("%02d:", heure);
	fflush(stdout);
	signal(SIGUSR1, display1);
	close(fd[2][0]);	// fermeture lecture
	write(fd[2][1], &result, sizeof(result));	//ecriture
}

void display2(int s) {
	printf("%02d:", heure);
	fflush(stdout);
	signal(SIGUSR1, display2);
	close(fd[2][0]);	// fermeture lecture
	write(fd[2][1], &result, sizeof(result));	//ecriture
}

void display3(int s) {
	printf("%02d\n\n", heure);
	fflush(stdout);
	signal(SIGUSR1, display3);
	close(fd[2][0]);	// fermeture lecture
	write(fd[2][1], &result, sizeof(result));	//ecriture
}

void reset(int s) {
	heure = 0;
	signal(SIGUSR2, reset);
}

void inc1(int s) {
	heure = heure+1>=HMAX?0:heure+1;
	signal(SIGALRM, inc1);
}

void inc2(int s) {
	heure = heure+1>=SMAX?0:heure+1;
	signal(SIGALRM, inc2);
}
