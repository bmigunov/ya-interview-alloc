# ya-interview-alloc

## 1. Problem

Implement memory allocation library.  
It should be able to allocate and release a single block of memory with the
fixed size from the static pool.  
Both block and pool should be of a fixed size and the size should be
configurable during the build process.  
Library should be adapted to be used on the embedded systems with various
bitness.  
Also, memory allocator should work properly on RTOSs with preemptive
scheduling.  
There also has to be a set of unit tests with the basic coverage.

## 2. Solution

First thing that is worth to mention that the only OS I have at my disposal is
Linux. So the userspace library for Linux will only show the allocation algo
itself without any RTOS-specific features (since we do not have direct memory
access and the process scheduler is not switching context after fixed time
period).

Pool allocation algorithm is already well-known.  
To keep track of memory blocks I will map them onto array and use a bit-to-block
state representation.

Pool size should rely on system bitness (since the memory addressing is limited
by bitness).  
Pool size should be dividable by block size.

To make memory allocator work properly on RTOSs with preemptive scheduling,
there has to be a call to stop the process scheduler before we will try to
allocate/deallocate a block of memory and continue its flow after
successful/unsuccessful memory allocation/deallocation. This is all done to
prevent potential allocation of the same block between different processes.
