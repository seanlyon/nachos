#include "elevator.h"
#include "system.h"
#include "synch.h"

Lock* lock = new Lock("Elevator lock");
Condition* cond = new Condition("Elevator condition");

List* call = new List;
List* stop = new List;

void Elevator(int numFloors) {
    ElevatorThread* e = new ElevatorThread;
    e->numFloors = numFloors;

    Thread* t = new Thread("elevator");
    t->Fork(elevatorLoop, (int)e);
}

void elevatorLoop(int elevatorThread) {
    ElevatorThread* e = (ElevatorThread*)elevatorThread;
    int nextCall = 0;
    int nextStop = 0;
    while (true) {
        lock->Acquire();
        while (stop->IsEmpty() && call->IsEmpty()) {
            printf("elevatorLoop\n");
            cond->Wait(lock);
        }
        nextCall = (int)call->Remove();
        nextStop = (int)stop->Remove();
        
        if (nextStop != NULL) {
            if (e->currentFloor > nextStop) {
                e->currentFloor--;
                stop->Prepend((void*)nextStop);
            } else if (e->currentFloor < nextStop) {
                e->currentFloor++;
                stop->Prepend((void*)nextStop);
            } else {
                printf("currentFloor = nextStop");
            }
        }
        else if (nextCall != NULL) {
            if (e->currentFloor > nextCall) {
                call->Prepend((void*)nextCall);
            } else if (e->currentFloor < nextCall) {
                call->Prepend((void*)nextCall);
            } else {
                printf("currentFloor = nextCall");
            }
        }
        lock->Release();
    }
}

void ArrivingGoingFromTo(int atFloor, int toFloor) {
    PersonThread* p = new PersonThread;
    p->id = atFloor;
    p->atFloor = atFloor;
    p->toFloor = toFloor;

    Thread* t = new Thread("person");
    t->Fork(rideElevator, (int)p);
}

void rideElevator(int personThread) {
    PersonThread* p = (PersonThread*)personThread;
}

