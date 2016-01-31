#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>     
#include <time.h>       
#include <sys/types.h>
#include <sys/wait.h>

#include "image.h"

int main(int arg_count, char ** args) {
	if(arg_count >= 3) {
		int inc, red=0, green=0, blue=0, procimage=0, status_red=0, status_green=0, status_blue=0, status_procimage=0, repetition=1;
		clock_t start = clock(), end = 0;
		
		if(arg_count == 4) {
		    repetition = strtoul(args[3], 0, 0);
		}
		FILE * tmp[3];
		struct image input = make_image_from_file(args[1]);
		struct image output = make_image(input.type, input.row_count, input.column_count, input.max_value);
		for(inc = 0 ; inc < 3 ; inc++) {
			tmp[inc] = tmpfile();
			if(tmp[inc] == NULL)
				printf("Impossible de creer un fichier temporaire %d.\n", inc+1);
		}
		
		for(int i = 0 ; i < repetition ; ++i) {
		    
		    red = fork();
		    if(!red) {
			    para_blur_image(&input, &output, strtoul(args[2], 0, 0), 0);
			    write_image_to_stream(&output, tmp[0]);
			    rewind(tmp[0]);
			    return 0;
		    } else {
			    green = fork();
			    if(!green) {
				    para_blur_image(&input, &output, strtoul(args[2], 0, 0), 1);
				    write_image_to_stream(&output, tmp[1]);
				    rewind(tmp[1]);
				    return 0;
			    } else {
				    blue = fork();
				    if(!blue) {
					    para_blur_image(&input, &output, strtoul(args[2], 0, 0), 2);
					    write_image_to_stream(&output, tmp[2]);
					    rewind(tmp[2]);
					    return 0;
				    } else {
					    waitpid(red, &status_red, 0);
					    waitpid(green, &status_green, 0);
					    waitpid(blue, &status_blue, 0);
				    }
			    }
			    printf("Passe %d/%d\n", i+1, repetition);
		    }
		    struct image unicol[3];
		    for(inc = 0 ; inc < 3 ; inc++) {
			    unicol[inc] = make_image_from_stream(tmp[inc]);
			    copy_image_layer(&(unicol[inc]), &output, inc);
			    clear_image(&unicol[inc]);
			    rewind(tmp[inc]);
		    }
		    //clear_image(&input);
		    input = output;
		}
		end = clock();
		printf("Temps d'execution : %f\n", (double)(end-start)/CLOCKS_PER_SEC);
		
		FILE * final = tmpfile();
		if(final == NULL)
			printf("Impossible de creer un fichier temporaire %d.\n", inc+1);
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
		fprintf(stderr, "Essaie plutot : %s input.ppm output.ppm 10\n", args[0]);
	}
}
