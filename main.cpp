#include "CountingBloomFilter.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <tuple>
#include <pthread.h>

template<typename T>
struct thread_data{
  BloomFilter<T>* bf;
  queue<tuple<string, T>>* q;
};

/*
 * K is the type of 'item'
 * This is the type of the object stored in the BloomFilter and the item stored in the queue
 */
template<typename K>
void *run_t(void *param){

  string action;
  string item;
  thread_data<K>* td = (thread_data<K> *) param;
  queue<tuple<string, K>>* q = td->q;

  while (!q->empty()){
    std::tie (action, item) = q->front();
    q->pop();
  }
  return NULL;
}

int main(int argc, char* argv[]){
#undef LENGTH
#define LENGTH atoi(argv[1])
#undef NUM_HASH_FXNS
#define NUM_HASH_FXNS atoi(argv[2])

  hash<string> _hash;
  BloomFilter<string> c(LENGTH, NUM_HASH_FXNS, _hash);
  queue<tuple<string, string>> q;  
  thread_data<string> td = {&c, &q};
  
  auto a = make_tuple("W", "bat");
  auto b = make_tuple("W", "car");
  auto m = make_tuple("W", "cat");
  auto d = make_tuple("W", "tree");
  auto e = make_tuple("W", "bush");
  auto f = make_tuple("W", "brick");
  auto g = make_tuple("R", "bat");
  auto h = make_tuple("R", "car");
  auto i = make_tuple("R", "cat");
  auto j = make_tuple("R", "boat");
  auto k = make_tuple("R", "block");
  auto l = make_tuple("R", "house");

  q.push(a);
  q.push(b);
  q.push(m);
  q.push(d);
  q.push(e);
  q.push(f);
  q.push(g);
  q.push(h);
  q.push(i);
  q.push(j);
  q.push(k);
  q.push(l);


  
  pthread_t pt1;
  pthread_create(&pt1, NULL, run_t<string>, &td);
  pthread_join(pt1, NULL);
  
//  thread t_2 (run_t, q);
//  thread t_3 (run_t, q);
  
  

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
