#include "file.h"

file_t * create_file() {
	file_t * retour = (file_t*)malloc(sizeof(file_t));
	retour->first = NULL;
	retour->last = NULL;
	retour->size = 0;
	return retour;
}

void push_file(file_t * f, char* str, char* filed) {
	elt_t * tmp = (elt_t*)malloc(sizeof(elt_t));
	if(tmp) {
		tmp->name = (char*)malloc(sizeof(char)*(strlen(str)+1));
		tmp->fich = (char*)malloc(sizeof(char)*(strlen(filed)+1));
		if((tmp->name)) {
			strcpy(tmp->name, str);
			strcpy(tmp->fich, filed);
			tmp->next = NULL;
			if(f->size > 0) {
				f->last->next = tmp;
				f->last = f->last->next;
			} else {
				f->first = tmp;
				f->last = tmp;
			}
			(f->size)++;
		} else {
			free(tmp);
			tmp = NULL;
		}
	}
}

int contains(file_t * f, char * nom) {
	int retour = 0;
	for(elt_t * cur = f->first ; cur != NULL ; cur = cur->next) {
		if(!strcmp(nom, cur->name)) {
			retour = 1;
		}
	}
	return retour;
}

char * top_file(file_t * f) {
	char * retour = NULL;
	if(f->size > 0) {
		retour = (char*)malloc(sizeof(char)*(strlen(f->first->name)+1));
		strcpy(retour, f->first->name);
	}
	return retour;
}

elt_t * remove_file(file_t * file, char * name) {
	elt_t * retour = file->first;
	elt_t * prec = NULL;
	
	while(retour != NULL && strcmp(retour->name, name)) {
		prec = retour;
		retour = retour->next;
	}

	if(retour != NULL) {
		if(prec == NULL) {
			file->first = retour->next;
		} else {
			if(retour->next == NULL) {
				file->last = prec;
			}
			prec->next = retour->next;
		}
		(file->size)--;
	}

	return retour;
}

void pop_file(file_t * f) {
	elt_t * tmp = f->first;
	if(f->size > 0) {
		f->first = tmp->next;
		(f->size)--;
		unlink(tmp->fich);
		free(tmp->name);
		free(tmp->fich);
		free(tmp);
	}
	if(f->size==0) {
		f->last = NULL;
	}
}

void delete_file(file_t * f) {
	while(f->size != 0)
		pop_file(f);
	free(f);
}


