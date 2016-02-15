#define _XOPEN_SOURCE

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


#define	NBFILS	5
#define	HMAX	24
#define	MMAX	60
#define	SMAX	60

int fils[NBFILS];
int sem = 0;

void safeExit(int s);

int main() {
	time_t t[2];
	int choix = 1;

	sem = semget(ftok("./semtest", 1103), 3, 0660 | IPC_CREAT);             // création
	unsigned short values[] = {0, 0, 0};
	semctl(sem, 0, SETALL, values);	                    // initialisation
	struct sembuf cmd[] = {{0,+1,0},{0,-SMAX,0},{1,+1,0},{1,-MMAX,0},{2,+1,0},{2,-HMAX,0}};   // commande pour operation
	
	signal(SIGTERM, safeExit);      // quitter proprement
	signal(SIGINT, safeExit);       // quoi qu'il arrive
	
	for(int i = 0; i  < NBFILS ; i++) {
		fils[i] = fork();
		if(!fils[i]) {	/// les fils
			if(i==0) { 		/// heures
			
				int heure = 0;
				while(1) {
				
				    semop(sem, cmd+3, 1);	// decrementation non blocante
			        semop(sem, cmd+4, 1);	// incrementation non blocante
				    heure++;
				    if(heure>=HMAX) {
				        semop(sem, cmd+5, 1);	// decrementation non blocante
				        heure = 0;
				    }
					
				}
				
			} else if(i==1) {	/// minutes
			
				while(1) {
				
				    semop(sem, cmd+1, 1);	// decrementation non blocante
					semop(sem, cmd+2, 1);	// incrementation non blocante
										
				}
				
			} else if(i==2) {	/// secondes
			
				time(&t[0]);
				time(&t[1]);
				while(1) {
				
					while(1.00 > difftime(t[1],t[0])) {
						time(&t[1]);
					}
					t[0] = t[1];
					semop(sem, cmd+0, 1);	// incrementation non blocante

				}
				
			} else if(i==3) {   // programme pour bip
			
		        unsigned short valeur = 0, old = 0;
                while(1) {
                
			        semctl(sem, 0, GETALL, values);
			        valeur = values[2];
			        if(valeur != old) {
					    fflush(stdout);
    			        for(unsigned short incBip = 0 ; incBip < valeur ; incBip++) {
	    		            printf("\aBIP ");
					        fflush(stdout);
	    		        }
	    		        printf("\n");
					    fflush(stdout);
	    		        old = valeur;
	    		    }
	    		    
			    }
			
			} else if(i==4) {
			
				while(choix) {
				
					puts("Horloge Suisse\n--------------\n");
					puts("1 - Afficher l'heure");
					puts("0 - Quitter");
					fflush(stdout);
					scanf("%d%*c", &choix);

					if(1==choix) {
					
						semctl(sem, 0, GETALL, values);
					    printf("Il est : %02d:%02d:%02d\n\n", values[2], values[1], values[0]);
						fflush(stdout);
						
					} else if(0==choix) {
					
					    safeExit(0);
					    
					}
					
				}
				
			}
			return 0;
		}
	}
	
	pause();
	
	return 0;
}

void safeExit(int s) {
    s = s;
    for(int i = 0 ; i < NBFILS ; i++) {
        kill(fils[i], SIGTERM);
    }
    if(semctl(sem, 0, IPC_RMID) == -1) {
        perror("Erreur lors de la suppression du tableau de semaphores ");
        exit(EXIT_FAILURE);
    } else {
        printf("SEMAPHORE LIBERE\n");
    }
    exit(EXIT_SUCCESS);
}
