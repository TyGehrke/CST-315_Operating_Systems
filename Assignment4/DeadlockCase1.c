#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;

// Function representing the first resource
void *resource1()
{
  // Acquire lock for resource1
  pthread_mutex_lock(&lock1);

  // Job in resource1
  printf("Job started in resource1..\n");
  sleep(2);

  // Attempt to acquire lock for resource2
  printf("Trying to get resource2\n");
  pthread_mutex_lock(&lock2); 
  printf("Acquired resource2\n");
  pthread_mutex_unlock(&lock2);

  // Job finished in resource1
  printf("Job finished in resource1..\n");

  // Release lock for resource1
  pthread_mutex_unlock(&lock1);

  // Exit the thread
  pthread_exit(NULL);
}

// Function representing the second resource
void *resource2()
{
  // Acquire lock for resource2
  pthread_mutex_lock(&lock2);

  // Job in resource2
  printf("Job started in resource2..\n");
  sleep(2);

  // Attempt to acquire lock for resource1
  printf("Trying to get resource1\n");
  pthread_mutex_lock(&lock1); 
  printf("Acquired resource1\n");
  pthread_mutex_unlock(&lock1);

  // Job finished in resource2
  printf("Job finished in resource2..\n");

  // Release lock for resource2
  pthread_mutex_unlock(&lock2);

  // Exit the thread
  pthread_exit(NULL);
}

int main() 
{
  // Initialize mutex locks
  pthread_mutex_init(&lock1, NULL);
  pthread_mutex_init(&lock2, NULL);

  // Create two threads, each representing a resource
  pthread_t t1, t2;
  pthread_create(&t1, NULL, resource1, NULL);
  pthread_create(&t2, NULL, resource2, NULL);

  // Wait for both threads to finish
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  // Destroy mutex locks
  pthread_mutex_destroy(&lock1);
  pthread_mutex_destroy(&lock2);

  // Exit the program
  return 0;
}
