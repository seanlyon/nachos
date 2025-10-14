#include "copyright.h"
#include "system.h"
#include "synch.h"
#include "elevator.h"

#define MAX_OCCUPANCY 5

int nextPersonID = 1;
Lock *personIDLock = new Lock("PersonIDLock");


ELEVATOR *e;

void ELEVATOR::updateState() {
    int nextState = 0;
    int floor = 0;

    if (currentState == 0) {
        for (int i = currentFloor - 1; i < maxFloor; i++) {
            if (requests[i]) {
                nextState = 1;
                floor = i + 1;
                break;
            }
        }
        if (floor == 0) {
        for (int i = currentFloor - 1; i > 0; i--) {
            if (requests[i]) {
                nextState = -1;
                floor = i + 1;
                break;
            }
        }
        }
    } else if (currentState == 1) {
        for (int i = currentFloor - 1; i < maxFloor; i++) {
            if (requests[i]) {
                nextState = 1;
                floor = i + 1;
                break;
            }
        }
        if (floor == 0) {
        for (int i = currentFloor - 1; i > 0; i--) {
            if (requests[i]) {
                nextState = -1;
                floor = i + 1;
                break;
            }
        }
        }
    } else if (currentState == -1) {
        for (int i = currentFloor - 1; i > 0; i--) {
            if (requests[i]) {
                nextState = -1;
                floor = i + 1;
                break;
            }
        }
        if (floor == 0) {
        for (int i = currentFloor - 1; i < maxFloor; i++) {
            if (requests[i]) {
                nextState = 1;
                floor = i + 1;
                break;
            }
        }
        }
    }
    /*
    for (int i = 0; i < maxFloor; i++) {
        if (requests[i]) {
            if ((i > currentFloor - 1) || (currentFloor == 1)) {
                nextState = 1;
            }
            else if ((i < currentFloor - 1) || (currentFloor == maxFloor)) {
                nextState = -1;
            }
            nextFloor = i + 1;
        }
    }
    */
    currentState = nextState;
    nextFloor = floor;
}

void ELEVATOR::start() {

    while(1) {

        // A. Wait until hailed
        elevatorLock->Acquire();
        while(currentState == 0) {
            wakeElevator->Wait(elevatorLock);
            //state->Wait(elevatorLock);
            updateState();
        }
        elevatorLock->Release();

        // B. While there are active persons, loop doing the following
        while(currentState != 0) {
        //      0. Acquire elevatorLock
            elevatorLock->Acquire();
            
        //      1. Signal persons inside elevator to get off (leaving->broadcast(elevatorLock))
            leaving[currentFloor - 1]->Broadcast(elevatorLock);
            while (exitsPending[currentFloor - 1] != 0) {
                floorCleared->Wait(elevatorLock);
            }

        //      2. Signal persons atFloor to get in, one at a time, checking occupancyLimit each time
            while ((personsWaiting[currentFloor - 1] > 0) && (occupancy < maxOccupancy)) {
                entering[currentFloor - 1]->Signal(elevatorLock);
                boarded->Wait(elevatorLock);
            }
            if (personsWaiting[currentFloor - 1] == 0) {
                requests[currentFloor - 1] = 0;
            }

            if (currentFloor == nextFloor) {
                updateState();
            }
        //      2.5 Release elevatorLock
            elevatorLock->Release();

        //      3. Spin for some time
            for(int j =0 ; j< 1000000; j++) {
                currentThread->Yield();
            }
        //      4. Go to next floor
            currentFloor = currentFloor + currentState;
            printf("Elevator arrives on floor %d\n", currentFloor);
        }
    }
}

void ElevatorThread(int numFloors) {

    printf("Elevator with %d floors was created!\n", numFloors);

    e = new ELEVATOR(numFloors);

    e->start();


}

ELEVATOR::ELEVATOR(int numFloors) {
    currentState = 0;
    currentFloor = 1;
    maxFloor = numFloors;
    // Initialize
    entering = new Condition*[numFloors];
    leaving = new Condition*[numFloors];
    exitsPending = new int[numFloors];
    personsWaiting = new int[numFloors];
    requests = new int[numFloors];
    for (int i = 0; i < numFloors; i++) {
        entering[i] = new Condition("Entering " + i);
        leaving[i] = new Condition("Leaving " + i);
        exitsPending[i] = 0;
        personsWaiting[i] = 0;
        requests[i] = 0;
    }

    elevatorLock = new Lock("ElevatorLock");
    wakeElevator = new Condition("Wake");
    boarded = new Condition("Boarded");
    floorCleared = new Condition("Floor Cleared");
    state = new Condition("State");
    occupancy = 0;
    maxOccupancy = MAX_OCCUPANCY;
}

ELEVATOR::~ELEVATOR() {
    
    for (int i = 0; i < maxFloor; i++) {
        delete entering[i];
        delete leaving[i];
    }
    delete[] entering;
    delete[] leaving;
    delete[] requests;
    delete[] personsWaiting;
}

void Elevator(int numFloors) {
    // Create Elevator Thread
    Thread *t = new Thread("Elevator");
    t->Fork(ElevatorThread, numFloors);
}


void ELEVATOR::hailElevator(Person *p) {
    // 1. Increment waiting persons atFloor
    elevatorLock->Acquire();
    personsWaiting[p->atFloor - 1]++;
    // 2. Hail Elevator
    requests[p->atFloor - 1] = 1;
    //state->Signal(elevatorLock);
    wakeElevator->Signal(elevatorLock);
    // 2.5 Acquire elevatorLock;
    // 3. Wait for elevator to arrive atFloor [entering[p->atFloor]->wait(elevatorLock)]
    while (currentFloor != p->atFloor) {
        entering[p->atFloor - 1]->Wait(elevatorLock);
    }
    // 5. Get into elevator
    printf("Person %d got into the elevator.\n", p->id);
    // 6. Decrement persons waiting atFloor [personsWaiting[atFloor]++]
    personsWaiting[p->atFloor - 1]--;
    // 7. Increment persons inside elevator [occupancy++]
    occupancy++;
    exitsPending[p->toFloor - 1]++;
    // 8. Wait for elevator to reach toFloor [leaving[p->toFloor]->wait(elevatorLock)]
    requests[p->toFloor - 1] = 1;
    //state->Signal(elevatorLock);
    boarded->Signal(elevatorLock);
    while (currentFloor != p->toFloor) {
        leaving[p->toFloor - 1]->Wait(elevatorLock);
    }
    // 9. Get out of the elevator
    printf("Person %d got out of the elevator.\n", p->id);
    // 10. Decrement persons inside elevator
    occupancy--;
    exitsPending[p->toFloor - 1]--;
    floorCleared->Signal(elevatorLock);
    // 11. Release elevatorLock;
    elevatorLock->Release();
}

void PersonThread(int person) {

    Person *p = (Person *)person;

    printf("Person %d wants to go from floor %d to %d\n", p->id, p->atFloor, p->toFloor);

    e->hailElevator(p);

}

int getNextPersonID() {
    int personID;
    personIDLock->Acquire();
    personID = nextPersonID;
    nextPersonID = nextPersonID + 1;
    personIDLock->Release();
    return personID;
}


void ArrivingGoingFromTo(int atFloor, int toFloor) {


    // Create Person struct
    Person *p = new Person;
    p->id = getNextPersonID();
    p->atFloor = atFloor;
    p->toFloor = toFloor;

    // Creates Person Thread
    Thread *t = new Thread("Person " + p->id);
    t->Fork(PersonThread, (int)p);

}
