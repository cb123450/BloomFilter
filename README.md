# BloomFilter
C++ implementation of a Bloom Filter. It uses bitsets to store the result of applying hash functions to the objects inserted into the Bloom Filter. Each bitset has a readers-writer lock, which allows multiple threads to read from the same bitset at the same time while only allowing one one thread to write to it. 
