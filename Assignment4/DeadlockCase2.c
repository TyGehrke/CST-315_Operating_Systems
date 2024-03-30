#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;
int iteration_count = 1;

// Function representing the first resource
void *resource1()
{
    while (1)
    {
        // Acquire lock for resource1
        pthread_mutex_lock(&lock1);
        printf("\nStarting Iteration %d\n", iteration_count);
        printf("Iteration %d: Job started in resource1..\n", iteration_count);
        sleep(2);

        // Attempt to acquire lock for resource2
        printf("Iteration %d: Trying to get resource2\n", iteration_count);
        pthread_mutex_lock(&lock2);
        printf("Iteration %d: Acquired resource2\n", iteration_count);
        pthread_mutex_unlock(&lock2);

        // Job finished in resource1
        printf("Iteration %d: Job finished in resource1..\n\n", iteration_count);

        // Release lock for resource1
        pthread_mutex_unlock(&lock1);

        // Increment iteration count and introduce a delay for better visibility
        iteration_count++;
        sleep(1);
    }

    // Exit the thread
    pthread_exit(NULL);
}

// Function representing the second resource
void *resource2()
{
    while (1)
    {
        // Acquire lock for resource2
        pthread_mutex_lock(&lock1);
        printf("\nStarting Iteration %d\n", iteration_count);
        printf("Iteration %d: Job started in resource2..\n", iteration_count);
        sleep(2);

        // Attempt to acquire lock for resource1
        printf("Iteration %d: Trying to get resource1\n", iteration_count);
        pthread_mutex_lock(&lock2);
        printf("Iteration %d: Acquired resource1\n", iteration_count);
        pthread_mutex_unlock(&lock2);

        // Job finished in resource2
        printf("Iteration %d: Job finished in resource2..\n\n", iteration_count);

        // Release lock for resource2
        pthread_mutex_unlock(&lock1);

        // Increment iteration count and introduce a delay for better visibility
        iteration_count++;
        sleep(1);
    }

    // Exit the thread
    pthread_exit(NULL);
}

// Main function
int main()
{
    // Initialize mutex locks
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    while (1)
    {
        // Create two threads, each representing a resource
        pthread_t t1, t2;
        pthread_create(&t1, NULL, resource1, NULL);
        pthread_create(&t2, NULL, resource2, NULL);

        // Wait for both threads to finish
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        // Increment iteration count
        iteration_count++;
    }

    // Destroy mutex locks
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    // Exit the program
    return 0;
}
