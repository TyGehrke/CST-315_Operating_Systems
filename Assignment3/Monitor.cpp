#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // Include for sleep function

class Monitor {
private:
    std::mutex mtx;                   // Mutex for synchronizing access to shared resources
    std::condition_variable empty, full;  // Condition variables for signaling between threads
    int buffer[5];                    // Circular buffer to store produced items
    int in, out, count;               // Indices and count for tracking buffer state
    int theProduct;                   // Counter for produced items

public:
    bool done;                        // Flag indicating whether production/consumption is complete
    Monitor() : in(0), out(0), count(0), done(false), theProduct(0) {}

    int produce() {
        return theProduct++;          // Increment and return the next item to be produced
    }

    void consume(int item, int consumer_id) {
        // Print out the consumer activity
        printf("Consumer %d is consuming item %d\n", consumer_id, item);
    }

    void put(int item, int producer_id) {
        // Print out the producer activity
        printf("Producer %d is producing item: %d\n", producer_id, item);

        std::unique_lock<std::mutex> lock(mtx);  // Acquire the lock for critical section

        while (count == 5) {
            empty.wait(lock);   // Wait if the buffer is full
        }

        buffer[in] = item;      // Put the item into the buffer
        in = (in + 1) % 5;      // Update the index for the next insertion
        count++;                // Increment the item count in the buffer

        lock.unlock();          // Release the lock

        full.notify_all();      // Notify waiting consumers that the buffer is not empty
    }

    int get() {
        int item;

        std::unique_lock<std::mutex> lock(mtx);  // Acquire the lock for critical section

        while (count == 0) {
            full.wait(lock);    // Wait if the buffer is empty
        }

        item = buffer[out];     // Get the item from the buffer
        out = (out + 1) % 5;    // Update the index for the next extraction
        count--;                // Decrement the item count in the buffer

        lock.unlock();          // Release the lock

        empty.notify_all();     // Notify waiting producers that the buffer is not full

        return item;            // Return the consumed item
    }

    void produceItems(int producer_id) {
        for (int i = 0; i < 5; ++i) {
            int item = produce();
            put(item, producer_id);
            sleep(1); // Sleep for 1 second to simulate production time
        }

        done = true;  // Set the done flag when production is complete
        std::cout << "Buffer is full. Waiting for a consumer...\n\n";
    }

    void consumeItems(int consumer_id) {
        for (int i = 0; i < 5; ++i) {
            int item = get();
            consume(item, consumer_id);
            sleep(1); // Sleep for 1 second to simulate consumption time
        }

        done = false;  // Reset the done flag when consumption is complete
        std::cout << "Buffer is empty. Waiting for a producer...\n\n";
    }
};

int main() {
    srand(time(NULL));

    Monitor monitor;

    // Simulate producer behavior
    while (1) {
        if (!monitor.done) {
            sleep(1); // Sleep for 1 second to simulate producer activity
            int producer_id = rand() % 10;
            monitor.produceItems(producer_id);
        }

        // Simulate consumer behavior
        if (monitor.done) {
            sleep(1); // Sleep for 1 second to simulate consumer activity
            int consumer_id = rand() % 10;
            monitor.consumeItems(consumer_id);
        }
    }

    return 0;
}
