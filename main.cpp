#include "CountingBloomFilter.cpp"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
#undef LENGTH
#define LENGTH atoi(argv[1])
#undef NUM_HASH_FXNS
#define NUM_HASH_FXNS atoi(argv[2])

  /* INTS work
  hash<int> _hash;
  BloomFilter<int> c(LENGTH, NUM_HASH_FXNS, _hash);

  c.insert(5);
  c.insert(400);

  cout << _hash(5);
  cout << "\n";
  cout << c.query(5); //1
  cout << "\n";		  
  cout << c.query(400); //1
  cout << "\n";		  
  cout << c.query(10); //0
  cout << "\n";		  
  */
  
  hash<string> _hash;
  BloomFilter<string> c(LENGTH, NUM_HASH_FXNS, _hash);

  c.insert("bat");
  c.insert("car");

  cout << _hash("bat");
  cout << "\n";
  cout << c.query("bat"); //1
  cout << "\n";
  cout << c.query("car"); //1
  cout << "\n";
  cout << c.query("tree"); //0
  cout << "\n";
  
  cout << "Total number of bits allocated: ";
  cout <<  c.total_bits_allocated();
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
