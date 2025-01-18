Threads in C allow you to create **concurrent tasks** within a program. 

Multithreading allows a program to perform multiple tasks concurrently, enhancing performance, especially on multi-core systems. Threads within the same process share resources such as memory space, file descriptors, and global variables, but each maintains its own stack, program counter, and set of registers.

---

### **Advantages of Using Threads**

- **Responsiveness**: Multithreading enables a program to remain responsive even if some threads are blocked or performing lengthy operations.
- **Resource Sharing**: Threads share the same address space, facilitating efficient communication and data sharing between them.
- **Performance**: On multi-core processors, threads can execute simultaneously on different cores, leading to improved performance through parallelism.
- **Scalability**: Multithreaded applications can handle increasing workloads more effectively by distributing tasks across multiple threads.

---

### **POSIX Threads (pthreads) Library**

The POSIX standard defines a thread API known as **pthreads** for C/C++ programming. This API provides a set of functions for thread creation, management, and synchronization. It's widely supported on Unix-like systems, including Linux and macOS.

---

### **Creating and Managing Threads**

To work with threads in C, include the pthread library by adding `#include <pthread.h>` at the beginning of your program. When compiling, link the pthread library using the `-pthread` flag with `gcc`.

### **1. Creating a Thread**

Use `pthread_create` to initiate a new thread:

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thread_function(void* arg) {
    // Thread code here
    return NULL;
}

int main() {
    pthread_t thread;
    int result;

    result = pthread_create(&thread, NULL, thread_function, NULL);
    if (result != 0) {
        fprintf(stderr, "Error creating thread\n");
        return EXIT_FAILURE;
    }

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    return EXIT_SUCCESS;
}

```

- **Parameters**:
    - `pthread_t *thread`: Pointer to the thread identifier.
    - `const pthread_attr_t *attr`: Thread attributes (use `NULL` for default settings).
    - `void *(*start_routine)(void *)`: Function to be executed by the thread.
    - `void *arg`: Argument passed to the thread function.

### **2. Waiting for a Thread to Finish**

Use `pthread_join` to wait for a thread's termination:

```c
pthread_join(thread, NULL);

```

- **Parameters**:
    - `pthread_t thread`: Thread identifier.
    - `void **retval`: Pointer to the return value of the thread (use `NULL` if not needed).

### **3. Terminating a Thread**

A thread can terminate by returning from its start function or by calling `pthread_exit`:

```c
pthread_exit(NULL);

```

- **Note**: `pthread_exit` allows other threads to continue execution; calling `exit` would terminate all threads in the process.

---

### **Thread Synchronization**

Proper synchronization is crucial to prevent race conditions and ensure data consistency when multiple threads access shared resources.

### **1. Mutexes**

A **mutex** (mutual exclusion) ensures that only one thread accesses a critical section at a time.

- **Initializing a Mutex**:
    
    ```c
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
    ```
    
- **Locking and Unlocking a Mutex**:
    
    ```
    pthread_mutex_lock(&mutex);
    // Critical section
    pthread_mutex_unlock(&mutex);
    
    ```
    
- **Destroying a Mutex**:
    
    ```
    pthread_mutex_destroy(&mutex);
    ```
    

### **2. Condition Variables**

Condition variables allow threads to wait for certain conditions to be met.

- **Declaring and Initializing**:
    
    ```
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    
    ```
    
- **Waiting a Condition**:
    
    ```
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    // Condition met
    pthread_mutex_unlock(&mutex);
    
    ```
    
- **Signaling a Condition**:
    
    ```
    pthread_cond_signal(&cond);
    
    ```
    
- **Destroying a Condition Variable**:
    
    ```
    pthread_cond_destroy(&cond);
    
    ```
    

### **Example: Incrementing a Shared Counter**

Here's an example demonstrating multiple threads incrementing a shared counter with proper synchronization:

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5
#define NUM_INCREMENTS 100000

int counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment_counter(void* arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            fprintf(stderr, "Error creating thread\n");
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d\n", counter);

    pthread_mutex_destroy(&counter_mutex);
    return EXIT_SUCCESS;
}

```

References

 https://cours.polymtl.ca/inf2610/documentation/notes/chap4.pdf
 Chatgpt4.o