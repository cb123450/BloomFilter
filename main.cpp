#include "CountingBloomFilter.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <tuple>
#include <pthread.h>
#include <cstring>
#include <assert.h>
#include <vector>
#define TYPE string
#define MAX 1000


/*
 * Buffer implementation for producer-consumer tests
 */
tuple<string, TYPE> buff[MAX];
int put_pointer = 0;
int get_pointer = 0;
int count = 0;

void put(tuple<string, TYPE> val) {
  buff[put_pointer] = val;
  put_pointer = (put_pointer + 1) % MAX;
  count += 1;
}

tuple<string, TYPE> get(){
  tuple<string, TYPE> ret = buff[get_pointer];
  get_pointer = (get_pointer + 1) % MAX;
  count -= 1;
  return ret;
}

/*
 * struc to hold data about our bloomfilter to pass it to 'run_t'
 */
template<typename T>
struct thread_data{
  BloomFilter<T>* bf;
  int n_t;
};



pthread_cond_t empty, fill;
pthread_mutex_t mutex;

/*
 * producer threads
 */
void* producer(void* param){
  thread_data<TYPE>* td = (thread_data<TYPE> *) param;
  int num_tasks = td->n_t;

  srand(time(0));
  for (int i = 0; i < num_tasks; i++) {
    pthread_mutex_lock(&mutex);
    while (count == MAX){
      pthread_cond_wait(&empty, &mutex);
    }
    string RW = "W";
    if (rand() % 2) {
      RW = "R";
    }
    auto tuple = make_tuple(RW, (rand() % 5000) + 1);
    put(tuple);
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);
  }
}

/*
 * consumer threads
 */
string action;
TYPE item;
void* consumer(void* param){
  
  thread_data<TYPE>* td = (thread_data<TYPE> *) param;
  int num_tasks = td->n_t;

  for (i = 0; i < num_tasks; i++){
    pthread_mutex_lock(&mutex);
    
    while(count == 0){
      pthread_cond_wait(&fill, &mutex);

      std::tie (action, item) = get();

      //Insert item into BloomFilter
      if (action.compare("R") == 0) {
	bf->query(item);
      } else if (action.compare("W") == 0) {
	bf->insert(item);
      }

      pthread_cond_signal(&empty);
      pthread_mutex_unlock(&mutex);
    }
  }
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

  int rc = pthread_mutex_init(&mutex, PTHREAD_PROCESS_PRIVATE);
  assert(rc == 0);
  hash<TYPE> _hash;
  BloomFilter<TYPE> c(LENGTH, NUM_HASH_FXNS, _hash);
  queue<tuple<string, TYPE>> q;  
  thread_data<TYPE> td = {&c, NUM_TASKS};

  
  pthread_t * consumers = new pthread_t[NUM_CONSUMERS];
  for (int j = 0; j < NUM_CONSUMERS; j++) {
    pthread_create(&pthreads[j], NULL, consumer, &td);
  }

  pthread_t* producer;
  pthread_create(&producer, NULL, producer, &td);
  
  for (int k = 0; k < NUM_CONSUMERS; k++) {
    pthread_join(pthreads[k], NULL);
  }

  pthread_join(producer, NULL);
  
  return 0;
  
}
