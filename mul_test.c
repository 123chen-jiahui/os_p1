#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "spinlock.h"
#include "shm.h"

int main() {
	int a;
	int w, r; // 写和读指针
	pid_t pid;
	srand((int)time(0));
	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error");
		exit(-1);
	}
	if (pid == 0) {
		int *shmaddr_buffer, *shmaddr_wr;
		spinlock *shmaddr_lock;

		shmaddr_buffer = (int *)heavy_connect(BUFFER_ID, BUFFERSIZE);
		shmaddr_wr = (int *)heavy_connect(WR_ID, WRSIZE);
		shmaddr_lock = (spinlock *)heavy_connect(LOCK_ID, LOCKSIZE);

		memset(shmaddr_buffer, 0, BUFFERSIZE);	
		memset(shmaddr_wr, 0, WRSIZE);
		memset(shmaddr_lock, 0, LOCKSIZE);

		sleep(1);

		while (1) {
			acquire(shmaddr_lock);
			w = *(shmaddr_wr);
			r = *(shmaddr_wr + 1);
			if (w == r)
				printf("\033[1;31m共享内存已空，无法读取\033[0m\n");
			else {
				a = *(shmaddr_buffer + r);
				r = (r + 1) % (BUFFERSIZE / 4);
				shm_show(shmaddr_buffer, w, r, getpid(), 1, a);
				*(shmaddr_wr + 1) = r;
			}
			release(shmaddr_lock);
			sleep(rand() % 3);
		}
	} else {
		int *shmaddr_buffer, *shmaddr_wr;
		spinlock *shmaddr_lock;

		shmaddr_buffer = (int *)heavy_connect(BUFFER_ID, BUFFERSIZE);
		shmaddr_wr = (int *)heavy_connect(WR_ID, WRSIZE);
		shmaddr_lock = (spinlock *)heavy_connect(LOCK_ID, LOCKSIZE);

		memset(shmaddr_buffer, 0, BUFFERSIZE);	
		memset(shmaddr_wr, 0, WRSIZE);
		memset(shmaddr_lock, 0, LOCKSIZE);

		sleep(1);

		while (1) {
			a = rand() % 20 + 1;
			acquire(shmaddr_lock);
			w = *(shmaddr_wr);
			r = *(shmaddr_wr + 1);
			if ((w + 1) % (BUFFERSIZE / 4) == r)
				printf("\033[1;31m共享内存已满，无法写入\033[0m\n");
			else {
				*(shmaddr_buffer + w) = a;
				w = (w + 1) % (BUFFERSIZE / 4);
				shm_show(shmaddr_buffer, w, r, getpid(), 0, a);
				*shmaddr_wr = w;
			}
			release(shmaddr_lock);
			sleep(rand() % 3);
		}
		wait(NULL);
	}
	return 0;
}
