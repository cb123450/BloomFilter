#include<functional>
#include<string>
#include <iostream>
#include <bitset>

using namespace std;

template <typename T>
class BloomFilter{
  
 public:
  int m;
  int k;
  bitset<1> bit_arr;
  hash<T> hash_fxn;

 public:
  BloomFilter();

  BloomFilter(int m, int k, hash<T> hash_fxn);

  virtual void insert(T obj);

  virtual bool query(T obj);
};
