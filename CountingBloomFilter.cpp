#include "CountingBloomFilter.h"

template <typename T>
CountingBloomFilter<T>::CountingBloomFilter() {
  typename BloomFilter<T>::BloomFilter();
  BloomFilter<T>::bit_arr = new int[BloomFilter<T>::m];
}

template <typename T>
CountingBloomFilter<T>::CountingBloomFilter(int m, int k, hash<T> hash_fxn) {
  typename BloomFilter<T>::BloomFilter(m, k, hash_fxn);
  BloomFilter<T>::bit_arr = new int[BloomFilter<T>::m];
}


template <typename T>
void CountingBloomFilter<T>::insert(T obj){
  for (int shift = 0; shift < BloomFilter<T>::k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%BloomFilter<T>::m;
    BloomFilter<T>::bit_arr[res]++;
  }
}

template <typename T>
bool CountingBloomFilter<T>::query(T obj, int count){

  for (int shift = 0; shift < BloomFilter<T>::k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%BloomFilter<T>::m;
    if (BloomFilter<T>::bit_arr[res] < count){
      return false;
    }
  }
  return true;   
}

template <typename T>
void CountingBloomFilter<T>::deleteObj(T obj){
  for (int shift = 0; shift < BloomFilter<T>::k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%BloomFilter<T>::m;
    if (BloomFilter<T>::bit_arr[res] == 0){
      return;
    }
  }
  for (int shift = 0; shift < BloomFilter<T>::k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%BloomFilter<T>::m;
    BloomFilter<T>::bit_arr[res]--;
  }
}

