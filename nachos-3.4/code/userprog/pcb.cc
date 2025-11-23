#include "pcb.h"

/*Process Control Block (PCB) is responsible for keeping track of relevant information regarding a process.
At contstruction/allocation it is only set with the processes id (taken from the pcbmanagers bitmap). All other 
variables must be set during address space construction or during system call functions.

Creation of pcbs should only be done through the pcbmanager as there is no check within this class whether or not
theres avaliable process space.
*/

//Constructor. Initializes a pcb and sets its id to the associated proccesses id
//parent and associated thread is set to null at initialization
//list of children is initialized
pcb::pcb(int id){
    pid = id;
    parent = NULL;
    thread = NULL;
    exitStatus = -9999;

    children = new List();

}

//Deconstructor (needs work)
pcb::~pcb(){
    delete children;
}

//Adds the passed pcb to the list of children.
void pcb::addChild(pcb* child){
    children -> Append(child);
}

//Removes the passed child from list of children.
int pcb::removeChild(pcb* child){
    return children->RemoveItem(child);
}

//Checks if process has been successfully completed
bool pcb::hasExited(int arg){
    return (exitStatus = -9999 ? false: true); 
}

//Performs some cleanup when a process is exited.
//Children associated with process have their parent set to null
//Parent of process that makes this call has this process removed from
//children
void pcb::exitCleanup(pcbmanager* manager){

    if (!(children -> IsEmpty())){
        do{
            pcb* child = (pcb*)children -> Remove();

            child -> parent = NULL;

        }while (!(children -> IsEmpty()));
    }


    if (parent != NULL){
        parent -> removeChild(manager->getPCB(pid));
    }
}
