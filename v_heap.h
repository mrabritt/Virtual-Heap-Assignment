#ifndef VHEAP_H
#define VHEAP_H

#include <cstdlib>

#define HEAP_SIZE 2048

class VHeap {

protected:
	char * vheap;
	
public:
	VHeap()
	{
		vheap = new char[HEAP_SIZE];
	}
	
	virtual void* vmalloc(size_t size) = 0;
	
	virtual void* vcalloc(size_t size) = 0;
	
	virtual void vfree(void* mem) = 0;
	
	virtual size_t vsizeof(void* mem)  = 0;
	
	virtual void printHeapState()  = 0;
	
	virtual ~VHeap()
	{
		delete [] vheap;
	}
};
#endif // VHEAP_H
