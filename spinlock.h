typedef struct {
	unsigned int locked;
} spinlock;

void acquire(spinlock *);
void release(spinlock *);
