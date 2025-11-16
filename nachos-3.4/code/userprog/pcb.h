#ifndef PCB_H
#define PCB_H

#include "list.h"
#include "pcbmanager.h"

class Thread;
class pcbmanager;
extern pcbmanager* pcbm;

class pcb{
  public:
  pcb(int id);
  ~pcb();

  int pid;
  pcb* parent;
  Thread* thread;
  int exitStatus;

  void addChild(pcb* pcb);
  //int removeChild(pcb* pcb);
  bool hasExited(int arg);
  void exitCleanup(pcbmanager* pcbm);

private:
  List* children;
};

#endif