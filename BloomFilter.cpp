#include "BloomFilter.h"

BloomFilter<T>(int m, int k){
  this.m = m;
  this.arr = new int[m];
  this.k = k;

}

bool insert(T obj){
  return true;
}

bool query(T obj){
  return true;
}

void createHashFxns(){
  hash<T*> orig_hash;
  hash_fxns[0] = orig_hash;  
}

int main(){
  //BloomFilter b = newBloomFilter(10, 3);
}
