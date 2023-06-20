#include "BloomFilter.h"
#include <bitset>

template <typename T>
BloomFilter<T>::BloomFilter(){
  this->m = 10;
  this->k = 3;
  bitset<10> b;
  /*bitset<num> is a template and num needs to be known at compile time*/
  this->bit_arr = b;
  this->hash_fxn = hash_fxn;
}

template <typename T>
BloomFilter<T>::BloomFilter(int m, int k, hash<T> hash_fxn){
  this->m = m;
  this->k = k;
  bitset<10> b;
  this->bit_arr = b;
  this->hash_fxn = hash_fxn;

}

template <typename T>
void BloomFilter<T>::insert(T obj){

  for (int shift = 0; shift < k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%m;
    bit_arr[res] = true;
  }
}

template <typename T>
bool BloomFilter<T>::query(T obj){
  for (int shift = 0; shift < k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%m;
    if (bit_arr[res] == false){
      return false;
    }
  }
  return true;
}
