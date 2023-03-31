# ya-interview-alloc

## Preamble

I have used already existing _FreeRTOS_ PosixGCC demo as the frame for this
task. So it relies on its source code. And, of course, there might be a little
bit of junk from the original demo left.  
To build this project, first of all, you have to download _FreeRTOS_ source code
because all of the paths in the Makefile are relative. Then move/clone this repo
into the `FreeRTOS/FreeRTOS/Demo` directory.

It builds with _make_ util. Pool size is configurable with the `POOL_SIZE` shell
variable. Block size is configurable with the `BLOCK_SIZE` shell variable.
Available target system bitnesses are either `32` or `64`, 32-bit target is used
by-default, to build this project for 64-bit system (so the addressing would be
appropriate), shell variable `ARCH64` should be defined.

```
git clone git@github.com/FreeRTOS/FreeRTOS.git
cd FreeRTOS/FreeRTOS/Demo
git clone git@github.com/bmigunov/ya-interview-alloc.git
cd ya-interview-alloc
make
```

To run tests execute file `build/ya_alloc_test`.

I definitely know that the test coverage could be better (e.g. with timers and
parallel tasks) but I did bare minimum because I have not so much of a spare
time. My apologies.

## Problem

Implement memory allocation library.  
It should be able to allocate and release a single block of memory with the
fixed size from the static pool.  
Both block and pool should be of a fixed size and the size should be
configurable during the build process.  
Library should be adapted to be used on the embedded systems with various
bitness.  
Also, memory allocator should work properly on **RTOS**s with preemptive
scheduling.  
There also has to be a set of unit tests with the basic coverage.

## Solution

First thing that is worth to mention that the only **OS** I have at my disposal
is _Linux_.

Pool allocation algorithm is already well-known.  
To keep track of memory blocks I will map their state onto appropriate bits in
the bit map (0 is free, 1 is allocated).

Pool size should rely on system bitness (since the memory addressing is limited
by bitness).  
Pool size should be dividable by block size.

To make memory allocator work properly on **RTOS**s with preemptive scheduling,
there has to be a call to stop the process scheduler before we will try to
allocate/deallocate a block of memory and continue its flow after
successful/unsuccessful memory allocation/deallocation. This is all done to
prevent potential allocation of the same block between different processes in
case of interruption.
