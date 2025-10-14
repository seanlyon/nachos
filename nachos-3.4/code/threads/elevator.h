
#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "copyright.h"

void Elevator(int numFloors);
void ArrivingGoingFromTo(int atFloor, int toFloor);

typedef struct Person {
    int id;
    int atFloor;
    int toFloor;
} Person;


class ELEVATOR {

public:
    ELEVATOR(int numFloors);
    ~ELEVATOR();
    void hailElevator(Person *p);
    void start();
    void updateState();

private:
    int currentState;
    int currentFloor;
    int nextFloor;
    int maxFloor;

    Condition *state;
    Condition **entering;
    Condition **leaving;
    
    int *requests;
    int *upRequests;
    int *downRequests;

    int *personsWaiting;
    int occupancy;
    int maxOccupancy;
    Lock *elevatorLock;

};

#endif
