#include "BloomFilter.h"
#include "assert.h"
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

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

    //    cout << pthread_self() << ": About to WriteLock " << outer_index << " \n";;
    int rc = pthread_rwlock_wrlock(&lock_arr[outer_index]);
    assert(rc == 0);
    //cout << "Success WriteLock " << outer_index << " \n";
    
    bit_arr[outer_index][inner_index] = 1;

    //cout << pthread_self() << ": W \n-"<< "About to WriteUnlock " << outer_index << " \n";
    rc = pthread_rwlock_unlock(&lock_arr[outer_index]);
    assert(rc == 0);
    //cout << "Success WriteUnlock " << outer_index << " \n";
  }
}

template <typename T>
bool BloomFilter<T>::query(T obj){
  for (int shift = 0; shift < k; shift++){
    unsigned int hash = this->hash_fxn(obj) >> shift;
    unsigned int outer_index = (hash%(1024 * bit_arr_size))/1024;
    unsigned int inner_index = hash%1024;

    //cout << pthread_self() << ": About to ReadLock " << outer_index << "\n";
    int rc = pthread_rwlock_rdlock(&lock_arr[outer_index]);
    assert(rc == 0);
    //cout << "Success ReadLock " << outer_index << " \n";
    
    if (bit_arr[outer_index][inner_index] == 0){
      //cout << pthread_self() << ": R \n" << "About to ReadUnlock " << outer_index << " \n";
      rc = pthread_rwlock_unlock(&lock_arr[outer_index]);
      assert(rc == 0);
      //cout << "Success ReadUnlock " << outer_index << " \n";
      return false;
    }
    //cout << pthread_self() << ": R \n" << "About to ReadUnlock " << outer_index << " \n";
    rc = pthread_rwlock_unlock(&lock_arr[outer_index]);
    assert(rc == 0);
    //cout << "Success ReadUnlock " << outer_index << " \n";
  }
  return true;
}

void static wbuf_write_uint32_t(struct wbuf* wb, uint32_t data){
  *((std::uint32_t *) wb->buf +  wb->offset) = data;
  wb->offset = wb->offset + 4;
}

void static wbuf_write_charStar(struct wbuf* wb, char* data, uint32_t numToWrite){
  assert(data[numToWrite] == '\0');
  for(std::uint32_t i = 0; i < numToWrite; i++) {
    *((char *) wb->buf + wb->offset) = data[i];
    wb->offset = wb->offset + 1;
  }
}

void static wbuf_write_bitSet(struct wbuf* wb, bitset<BITSET_SIZE> data){
  bitset<BITSET_SIZE> mask (0xFF);
  for(std::uint32_t i = 0; i < BITSET_SIZE/8; i++) {
    bitset<1024> newByte = data & mask;
    std::uint8_t w = 0b00000000;
    
    for (std::uint32_t k = 0; k < 8; k++){
      if (newByte[k] == 1){
	w &= (0b1 << k); 
      }
    }
      
    *((std::uint8_t *) wb->buf + wb->offset) = w;
    wb->offset = wb->offset + 1;
    data >> 8;
  }
}

/*
 * Serialize the BloomFilter (put it onto disk) in the following format
 * [Version #][user_provided bits (m, 1st arg)][# of hash fxns (k, 2nd arg)][# of locks]
 * [total bits stored][bits per bitset][number of bitsets][bitset 1]...[bitset n]
 *
 * [6 bytes][4 bytes][4 bytes][4 bytes]
 * [4 bytes][4 bytes][4 bytes][BITSET_SIZE/8 bytes]...[BITSET_SIZE/8 bytes]
 */
template <typename T>
void BloomFilter<T>::serialize(){

  char* buffer = {};
  std::uint32_t off = 0;
  std::uint32_t s = 32;
  s += (BITSET_SIZE/8)*this->bit_arr_size;
  
  wbuf wb = {&buffer, off, s};

  wbuf_write_charStar(&wb, "v1bf", 4);
  wbuf_write_uint32_t(&wb, m);
  wbuf_write_uint32_t(&wb, k);
  wbuf_write_uint32_t(&wb, bit_arr_size);
  wbuf_write_uint32_t(&wb, bit_arr_size * BITSET_SIZE);
  wbuf_write_uint32_t(&wb, BITSET_SIZE);
  wbuf_write_uint32_t(&wb, bit_arr_size);
  for(int i = 0; i < bit_arr_size; i++) {
    wbuf_write_bitSet(&wb, bit_arr[i]);
  }
  
  int fd = open("file", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
  write(fd, wb.buf, wb.size);
  
  cout << "test" << "\n";
}

template <typename T>
int BloomFilter<T>::total_bits_allocated(){
  return this->bit_arr_size*BITSET_SIZE;
}

template <typename T>
int BloomFilter<T>::bits_user_asked_for(){
  return this->m;
}
