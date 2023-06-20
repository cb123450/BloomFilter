#include "BloomFilter.h"

template <typename T>
BloomFilter<T>::BloomFilter(){
  this->m = 10;
  this->k = 3;

  this->bit_arr = new int[m];
  this->hash_fxn = hash_fxn;
}

template <typename T>
BloomFilter<T>::BloomFilter(int m, int k, hash<T> hash_fxn){
  this->m = m;
  this->k = k;
  
  this->bit_arr = new int[m];
  this->hash_fxn = hash_fxn;

  for (int i = 0; i < m; i++){
    bit_arr[i] = 0;
  }
}

template <typename T>
void BloomFilter<T>::insert(T obj){

  for (int shift = 0; shift < k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%m;
    bit_arr[res] = 1;
  }
}

template <typename T>
bool BloomFilter<T>::query(T obj){
  for (int shift = 0; shift < k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%m;
    if (bit_arr[res] == 0){
      return false;
    }
  }
  return true;
}
