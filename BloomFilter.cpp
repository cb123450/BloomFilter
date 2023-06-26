#include "BloomFilter.h"

template <typename T>
BloomFilter<T>::BloomFilter(){
  hash<T> hash_fxn;
  BloomFilter<T>::BloomFilter(5000, 10, hash_fxn);
}

template <typename T>
BloomFilter<T>::BloomFilter(int m, int k, hash<T> hash_fxn){
  this->m = m;
  this->k = k;
  this->bit_arr_size = ceil(m/BITSET_SIZE);  
  this->bit_arr = new bitset<BITSET_SIZE>[this->bit_arr_size];
  
  this->hash_fxn = hash_fxn;

  for (int i = 0; i < bit_arr_size; i++){
    for (int k = 0; k < BITSET_SIZE; k++){
      bit_arr[i][k] = 0;
    }
  }
}

template <typename T>
void BloomFilter<T>::insert(T obj){

  for (int shift = 0; shift < k; shift++){
    int hash = this->hash_fxn(obj) >> shift;
    int outer_index = hash/1024;
    int inner_index = hash%1024;
    bit_arr[outer_index][inner_index] = 1;
  }
}

template <typename T>
bool BloomFilter<T>::query(T obj){
  for (int shift = 0; shift < k; shift++){
    int hash = this->hash_fxn(obj) >> shift;
    int outer_index = hash/1024;
    int inner_index = hash%1024;

    if (bit_arr[outer_index][inner_index] == 0){
      return false;
    }
  }
  return true;
}

template <typename T>
int BloomFilter<T>::total_bits_allocated(){
  return this->bit_arr_size*BITSET_SIZE;
}

template <typename T>
int BloomFilter<T>::bits_user_asked_for(){
  return this->m;
}
