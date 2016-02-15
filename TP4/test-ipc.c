#define _XOPEN_SOURCE

#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "debug.h"

int main()
{
	char const *path = getenv("HOME");
	CHECK_ERRNO(path);
	key_t key = ftok(path, 0);
	CHECK_ERRNO(key != -1);
	size_t size = 100;
	int shm = shmget(key, size, 0666 | IPC_CREAT);
	CHECK_ERRNO(shm >= 0);
	char *buffer = shmat(shm, 0, 0);
	CHECK_ERRNO(buffer != (void *) -1);
	pid_t child = fork();
	CHECK_ERRNO(child >= 0);
	printf("%d: début\n", getpid());

	if(child)
	{
		for(int i = 0; i < 10; ++i)
		{
			printf("%d: message de %d: %s\n", getpid(), child, buffer);
			sleep(1);
		}
		CHECK_ERRNO(!kill(child, SIGTERM));
		CHECK_ERRNO(wait(0) == child);
		CHECK_ERRNO(!shmctl(shm, IPC_RMID, 0));
	}
	else
	{
		for(;;)
		{
			snprintf(buffer, size, "pid=%d, time=%lu", getpid(), time(0));
			sleep(1);
		}
	}

	printf("%d: fin\n", getpid());
}
