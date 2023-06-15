#include<functional>

using namespace std;

template <typename T>
class BloomFilter{
  
 private:
  int arr[];
  int m;
  int k;
  hash<T*> hash_fxns;

 public:
  BloomFilter(int m, int k);

  bool insert(T obj);
  bool query(T obj);

  void createHashFxns();
  
}
