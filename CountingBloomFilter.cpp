#include "CountingBloomFilter.h"

template <typename T>
void CountingBloomFilter<T>::insert(T obj){
  for (int shift = 0; shift < this.k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%m;
    bit_arr[res];
  }
}

template <typename T>
bool CountingBloomFilter<T>::query(T obj, int count){

  for (int shift = 0; shift < this.k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%m;
    if (bit_arr[res] < count){
      return false;
    }
    return true; 
  }
}

template <typename T>
void CountingBloomFilter<T>::delete(T obj){
  for (int shift = 0; shift < k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%m;
    if (bit_arr[res] == 0){
      return;
    }
  }
  for (int shift = 0; shift < k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%m;
    bit_arr[res]--;
  }
}

int main(){
  hash<int> string_hash;
  CountingBloomFilter<int> c(10, 3, string_hash);

  c.insert(3);
  c.insert(3);
  c.insert(3);
  c.insert(7);

  cout << c.query(3,2); // 1
  cout << "\n";
  cout << c.query(3,3); // 1
  cout << "\n";
  cout << c.query(3,4); // 0
  cout << "\n";
  cout << c.query(7,1); // 1
  cout << "\n";
  cout << c.query(5,1); // 0
  cout << "\n";

  return 0;
}
