#include "CountingBloomFilter.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <tuple>
#include <pthread.h>
#include <cstring>
#include <assert.h>
#include <vector>
#define TYPE int

/*
 * Buffer implementation for producer-consumer tests
 */
int MAX = 1000;
tuple<string, TYPE>* buff;
int put_pointer = 0;
int get_pointer = 0;
int count = 0;


void put(tuple<string, TYPE> val) {
  if (count < MAX-1){
    buff[put_pointer] = val;
    put_pointer = (put_pointer + 1) % MAX;
    count += 1;
  }
}

tuple<string, TYPE> get(){
  if (count >= 1){
    tuple<string, TYPE> ret = buff[get_pointer];
    get_pointer = (get_pointer + 1) % MAX;
    count -= 1;
    return ret;
  }
}

/*
 * struc to hold data about our bloomfilter to pass it to 'run_t'
 */
template<typename T>
struct thread_data{
  BloomFilter<T>* bf;
  int n_t;
};


pthread_cond_t empty;
pthread_cond_t filled;
pthread_mutex_t mutex;

bool producer_done = false;

/*
 * producer threads
 */
void* producer(void* param){
  thread_data<TYPE>* td = (thread_data<TYPE> *) param;
  int num_tasks = td->n_t;

  srand(time(0));
  for (int i = 0; i < num_tasks; i++) {
    //    cout << pthread_self() << ": About to lock producer" << "\n";
    int rc = pthread_mutex_lock(&mutex);
    assert(rc == 0);
    cout << pthread_self() << ": Locked producer" << "\n";    
    while (count == MAX-1){
      cout << pthread_self() << " :producer asleep" << "\n";
      producer_done = true;
      pthread_cond_wait(&empty, &mutex);
    }
    string RW = "W";
    if (rand() % 2) {
      RW = "R";
    }
    auto tuple = make_tuple(RW, (rand() % 5000) + 1);
    put(tuple);
    rc = pthread_cond_signal(&filled);
    assert (rc == 0);
    cout << pthread_self() << " :count " << count << " :MAX " << MAX  << " :producer " << "\n";

    cout << pthread_self() << ": About to unlock producer" << "\n";
    rc = pthread_mutex_unlock(&mutex);
    assert(rc == 0);
    //    cout << pthread_self() << ": Unlocked producer" << "\n";
  }
  return 0;
}

/*
 * consumer threads
 */
string action;
TYPE item;
void* consumer(void* param){
  
  thread_data<TYPE>* td = (thread_data<TYPE> *) param;
  int num_tasks = td->n_t;

  for (int i = 0; i < num_tasks; i++){
    //    cout << pthread_self() << ": About to lock consumer" << "\n";
    int rc = pthread_mutex_lock(&mutex);
    assert(rc == 0);
    cout << pthread_self() << ": Locked consumer" << "\n";
    
    while (count == 0 && !producer_done){
      cout << pthread_self() << " :consumer asleep" << "\n";
      pthread_cond_wait(&filled, &mutex);
    }
    if (count > 0){
      std::tie (action, item) = get();

      //Insert item into BloomFilter
      if (action.compare("R") == 0) {
	td->bf->query(item);
      } else if (action.compare("W") == 0) {
	td->bf->insert(item);
      }
    }
    rc = pthread_cond_signal(&empty);
    assert(rc == 0);
    cout << pthread_self() << " " << count << " :consumer" << "\n";
    
    cout << pthread_self() << ": About to unlock consumer" << "\n";
    rc = pthread_mutex_unlock(&mutex);
    assert(rc == 0);
    //    cout << pthread_self() << ": Unlocked consumer" << "\n";

  }
  return 0;
}

/*
 * K is the type of 'item'
 * This is the type of the object stored in the BloomFilter and the item stored in the queue
 
template<typename K>
void *run_t(void *param){

  string action;
  int item;
  thread_data<K>* td = (thread_data<K> *) param;
  queue<tuple<string, K>>* q = td->q;
  BloomFilter<K>* bf = td->bf;

  bool done = false;

  while (!done){
    int rc = pthread_spin_lock(&spinlock);
    assert(rc == 0);
    
    if (!q->empty()){
      std::tie (action, item) = q->front();
      q->pop();
      rc = pthread_spin_unlock(&spinlock);
      assert(rc == 0);
    
      //cout << pthread_self();
      //cout << ": ";
      if (action.compare("R") == 0) {
	//cout << action;
	//cout << " ";
	bf->query(item);
      } else if (action.compare("W") == 0) {
	//cout << action;
	//cout << " ";
	//cout << item;
	//cout << " was inserted";
	bf->insert(item);
      }
    }
    else{
      done = true;
    }
  }
  //no way to unlock twice because threads do not share local variables
  int rc = pthread_spin_unlock(&spinlock); 
  assert(rc == 0);
  return NULL;
}

*/
int main(int argc, char* argv[]){
#undef LENGTH
#define LENGTH atoi(argv[1])
#undef NUM_HASH_FXNS
#define NUM_HASH_FXNS atoi(argv[2])
#define NUM_CONSUMERS atoi(argv[3])
#define NUM_TASKS atoi(argv[4])

  MAX = NUM_TASKS;
  buff = (tuple<string, TYPE>*) malloc(MAX*sizeof(tuple<string, TYPE>));
  
  int rc = pthread_mutex_init(&mutex, NULL);
  assert(rc == 0);
  hash<TYPE> _hash;
  BloomFilter<TYPE> c(LENGTH, NUM_HASH_FXNS, _hash);
  thread_data<TYPE> td = {&c, NUM_TASKS};

  pthread_t prod;
  pthread_create(&prod, NULL, producer, &td);
  
  pthread_t * consumers = new pthread_t[NUM_CONSUMERS];
  for (int j = 0; j < NUM_CONSUMERS; j++) {
    pthread_create(&consumers[j], NULL, consumer, &td);
  }

  for (int k = 0; k < NUM_CONSUMERS; k++) {
    pthread_join(consumers[k], NULL);
  }
  pthread_join(prod, NULL);
    
  cout << "Done" << "\n";
  c.serialize();
  
  return 0;
  
}
