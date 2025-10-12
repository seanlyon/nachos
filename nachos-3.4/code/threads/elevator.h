#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "copyright.h"

void Elevator(int numFloors);
void elevatorLoop(int elevatorThread);
void ArrivingGoingFromTo(int atFloor, int toFloor);
void rideElevator(int personThread);

struct PersonThread {
    int id;
    int atFloor;
    int toFloor;
};

struct ElevatorThread {
    int numFloors;
    int currentFloor;
    int numPeopleIn;
};


#endif
