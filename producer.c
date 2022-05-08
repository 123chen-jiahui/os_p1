#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
		printf("请输入一个数以写入共享内存(输入非法字符退出):");
		int ret = scanf("%d", &a);
		if (ret == 0)
			break;
		acquire(shmaddr_lock);
		w = *shmaddr_wr;
		r = *(shmaddr_wr + 1);
		if ((w + 1) % (BUFFERSIZE / 4) == r)
			printf("共享内存已满，无法写入，请通过consumer读出若干数字\n");
		else {
			*(shmaddr_buffer + w) = a;
			w = (w + 1) % (BUFFERSIZE / 4);
			shm_show(shmaddr_buffer, w, r, getpid(), 0, a);
			*shmaddr_wr = w;
		}
		release(shmaddr_lock);
	}
	shmctl(shmid_buffer,IPC_RMID, NULL);
	shmctl(shmid_wr,IPC_RMID, NULL);
	shmctl(shmid_lock,IPC_RMID, NULL);
	return 0;
}
