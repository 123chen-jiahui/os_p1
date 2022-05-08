#define BUFFERSIZE 80
#define WRSIZE 8
#define LOCKSIZE 4

#define BUFFER_ID 1
#define WR_ID 2
#define LOCK_ID 3

void *connect(int *, int, int);
void *heavy_connect(int, int);
void shm_show(int *, int, int, int, int, int);
// void write(void *, void *, int, int); // 按字节写入
// void read(void *, void *, int, int); // 按字节读取
