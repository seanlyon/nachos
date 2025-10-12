void Elevator(int numFloors) {
    
}

void ArrivingGoingFromTo(int atFloor, int toFloor) {

}

// Waiting queue

struct ElevatorThread {
    int numFloors;
    int currentFloor;
    int numPeopleIn;

// Wait until waiting queue is not empty
// Increment floor until currentFloor = atFloor
// Yield to PersonThread
// Increment numPeopleIn
// Increment floor until currentFloor = toFloor or atFloor
// Increment numPeopleIn
// Yield to PersonThread
// Repeat until numPeopleIn = 0
// Wait until waiting queue is not empty

}

struct PersonThread {
    int id;
    int atFloor;
    int toFloor;

// Request elevator
// Print "Person x wants to go to floor y from floor z"
// Wait until elevator arrives
// Get into elevator
// Print "Person x got into the elevator"
// Tell elevator which floor to go to
// Wait until elevator arrives
// Get out of elevator
// Print "Person x got out of the elevator"

}

