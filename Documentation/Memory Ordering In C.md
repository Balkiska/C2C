# Memory Ordering in C

## Introduction

Memory ordering is a concept in multi-threaded programming. Ir refers to the sequence in which read and write operations to shared memory are executed and observed in a multi-threaded program. Modern processors and compilers may reorder instructions for performance optimization, potentially leading to inconsistencies in concurrent programs.

---

## **The Basics of Memory Ordering**

### **Reordering and Visibility**

- **Compiler Reordering**: Compilers may rearrange instructions to optimize performance.
- **Hardware Reordering**: CPUs may execute instructions out of order for efficiency.
- **Visibility**: Threads may not see the same order of memory operations performed by another thread due to these reorderings.

### **Race Conditions**

- A **data race** occurs when:
    - Two threads access the same memory location.
    - At least one of the accesses is a write.
    - No proper synchronization mechanism is used.

---

Memory orders in C specify the visibility and synchronization guarantees of atomic operations. They control how operations are ordered relative to others across threads.

*Atomic operations are basic actions in programming that happen all at once. They make sure data stays correct when multiple tasks use it at the same time, without needing extra locking tools.

## **Atomic Operations in C11**

The C11 standard provides a `<stdatomic.h>` library for atomic operations. These operations ensure that memory access is atomic and offer various levels of control over memory ordering.

C11 introduces atomic types, e.g.:

```c
#include <stdatomic.h>

atomic_int shared_var; // An atomic integer

```

### **Atomic Functions**

Examples of atomic functions:

- `atomic_store`: Store a value atomically.
- `atomic_load`: Load a value atomically.
- `atomic_fetch_add`: Perform atomic addition.
- `atomic_exchange`: Atomically swap values.

Each of these functions can take a **memory order** parameter, defining the visibility and ordering constraints.

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/127bca41-dc65-4c96-b713-5125ca8f314d/78f79d00-bbd7-4ef3-a607-f8330710305b/image.png)

### **Memory Orders: Overview**

| **Memory Order** | **Description** |
| --- | --- |
| **`memory_order_relaxed`** | No synchronization or ordering guarantees. Suitable for non-critical operations where performance is more important.  |
| **`memory_order_consume`** | Prevents reordering of dependent reads before the atomic operation. |
| **`memory_order_acquire`** | Blocks reordering of memory operations before the atomic operation. |
| **`memory_order_release`** | Blocks reordering of memory operations after the atomic operation. |
| **`memory_order_acq_rel`** | Combines `acquire` and `release` semantics. Prevents reordering before and after the atomic. |
| **`memory_order_seq_cst`** | Provides the strongest guarantees with a total ordering across all threads. |

### Comparison of Memory Orders

| **Memory Order** | Reordering Allowed	 | Synchronization |
| --- | --- | --- |
| **`memory_order_relaxed`** | Yes | No |
| **`memory_order_consume`** | Limited (dependencies)	 | Partial |
| **`memory_order_acquire`** | No reorder after	 | Read synchronization |
| **`memory_order_release`** | No reorder before	 | Write synchronization |
| **`memory_order_acq_rel`** | No reorder (bidirectional)	 | Synchronizes reads and writes |
| **`memory_order_seq_cst`** | No reorder globally	 | Total order synchronization |

### Exemple of usage

```c
atomic_store_explicit(&shared_var, 42, memory_order_relaxed);
```

### **Programming Example**

### **Programming Example**

The following example demonstrates how to use memory orders in C with atomic operations:

```c
#include <stdatomic.h>
#include <pthread.h>
#include <stdio.h>

atomic_int shared_data = 0;

void* producer(void* arg) {
    // Store data with release semantics
    atomic_store_explicit(&shared_data, 42, memory_order_release);
    return NULL;
}

void* consumer(void* arg) {
    // Load data with acquire semantics
    int value = atomic_load_explicit(&shared_data, memory_order_acquire);
    printf("Read value: %d\n", value);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
```

### **Explanation**:

- The producer writes to `data` and sets `ready` with `memory_order_release`, ensuring `data` is visible to consumers before `ready` is set.
- The consumer waits for `ready` with `memory_order_acquire`, ensuring all preceding writes are visible before proceeding.

# **How Memory Orders Work at the Processor Level**

## **Processor Memory Models**

Each architecture defines its own memory model:

- **x86 (Intel/AMD)**: Strong ordering; most memory operations are observed in the order written, but explicit memory barriers (`mfence`, `lfence`, `sfence`) are available for stricter control.
- **ARM/PowerPC**: Relaxed ordering; significant reordering is allowed. Memory barriers (e.g., `dmb`) are essential to enforce order.
- **RISC-V**: Configurable memory model with both relaxed and sequentially consistent options, using `fence` instructions for ordering.

---

## **Common Hardware Reorderings**

Processors may reorder:

- **Store-Store**: Two writes may be swapped.
- **Load-Load**: Two reads may be swapped.
- **Load-Store**: A read before a write may be swapped.
- **Store-Load**: A write before a read can be reordered, often requiring barriers to prevent.

---

## **Memory Barriers**

Memory barriers (fences) prevent reordering of specific types of operations:

- **Load Barrier**: Ensures reads before the barrier complete first.
- **Store Barrier**: Ensures writes before the barrier complete first.
- **Full Barrier**: Enforces both read and write order.

Example:

- ARM: `asm volatile ("dmb ish" ::: "memory");`
- x86: `asm volatile ("mfence" ::: "memory");`

---

## **Processor and Compiler Interactions**

Both compilers and CPUs reorder instructions:

- **Compiler Reordering**: Optimizes code during compilation.
- **CPU Reordering**: Occurs at runtime for hardware efficiency.

### Solution:

Use C11 atomic operations (`stdatomic.h`) with memory order constraints:

- `memory_order_acquire/release`: Prevent critical reordering.
- `memory_order_relaxed`: Allows reordering but ensures atomicity.

---

## **Example: Avoiding Issues**

```c
x = 1;
atomic_store_explicit(&y, 1, memory_order_release);
if (atomic_load_explicit(&y, memory_order_acquire)) {
    printf("%d\n", x); // Guaranteed to see x = 1
}
```

---

### **Practical Use Cases**

**Relaxed Order:**

- Ideal for performance-critical applications where threads do not rely on specific ordering.

**Acquire/Release Order:**

- Common in producer-consumer models where synchronization is critical.

**Sequential Consistency:**

- Necessary for applications requiring strict global ordering of operations.

---

### References

https://www.educative.io/answers/what-is-memoryorder-in-chttps://developer.arm.com/documentation/102336/0100/Memory-orderinghttps://developer.arm.com/documentation/102336/0100/Memory-barriers
chatgpt4.o
