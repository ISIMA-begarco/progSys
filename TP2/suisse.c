#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"
#include "time.h"

unsigned seconds = 0;
unsigned temps = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * timer(void* a) {
	while(1) {
		pthread_mutex_lock(&mutex); // verrouillage
		time_t t;
		while(temps == time(&t));
		temps = time(&t);
		pthread_mutex_unlock(&mutex); // déverrouillage
	}
}

void * second(void* a) {
	while(1) {
		pthread_mutex_lock(&mutex); // verrouillage
		printf("%d", ++seconds);
		fflush(stdout);
		pthread_mutex_unlock(&mutex); // déverrouillage
	}
}

int  main() {

	pthread_t threads[2];
	time_t t;
	temps = time(&t);
	pthread_create(&threads[0], 0, timer, 0);
	pthread_create(&threads[1], 0, second, 0);
	
	for(int i = 0; i < 2; ++i)
		pthread_join(threads[i], 0); // attente

	return 0;
}
