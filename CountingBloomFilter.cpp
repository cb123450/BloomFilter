#include "CountingBloomFilter.h"

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
    return true; 
  }
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

int main(){
  hash<int> int_hash;
  CountingBloomFilter<int> c(10, 3, int_hash);

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

  c.deleteObj(3);
  cout << c.query(3,3); // 0
  cout << "\n";

  return 0;
}

