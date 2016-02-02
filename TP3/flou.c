#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     
#include <time.h>       
#include <sys/types.h>
#include <sys/wait.h>

#include "image.h"

int main(int arg_count, char ** args) {
	if(arg_count >= 3) {
		int		procimage = 0, status = 0, repetition=1, fils[3] = {0}, fd[2], pix[3] = {0};
		clock_t start = clock(), end = 0;
		time_t	temps[2]; time(&temps[0]);
		char 	message[3] = "00\0";
		
		/// traitement des arguments
		if(arg_count == 4) {
		    repetition = strtoul(args[3], 0, 0);
		}
		
		/// creation des images
		struct image input = make_image_from_file(args[1]);

		/// ouverture des communications		
		pipe(fd);
		
		for(int i = 0 ; i < repetition ; ++i) {	/// nombre de repetition

			for(int j = 0 ; j < 3 ; ++j) {	/// traitement par canal
				pix[j] = 0;
				fils[j] = fork();
				if(!fils[j]) {
					para_blur_image(&input, fd, strtoul(args[2], 0, 0), j);
					return 0;
				}
			}

			// reception des pixels
			int nbPixels = 3 * input.row_count * input.column_count;
			close(fd[1]);	// fermeture ecriture
			while(pix[0]+pix[1]+pix[2] < nbPixels) {
				message[0] = 0; message[1] = 0;
				read(fd[0], &message, sizeof(message));	//lecture
				input.data.as_rgb8[pix[(int)message[0]]++][(int)message[0]] = (int)message[1];
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
		
		/// affichage a l'ecran
		FILE * final = tmpfile();
		if(final == NULL)
			printf("Impossible de creer un fichier temporaire.\n");
		else {
			write_image_to_stream(&input, final);
		    rewind(final);
			
			procimage = fork();
		    if(!procimage) {
				close(STDIN_FILENO);
				if(dup2(fileno(final), STDIN_FILENO) != STDIN_FILENO) {;
					perror("dup2");
				}
				fclose(final);
				if(execlp("display", "display", NULL)==-1) {
					perror("execlp");
				}
			    return 0;
		    }
		}
	} else {
		fprintf(stderr, "Essaie plutot : %s input.ppm 10\n", args[0]);
	}
}
