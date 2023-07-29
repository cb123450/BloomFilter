# Bloom Filter
**C++ implementation of a Bloom Filter.**

* A Bloom Filter is a space efficient data structure that can be used to test if an object is in a set. It can give false positives, but it will not give false negatives. It is used to reduce the amount of searches to memory for objects that are not there.
* Our implementation uses an array of bitsets to store the result of applying hash functions to the objects inserted into the Bloom Filter. 
* Each bitset has a readers-writer lock, which allows multiple threads to read from the same bitset at the same time while only allowing one one thread to write to it.
  * This improves the efficency of the Bloom Filter without sacrificing correctness.
  * We import the <pthread.h> header to provide this functionality.
* The information stored in our Bloom Filter can be persistently stored on disk through the use of the 'serialize()' function which opens a new file and writes a compact representation of our Bloom Filter into in.


**Producer-Consumer Model and Command Line Arguments**
* Our current implementation tests our Bloom Filter by having one producer thread insert data into the Bloom Filter and multiple consumer threads consuming the data. 
* When running the executable, argv[0] is the number of bits 'm' in the Bloom Filter, argv[1] is the number of hash functions 'k', argv[3] is the number of consumer threads, and argv[4] is the number of objects inserted into the Bloom Filter.

**Future Work**
* Implement a 'deserialize()' function that reads the contents of the Bloom Filter from disk and constructs an indentical Bloom Filter.
* Debug the occassional deadlocks between the threads.
