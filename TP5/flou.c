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
#include <sys/shm.h>

#include "image.h"

int main(int arg_count, char ** args) {
	if(arg_count >= 3) {
		int		procimage = 0, status = 0, repetition=1, fils[3] = {0}, shm = 0;
		clock_t start = clock(), end = 0;
		time_t	temps[2]; time(&temps[0]);
		
		/// traitement des arguments
		if(arg_count == 4) {
		    repetition = strtoul(args[3], 0, 0);
		}
		
		/// creation des images
		struct image input = make_image_from_file(args[1]);
		struct image output = make_image(input.type, input.row_count, input.column_count, input.max_value);
		int resultat = open(file, O_RDWR | O_CREAT, 0600);

		/// ouverture des communications
		shm = shmget(ftok("./semtest", 1407), get_image_byte_count(&input), 0660 | IPC_CREAT);
		output.data.as_rgb8 = shmat(shm, 0, 0);
		shmctl(shm, IPC_RMID, 0);
		
		for(int i = 0 ; i < repetition ; ++i) {	/// nombre de repetition

			for(int j = 0 ; j < 3 ; ++j) {	/// traitement par canal
				fils[j] = fork();
				if(!fils[j]) {
					para_blur_image(&input, &output, strtoul(args[2], 0, 0), j);
					shmdt(output.data.as_rgb8);
					return 0;
				}
			}			
			
			/// attente fin de traitement
			for(int j = 0 ; j < 3 ; ++j)
				waitpid(fils[j], &status, 0);

		    printf("Passe %d/%d\n", i+1, repetition);
		}
		end = clock();
		time(&temps[1]);
		printf("Temps d'execution (clks) : %f\n", (double)(end-start)/CLOCKS_PER_SEC);
		printf("Temps d'execution (secs) : %f\n", difftime(temps[1], temps[0]));
		
		
		// sauvegarde
		close(fd);

		// liberation
		munmap(memory, memorySize);
	} else {
		fprintf(stderr, "Essaie plutot : %s input.ppm 10\n", args[0]);
	}
}
