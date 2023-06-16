#include<functional>
#include<string>

using namespace std;

template <typename T>
class BloomFilter{
  
 private:
  int m;
  int k;
  int* arr;
  int* shifts;
  hash<T> hash_fxn;

 public:
  BloomFilter();
  BloomFilter(int m, int k, hash<T> hash_fxn);
  bool insert(T obj);
  bool query(T obj);

  //void createHashFxns();
  
};

template <typename T>
BloomFilter<T>::BloomFilter(){
  this->m = 10;
  this->k = 3;

  this->arr = new int[m];
  this->shifts = new int[k];
  this->hash_fxn = hash_fxn;
}

template <typename T>
BloomFilter<T>::BloomFilter(int m, int k, hash<T> hash_fxn){
  this->m = m;
  this->k = k;
  
  this->arr = new int[m];
  this->shifts = new int[k];
  this->hash_fxn = hash_fxn;
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
  std::hash<string> string_hash; 
  BloomFilter<string> b(10, 3, string_hash);
}
