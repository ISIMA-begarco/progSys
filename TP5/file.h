#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct elt {
	char  		* name;
	char		* fich;
	struct elt 	* next;
} elt_t;

typedef struct file {
	elt_t 		  * first;
	elt_t 		  * last;
	int				size;
} file_t;

file_t * create_file();
void push_file(file_t *, char*, char*);
char * top_file(file_t *);
void pop_file(file_t *);
int contains(file_t * f, char * nom);
elt_t * remove_file(file_t * file, char *);
void delete_file(file_t *);

#endif

