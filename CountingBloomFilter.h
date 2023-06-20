#include "BloomFilter.cpp"
template <typename T>
class CountingBloomFilter : private BloomFilter<T>{
public:
  CountingBloomFilter();
  CountingBloomFilter(int m, int k, hash<T> hash_fxn);
  void insert(T obj);
  bool query(T obj, int count);
  void deleteObj(T obj);
};
