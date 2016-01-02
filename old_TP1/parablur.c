#include "image.h"

int main(int arg_count, char **args)
{
	if(arg_count == 4)
	{
		int inc, red=0, green=0, blue=0, status_red=0, status_green=0, status_blue=0;
		FILE * tmp[3];
		struct image input = make_image_from_file(args[1]);
		struct image output = make_image(input.type, input.row_count, input.column_count, input.max_value);
		for(inc = 0 ; inc < 3 ; inc++) {
			tmp[inc] = tmpfile();
			if(tmp[inc] == NULL)
				printf("merde ca marche pas\n");
		}
		red = fork();
		if(!red) {			
			para_blur_image(&input, &output, strtoul(args[3], 0, 0), 0);
			write_image_to_stream(&output, tmp[0]);
			return 0;
		} else {
			green = fork();			
			if(!green) {
				para_blur_image(&input, &output, strtoul(args[3], 0, 0), 1);
				write_image_to_stream(&output, tmp[1]);
				return 0;
			} else {
				blue = fork();
				if(!blue) {
					para_blur_image(&input, &output, strtoul(args[3], 0, 0), 2);	
					write_image_to_stream(&output, tmp[2]);
					return 0;
				} else {
					waitpid(red, &status_red, 0);
					waitpid(green, &status_green, 0);
					waitpid(blue, &status_blue, 0);
					sleep(5);
				}
			}
		}
		struct image unicol[3];
		for(inc = 0 ; inc < 3 ; inc++) {
			unicol[inc] = make_image_from_stream(tmp[inc]);
			copy_image_layer(&(unicol[inc]), &output, inc);
		}
		write_image_to_file(&output, args[2]);
	}
	else
	{
		fprintf(stderr, "Essaie plutôt : %s input.ppm output.ppm 10", args[0]);
	}
}
