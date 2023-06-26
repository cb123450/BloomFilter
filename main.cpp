#include "CountingBloomFilter.cpp"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
#undef LENGTH
#define LENGTH atoi(argv[1])
#undef NUM_HASH_FXNS
#define NUM_HASH_FXNS atoi(argv[2])

  hash<int> int_hash;
  BloomFilter<int> c(LENGTH, NUM_HASH_FXNS, int_hash);

  c.insert(300);
  c.insert(551);

  cout << c.query(330);
  cout <<  "\n"; //1
  cout << c.query(551);
  cout << "\n"; //1
  cout << c.query(7);
  cout << "\n"; //0
  cout << "Total number of bits allocated: ";
  cout << c.total_bits_allocated();
  cout << "\n";
  cout << "Bits user asked for: ";
  cout << c.bits_user_asked_for();
  cout << "\n";

  return 0;

  /* CountingBloomFilter Tests
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
  
}
