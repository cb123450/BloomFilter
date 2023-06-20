#include "CountingBloomFilter.cpp"

int main(){
/*
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
*/
  hash<int> string_hash; 
  BloomFilter<int> b(10, 3, string_hash);

  b.insert(4);
  cout << b.query(5);
  cout << "\n";
  cout << b.query(4);

  return 0;

}
