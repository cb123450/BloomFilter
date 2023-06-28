#include "BloomFilter.h"
#include "assert.h"


template <typename T>
BloomFilter<T>::BloomFilter(){
  hash<T> hash_fxn;
  BloomFilter<T>::BloomFilter(5000, 10, hash_fxn);
}

template <typename T>
BloomFilter<T>::BloomFilter(int m, int k, hash<T> hash_fxn){
  this->m = m;
  this->k = k;
  this->hash_fxn = hash_fxn;

  this->bit_arr_size = ceil((double)m/BITSET_SIZE);  
  this->bit_arr = new bitset<BITSET_SIZE>[this->bit_arr_size];
  this-> lock_arr = new pthread_rwlock_t[this->bit_arr_size];

  for (int i = 0; i < bit_arr_size; i++){
    for (int k = 0; k < BITSET_SIZE; k++){
      bit_arr[i][k] = 0;
    }
    int rc = pthread_rwlock_init(&lock_arr[i], NULL);
    assert (rc == 0);
  }
}

template <typename T>
void BloomFilter<T>::insert(T obj){
  for (int shift = 0; shift < k; shift++){
    unsigned int hash = this->hash_fxn(obj) >> shift;
    unsigned int outer_index = (hash%(1024 * bit_arr_size))/1024;
    unsigned int inner_index = hash%1024;

    cout << pthread_self() << ": About to WriteLock " << outer_index << " \n";;
    int rc = pthread_rwlock_wrlock(&lock_arr[outer_index]);
    assert(rc == 0);
    cout << "Success WriteLock " << outer_index << " \n";
    
    bit_arr[outer_index][inner_index] = 1;

    cout << pthread_self() << ": W \n-"<< "About to WriteUnlock " << outer_index << " \n";
    rc = pthread_rwlock_unlock(&lock_arr[outer_index]);
    assert(rc == 0);
    cout << "Success WriteUnlock " << outer_index << " \n";
  }
}

template <typename T>
bool BloomFilter<T>::query(T obj){
  for (int shift = 0; shift < k; shift++){
    unsigned int hash = this->hash_fxn(obj) >> shift;
    unsigned int outer_index = (hash%(1024 * bit_arr_size))/1024;
    unsigned int inner_index = hash%1024;

    cout << pthread_self() << ": About to ReadLock " << outer_index << "\n";
    int rc = pthread_rwlock_rdlock(&lock_arr[outer_index]);
    assert(rc == 0);
    cout << "Success ReadLock " << outer_index << " \n";
    
    if (bit_arr[outer_index][inner_index] == 0){
      cout << pthread_self() << ": R \n" << "About to ReadUnlock " << outer_index << " \n";
      rc = pthread_rwlock_unlock(&lock_arr[outer_index]);
      assert(rc == 0);
      cout << "Success ReadUnlock " << outer_index << " \n";
      return false;
    }
    cout << pthread_self() << ": R \n" << "About to ReadUnlock " << outer_index << " \n";
    rc = pthread_rwlock_unlock(&lock_arr[outer_index]);
    assert(rc == 0);
    cout << "Success ReadUnlock " << outer_index << " \n";
  }
  return true;
}

template <typename T>
int BloomFilter<T>::total_bits_allocated(){
  return this->bit_arr_size*BITSET_SIZE;
}

template <typename T>
int BloomFilter<T>::bits_user_asked_for(){
  return this->m;
}
