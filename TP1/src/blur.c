#include "image.h"

int main(int arg_count, char **args)
{
	if(arg_count == 4)
	{
		struct image input = make_image_from_file(args[1]);
		struct image output = make_image(input.type, input.row_count, input.column_count, input.max_value);
		blur_image(&input, &output, strtoul(args[3], 0, 0));
		write_image_to_file(&output, args[2]);
	}
	else
	{
		fprintf(stderr, "Essaie plutôt : %s input.ppm output.ppm 10", args[0]);
	}
}
