#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "spinlock.h"
#include "shm.h"

int main() {
	int shmid_buffer, shmid_wr, shmid_lock;
	int *shmaddr_buffer, *shmaddr_wr;
	spinlock *shmaddr_lock;

	shmaddr_buffer = (int *)connect(&shmid_buffer, BUFFER_ID, BUFFERSIZE);
	shmaddr_wr = (int *)connect(&shmid_wr, WR_ID, WRSIZE);
	shmaddr_lock = (spinlock *)connect(&shmid_lock, LOCK_ID, LOCKSIZE);

	int a;
	int w, r; // 写和读指针
	while (1) {
		printf("按回车从共享内存中读出一个数(否则退出程序):");
		if (getchar() != '\n')
			break;

		acquire(shmaddr_lock);
		w = *(shmaddr_wr);
		r = *(shmaddr_wr + 1);
		if (w == r)
			printf("共享内存为空，无法读取，请通过producer写入若干数字\n");
		else {
			a = *(shmaddr_buffer + r);
			r = (r + 1) % (BUFFERSIZE / 4);
			shm_show(shmaddr_buffer, w, r, getpid(), 1, a);
			*(shmaddr_wr + 1) = r;
		}
		release(shmaddr_lock);
	}
	shmctl(shmid_buffer, IPC_RMID, NULL);
	shmctl(shmid_wr, IPC_RMID, NULL);
	shmctl(shmid_lock, IPC_RMID, NULL);
	return 0;
}
