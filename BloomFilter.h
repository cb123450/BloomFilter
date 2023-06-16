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
  BloomFilter(int m, int k, hash<T> hash_fxn);

  bool insert(T obj);
  bool query(T obj);

  //void createHashFxns();
  
};


