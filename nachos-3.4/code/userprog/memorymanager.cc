#include "memorymanager.h"

/*memorymanager is responsible for assigning physical memory to processes during address space construction. It fuctions through a
bitmap that flips a bit when a "physical address" is allocated or deallocated*/

//ONLY ONE MEMORY MANAGER IS USED, DECLARATION AND INITIALIZATION IS LOCATED IN system.h/cc (found in threads folder).

//Constructor. Intializes virtual memory through a bitmap.
//Bitmap initialized to the maximum amount of pages avaliable on the OS
memorymanager::memorymanager() {
	Mem = new BitMap(NumPhysPages);
}

//Deconstructor
memorymanager::~memorymanager() {
	delete Mem;
}

//Finds and returns the first avaliable page on the bitmap
int memorymanager::allocatePage() {
	int page = Mem->Find();

	ASSERT(page != -1);

	return page;
}

//Clears a page of use making it avaliable
void memorymanager::deallocatePage(int ppid) {
	
	if (Mem->Test(ppid)) {
		Mem->Clear(ppid);
	}
	else {
		return;
	}

}

int memorymanager::getFreePageCount(){
	return Mem->NumClear();
}