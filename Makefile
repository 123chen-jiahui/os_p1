all: consumer producer mul_test
mul_test: mul_test.o shm.o spinlock.o
	gcc mul_test.o shm.o spinlock.o -o mul_test
consumer: consumer.o shm.o spinlock.o
	gcc consumer.o shm.o spinlock.o -o consumer
producer: producer.o shm.o spinlock.o
	gcc producer.o shm.o spinlock.o -o producer
mul_test.o: mul_test.c shm.c spinlock.c shm.h spinlock.h
	gcc -c mul_test.c shm.c spinlock.c
consumer.o: consumer.c shm.c spinlock.c shm.h spinlock.h
	gcc -c consumer.c spinlock.c shm.c
producer.o: producer.c shm.c spinlock.c shm.h spinlock.h
	gcc -c producer.c spinlock.c shm.c
shm.o: shm.c shm.h
	gcc -c shm.c
spinlock.o: spinlock.c spinlock.h
	gcc -c spinlock.c

clean:
	@echo "cleanning project"
	-rm *.o consumer producer mul_test
	@echo "clean completed"

.PHONY: clean
