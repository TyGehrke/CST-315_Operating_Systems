#include <stdio.h>
#include <iostream>
#include <pthread.h>	// Include pthread
#include <unistd.h> 	// Include the sleep function
#include <cstdlib>  	// Include the rand function
#include <ctime>    	// Include the time function

#define BUFFER_SIZE 5	// Setting the size of the of the Queue to 5

int buffer[BUFFER_SIZE];		// Creating the array of data for the queue

// Defining necessary variables
int in = 0;
int out = 0;
int count = 0;


// setting up the threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t producer_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t consumer_cond = PTHREAD_COND_INITIALIZER;

int theProduct = 0; // Initialize theProduct to 0

// Function to produce an item
int produce() {
    return theProduct++;
}

// Function to consume an item
void consume(int i, int slot, int consumer_id) {
	// print out the consumer activity
    printf("Consumer %d consumed item %d from slot %d\n", consumer_id, i, slot);

    out = (out + 1) % BUFFER_SIZE; // Update the out index
}

// Function to put an item into the buffer
void put(int item, int slot, int producer_id) {
	// print out the producer activity
    printf("Producer %d put %d in slot %d\n", producer_id, item, slot);

    // Acquire the lock on the mutex to enter a critical section,
	// ensuring exclusive access to shared resources.
	pthread_mutex_lock(&mutex);

    while (count == BUFFER_SIZE) {
        pthread_cond_wait(&producer_cond, &mutex);
    }

	// push the item into the queue, and update variables accordingly
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE; // Update the in index
    std::cout << "Increment in!\n";
    count++;

    pthread_cond_signal(&consumer_cond);
    pthread_mutex_unlock(&mutex);
}

// Function to get an item from the buffer
int get(int slot) {
    int item;

    pthread_mutex_lock(&mutex);
    while (count == 0) {
        pthread_cond_wait(&consumer_cond, &mutex);
    }


	// retrieve the item and update the count accordingly
    item = buffer[out];
    count--;

    pthread_cond_signal(&producer_cond);
    pthread_mutex_unlock(&mutex);

    return item;
}

// Producer thread function
void *producer(void *arg) {
	// infinite loop
    while(1) {
		// this to ensure the queue is clear before the producer begin production
        if(count == 0) {
			// create a random id
            int producer_id = rand() % 10;
            for (int i = 0; i < BUFFER_SIZE; ++i) {
                int item = produce();
                put(item, in, producer_id); // Pass the slot number to put() function
                sleep(1);      // Sleep for 1 second
            }
        }
    }
    return NULL;
}

// Consumer thread function
void *consumer(void *arg) {
	// infinite loop
    while(1) {

		// only comsume when the queue is full
        if(count == BUFFER_SIZE) {
			sleep(1);

			// create a random id
            int consumer_id = rand() % 10;
            for (int i = 0; i < BUFFER_SIZE; ++i) {
                int item = get(out); // Pass the slot number to get() function
                consume(item, out, consumer_id);
                sleep(1); // Sleep for 1 second
            }
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL)); // Seed the random number generator with the current time

    // Create threads for producer and consumer
    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Let the main function run indefinitely
    while (1) {
        
    }

    return 0;
}
