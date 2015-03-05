/* 
 * File:   mem_heap.h
 * Author: Matthew Britt
 * This is the specification of a class named MemHeap,
 * a memory management class that will inherit from the
 * abstract class VHeap. It includes two helper functions 
 * which are for purely testing purposes.
 * Created on 20 April 2013, 11:16 PM
 */
#include "v_heap.h"

class MemHeap : public VHeap
{
    struct Block 
    {
        int size;
        bool isFree;
        Block* next;
        Block* prev;
    };
public:
    MemHeap();
    void* vmalloc(size_t size);
    void* vcalloc(size_t size);
    void vfree(void* mem);
    size_t vsizeof(void* mem);
    void printHeapState();
    size_t getBlockSize(); // to be used in main for portability of the testing
private:
    void displayBlockInfo(Block* block); //used to show information about each block as it is allocated
};

