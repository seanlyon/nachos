#include "pcbmanager.h"

PCBManager::PCBManager(int maxProcesses)
{
    bitmap = new BitMap(maxProcesses);
    pcbs = new PCB *[maxProcesses];

    for (int i = 0; i < maxProcesses; i++)
    {
        pcbs[i] = NULL;
    }
}

PCBManager::~PCBManager()
{
    delete bitmap;
    delete pcbs;
}

PCB *PCBManager::AllocatePCB()
{

    // pcbManagerLock->Acquire();

    int pid = bitmap->Find();

    // pcbManagerLock->Release();

    ASSERT(pid != -1);

    pcbs[pid] = new PCB(pid);

    return pcbs[pid];
}

int PCBManager::DeallocatePCB(PCB *pcb)
{
    int pid = pcb->pid;

    // Check is pcb is valid -- check pcbs for pcb->pid
    if (pcbs[pid] == NULL)
        return -1;

    // pcbManagerLock->Acquire();

    bitmap->Clear(pid);

    // pcbManagerLock->Release();

    delete pcbs[pid];
    pcbs[pid] = NULL;

    return 0;
}

PCB *PCBManager::GetPCB(int pid)
{
    return pcbs[pid];
}