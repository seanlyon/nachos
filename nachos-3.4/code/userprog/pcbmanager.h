#ifndef PCBM_H
#define PCBM_H

#include "bitmap.h"
#include "synch.h"
#include "pcb.h"

class pcb;
class Lock;

class pcbmanager{
public:
    pcbmanager(int MAXPROCESSES);
    ~pcbmanager();

    pcb* allocatePCB();
    int deallocatePCB(pcb* pcb);

    pcb* getPCB(int pid);

private:
    BitMap* pcbmap;
    pcb** pcbs;
    Lock* pcbLock;

};

#endif