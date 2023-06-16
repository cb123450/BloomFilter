#include<functional>
#include<string>
#include <iostream>

using namespace std;

template <typename T>
class BloomFilter{
  
 private:
  int m;
  int k;
  int* bit_arr;
  hash<T> hash_fxn;

 public:
  BloomFilter();
  BloomFilter(int m, int k, hash<T> hash_fxn);
  void insert(T obj);
  bool query(T obj);

  //void createHashFxns();
  
};

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
    bit_arr[res]= 1;
  }
}

template <typename T>
bool BloomFilter<T>::query(T obj){
  for (int shift = 0; shift < k; shift++){
    int res = (this->hash_fxn(obj) >> shift)%m;
    if (bit_arr[res] == 0){
      return false;
    }
    return true; 
  }
}

int main(){
   
 
  hash<int> string_hash; 
  BloomFilter<int> b(10, 3, string_hash);

  b.insert(4);
  cout << b.query(5);
  cout << "\n";
  
  return 0;
}
