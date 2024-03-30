#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int count = 0;

bool done = false;

sem_t mutex, empty, full;

int theProduct = 0;

int produce() {
    return theProduct++;
}

void consume(int item, int consumer_id) {
    printf("Consumer %d is consuming item %d\n", consumer_id, item);
    out = (out + 1) % BUFFER_SIZE;
}

void put(int item, int producer_id) {
    printf("Producer %d is producing item: %d\n", producer_id, item);

    sem_wait(&empty);
    sem_wait(&mutex);

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    count++;

    sem_post(&mutex);
    sem_post(&full);
}

int get() {
    int item;

    sem_wait(&full);
    sem_wait(&mutex);

    item = buffer[out];
    count--;

    sem_post(&mutex);
    sem_post(&empty);

    return item;
}

int main() {
    srand(time(NULL));

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Produce items sequentially
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        int item = produce();
        put(item, i);
        sleep(1);
    }

    // Consume items sequentially
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        int item = get();
        consume(item, i);
        sleep(1);
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
