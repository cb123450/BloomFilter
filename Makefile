# the compiler: gcc for C program, define as g++ for C++
  CC = g++
 
  # compiler flags:
  #  -g     - this flag adds debugging information to the executable file
  #  -Wall  - this flag is used to turn on most compiler warnings
  CFLAGS  = -g -Wall -pthread
 
  # The build target 
  TARGET = BloomFilter
 
  all: $(TARGET)
 
  $(TARGET): main.cpp $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp main.cpp -o exe
 
  clean:
	$(RM) exe
