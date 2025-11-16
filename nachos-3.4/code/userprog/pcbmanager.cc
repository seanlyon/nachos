#include "pcbmanager.h"

/*Similar to memorymanager pcbmanager is responsible for allocating process control blocks (PCBs) to processes.
works through both a bitmap an array of pcbs, bitmap keeps track of amount of process space avaliable while the array
keeps track of the individula pcbs. Both are set to the size of maxprocesses.

Allocation of pcbs takes place here, the index of an avaliable pcb in the bitmap becomes the process id of the pcb and the index of the pcb in the pcbs array.

ONLY ONE PCBMANAGER IS NEEDED FOR THE PROJECT AND IS ALREADY DECLARED AND INTITIALIZED IN system.h/cc 

pcbs get allocated in address space construction

*/
//Constructor. Takes in the max amount of processes allowed on OS
//and initializes a bitmap and array with that amount
pcbmanager::pcbmanager(int MAXPROCESSES){
    pcbmap = new BitMap(MAXPROCESSES);
    pcbs = new pcb*[MAXPROCESSES];
    pcbLock = new Lock("pcb lock");

    for (int i = 0; i < MAXPROCESSES; i++){
        pcbs[i] = NULL;
    }
}

//Deconstructor
pcbmanager::~pcbmanager(){
    delete pcbmap;
    delete pcbs;
}

//Allocates an avaliable process control block.
//bitmap finds a free PCB and a new PCB is initialized
//with the integer the bitmap returns
//pcb is then added to array of pcbs and is returned
pcb* pcbmanager::allocatePCB(){
    
    //pcbLock -> Acquire();

    int freePID = pcbmap -> Find();

    //pcbLock -> Release();
    
    ASSERT(freePID != 1);

    pcb* newPCB = new pcb(freePID);

    pcbs[freePID] = newPCB;

    return newPCB;
}

//Deallocates a pcb making it avaliable for use
//First checks if pcb is valid (aka is in both pcbs and pcbmap)
//then clears the pcb (removes processes data from the pcb)
// 1 is returned if successfull otherwise, -1
int pcbmanager::deallocatePCB(pcb* pcb){
    if(pcbs[pcb->pid] == NULL){
        return -1;
    }

    //pcbLock -> Acquire();

    if (pcbmap -> Test(pcb->pid)){
        pcbmap -> Clear(pcb->pid);

    }else{
        //pcbLock -> Release();
        return -1;

    }

    //pcbLock -> Release();

    delete pcbs[pcb->pid];
    pcbs[pcb->pid] = NULL;

    return 1;
}

//Returns a pcb whos pid matches
pcb* pcbmanager::getPCB(int pid){
    return pcbs[pid];
}