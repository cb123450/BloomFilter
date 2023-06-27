#include "CountingBloomFilter.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <tuple>
#include <thread>

template<typename T>
int run_t(queue<T>* q){
  while (!q.empty()){
    std::tie(action, item) = q.pop();
  }
}

int main(int argc, char* argv[]){
#undef LENGTH
#define LENGTH atoi(argv[1])
#undef NUM_HASH_FXNS
#define NUM_HASH_FXNS atoi(argv[2])

  hash<string> _hash;
  BloomFilter<string> c(LENGTH, NUM_HASH_FXNS, _hash);
  
  queue<string> q;
  q.push(make_tuple("W", "bat"));
  q.push(make_tuple("W", "car"));
  q.push(make_tuple("W", "cat"));
  q.push(make_tuple("W", "tree"));
  q.push(make_tuple("W", "bush"));
  q.push(make_tuple("W", "brick"));

  q.push(make_tuple("R", "bat"));
  q.push(make_tuple("R", "car"));
  q.push(make_tuple("R", "cat"));
  
  q.push(make_tuple("R", "boat"));
  q.push(make_tuple("R", "block"));
  q.push(make_tuple("R", "house"));

  std::thread t_1 (run_t, q);
  std::thread t_2 (run_t, q);
  std::thread t_3 (run_t, q);
  
  return 0;

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

  /* STRING TESTS
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
  */

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
