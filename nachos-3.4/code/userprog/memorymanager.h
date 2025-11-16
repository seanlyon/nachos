#ifndef MEMORY_H
#define MEMORY_H

#include "synch.h"
#include "bitmap.h"
#include "machine.h"

class memorymanager{
public:
  memorymanager ();

  ~memorymanager();

  int allocatePage();

  void deallocatePage(int ppid);

  int getFreePageCount();
private:
  BitMap *Mem;
  Lock *lock;
};

#endif