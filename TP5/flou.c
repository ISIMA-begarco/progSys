#define _XOPEN_SOURCE	500

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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#include "image.h"

int main(int arg_count, char ** args) {
	if(arg_count >= 4) {
		int		status = 0, repetition=1, fils[3] = {0};
		clock_t start = clock(), end = 0;
		time_t	temps[2]; time(&temps[0]);

		/// traitement des arguments
		if(arg_count == 5) {
		    repetition = strtoul(args[4], 0, 0);
		}

		/// creation des images
		char * file = args[2];
		struct image input = make_image_from_file(args[1]);
		int fd = open(file, O_RDWR | O_CREAT, 0600); //ouverture
		if(fd <= 0)
			perror("Error: ");
		char * memory = mmap(0, get_image_byte_count(&input), PROT_WRITE, MAP_SHARED, fd, 0);
		ftruncate(fd, get_image_byte_count(&input));

		for(int i = 0 ; i < repetition ; ++i) {	/// nombre de repetition
			int length = sprintf(memory, "P%u %u %u %u\n", input.type, input.column_count, input.row_count, input.max_value);	/// ecriture du header
			for(int j = 0 ; j < 3 ; ++j) {	/// traitement par canal
				fils[j] = fork();
				if(!fils[j]) {
					para_blur_image(&input, &(memory[length]), strtoul(args[3], 0, 0), j);
					return 0;
				}
			}

			/// attente fin de traitement
			for(int j = 0 ; j < 3 ; ++j)
				waitpid(fils[j], &status, 0);
			
		    printf("Passe %d/%d\n", i+1, repetition);
		}

		// liberation
		munmap(memory, get_image_byte_count(&input));
		close(fd);
		end = clock();
		time(&temps[1]);
		printf("Temps d'execution (clks) : %f\n", (double)(end-start)/CLOCKS_PER_SEC);
		printf("Temps d'execution (secs) : %f\n", difftime(temps[1], temps[0]));
	} else {
		fprintf(stderr, "Essaie plutot : %s input.ppm output.ppm 10\n", args[0]);
	}
}
