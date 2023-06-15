#include "BloomFilter.h"

template <typename T>
BloomFilter<T>::BloomFilter(int m, int k){
  this.m = m;
  this.k = k;
  
  this.arr = new int[m];
  this.shifts = new int[k];

}

template <typename T>
bool BloomFilter<T>::insert(T obj){
  return true;
}

template <typename T>
bool BloomFilter<T>::query(T obj){
  return true;
}

int main(){
  BloomFilter<string> b = new BloomFilter<string>(10, 3);
}
