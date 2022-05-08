#include "shm.h"
#include <sys/types.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *connect(int *shmid, int proj_id, int size) {
	key_t key;
	void *shmadd;

	if ((key = ftok("./", proj_id)) == -1)
		perror("ftok error");

	if ((*shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {
		perror("shmget error");
		exit(-1);
	}	
	printf("connect shared-memory success, with shmid: %d\n", *shmid);

	if ((shmadd = shmat(*shmid, NULL, 0)) < 0) {
		perror("shmat error");
		exit(-1);
	}
	printf("%p\n", shmadd);
	return shmadd;	
}

void *heavy_connect(int proj_id, int size) {
	int shmid;
	key_t key;
	void *shmadd;

	if ((key = ftok("./", proj_id)) == -1)
		perror("ftok error");

	if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {
		perror("shmget error");
		exit(-1);
	}	
	printf("connect shared-memory success, with shmid: %d\n", shmid);

	if ((shmadd = shmat(shmid, NULL, 0)) < 0) {
		perror("shmat error");
		exit(-1);
	}
	printf("%p\n", shmadd);
	return shmadd;	
}


void shm_show(int *shmaddr, int w, int r, int pid, int flag, int v) {
	if (flag)
		printf("=========================\n");
	else
		printf("+++++++++++++++++++++++++\n");
	printf("当前进程pid为 %d\n", pid);
	printf("当前进程为 ");
	if (flag)
		printf("\033[42;35m消费者\033[0m\n");
	else printf("\033[42;35m生产者\033[0m\n");

	if (flag)
		printf("读取的数为%d\n", v);
	else 
		printf("写入的数为%d\n", v);

	if (w == r)
		printf("缓冲区空\n");
	else {
		printf("缓冲区中内容为:");
		for (int i = r; i != w; i = (i + 1) % (BUFFERSIZE / 4))
			printf("%d ", *(shmaddr + i));
		printf("\n");
	}
	printf("write端为%d\n", w);
	printf("read端为%d\n", r);
	if (flag)
		printf("=========================\n");
	else
		printf("+++++++++++++++++++++++++\n");
	printf("\n");
}

/*
void write(void *shmaddr, void *src, int offset, int size) {
	for (int i = 0; i < size; i += 4)
		*(shmaddr + offset * 4 + i) = *(src + i);
}

void read(void *shmaddr, void *dst, int offset, int size) {
	for (int i = 0; i < size; i += 4)
		*(dst + i) = *(shmaddr + offset * 4 + i);
}
*/
