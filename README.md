# BloomFilter
C++ implementation of a Bloom Filter. 

* A Bloom Filter is a space efficient data structure that can be used to test if an object is in a set. It can give false positives, but it will not give false negatives. It is used to decrease the amount of time spent reading from memory.
* Our implementation uses an array of bitsets to store the result of applying hash functions to the objects inserted into the Bloom Filter. 
* Each bitset has a readers-writer lock, which allows multiple threads to read from the same bitset at the same time while only allowing one one thread to write to it.
  * This improves the efficency of the Bloom Filter without sacrificing correctness.    
