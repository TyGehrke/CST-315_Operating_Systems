#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resourceMutex;
int resourceAvailable = 1;

// Function representing a concurrent process
void *process(void *processID)
{
    int id = *((int *)processID);

    while (1)
    {
        // Display the current state of the process
        printf("Process %d is running.\n", id);

        // Acquire lock for resource access
        pthread_mutex_lock(&resourceMutex);

        if (resourceAvailable)
        {
            // Attempt to access the resource if available
            printf("Process %d is trying to access the resource.\n", id);
            resourceAvailable = 0; // Mark resource as unavailable
            pthread_mutex_unlock(&resourceMutex);

            // Simulate the process working with the resource
            sleep(2);

            // Display successful access to the resource
            printf("Process %d successfully accessed the resource.\n", id);

            // Release the resource and mark it as available
            pthread_mutex_lock(&resourceMutex);
            resourceAvailable = 1;
            pthread_mutex_unlock(&resourceMutex);
        }
        else
        {
            // Process is starved, simulate timer stopping the process
            printf("Process %d is starved. Timer will stop the process.\n", id);
            pthread_mutex_unlock(&resourceMutex);
            
            // Simulate timer stopping the process
            sleep(1);
            
            // Display that the process has restarted
            printf("Process %d restarted.\n", id);
        }

        // Introduce some delay between iterations
        sleep(1);
    }

    // Exit the thread
    pthread_exit(NULL);
}

// Main function
int main()
{
    pthread_t processes[3]; // You can adjust the number of processes as needed
    int processIDs[3] = {1, 2, 3};

    // Initialize mutex for resource access
    pthread_mutex_init(&resourceMutex, NULL);

    // Create threads for each process
    for (int i = 0; i < 3; ++i)
    {
        pthread_create(&processes[i], NULL, process, (void *)&processIDs[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 3; ++i)
    {
        pthread_join(processes[i], NULL);
    }

    // Destroy mutex after use
    pthread_mutex_destroy(&resourceMutex);

    // Exit the program
    return 0;
}
