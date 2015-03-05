/* 
 * File:   mem_heap.cpp
 * Author: Matthew Britt
 * This is the definition file of the class named MemHeap,
 * a memory management class that will inherit from the
 * abstract class VHeap. It includes two helper functions 
 * which are for purely testing purposes. 
 * Created on 20 April 2013, 11:16 PM
 */

#include "mem_heap.h"
#include <iostream>
#include <string.h>
#define PRINT_WIDTH HEAP_SIZE / 16 // The width of each line to be used by printHeapState to keep the printout neat and uniform in appearance

using namespace std;

/*
 * The constructor which takes no parameters. VHeap's constructor
 * is called in the initialiser list. Sets the first block
 * to the beginning of the heap, indicates that the block is free 
 * and sets the blocks size to the amount of bytes left within the array  
 */
MemHeap::MemHeap()
: VHeap()
{
    Block* temp = (Block*)vheap;
    temp->next = NULL;
    temp->prev = NULL;
    temp->isFree = true;
    temp->size = HEAP_SIZE - sizeof(Block);
}

/*
 * This function traverses the linked list of Block structs until it finds one that is free to use 
 * and large enough to hold the data asked for in the size parameter.
 * If space is not found NULL is returned and a message is printed, if space is
 * found the Block is made to reflect the relevant information and a pointer to the data is returned.
 */
void* MemHeap::vmalloc(size_t size)
{
    Block* temp = (Block*)vheap;
    
    while (temp) {
        if (temp->isFree) {
            if (temp->size >= size) {
                temp->isFree = false;
                if (temp->next == NULL && temp->size > size + sizeof(Block)) {
                    temp->next = (Block*)((char*)temp + sizeof(Block) + size);
                    temp->next->isFree = true;
                    temp->next->size = temp->size - (sizeof(Block) + size);
                    temp->next->prev = temp;
                    temp->next->next = NULL;
                }
                
                // creates a new Block if there is space between this block and next block 
                // to ensure all space available is going to be used
                else if (temp->next != NULL && (((char*)temp->next - ((char*)temp + sizeof(Block) + size)) > sizeof(Block))) {
                    Block* nextBlock = temp->next;
                    temp->next = (Block*)((char*)temp + sizeof(Block) + size);
                    temp->next->size = ((char*)nextBlock - ((char*)temp->next + sizeof(Block)));
                    temp->next->next = nextBlock;
                    temp->next->isFree = true;
                    temp->next->prev = temp;
                    temp->next->next->prev = temp->next;
                }
                
                temp->size = size;
                displayBlockInfo(temp);
                return (void*)((char*)temp + sizeof(Block));
            }
        }
        temp = temp->next;
    }
    
    // if it gets here no suitable space was found
    cout << "Not enough space in the heap!" << endl;
    return NULL;
}

/*
 * This function takes a size as the parameter and calls vmalloc to allocate memory of that size.
 * IF no space is found NULL is returned, otherwise all data in the memory that vmalloc returns is set to 0
 * by using memset. A pointer to the data location is returned.
 */
void* MemHeap::vcalloc(size_t size)
{
    void* temp = vmalloc(size);
    if (temp == NULL)
        return NULL;
    memset(temp,0,size);
    return temp;
}

/*
 * 
 */
void MemHeap::vfree(void* mem)
{
    Block* temp = (Block*)mem;
    temp--;
    temp->isFree = true;
    
    // merges this block with the next one if it's free, adding it's size to this ones so no bytes are lost
    if (temp->next != NULL && temp->next->isFree) { 
        temp->size += (temp->next->size + sizeof(Block));
        temp->next = temp->next->next;
    }
    
    // merges this block with the next one if it's free, adding it's size to this ones so no bytes are lost
    if (temp->prev != NULL && temp->prev->isFree) {
        temp->prev->size += (temp->size +sizeof(Block));
        temp->prev->next = temp->next;
    }
    
    // if space is found between previous block and this block, 
    // this block is relocated to be immediately after previous, size is adjusted to reflect correctly
    if (temp->prev != NULL && ((char*)temp->prev + sizeof(Block) + temp->prev->size < (char*)temp)) {
        Block* previous = temp->prev;
        Block* next = temp->next;
        int newSize = temp->size + (((char*)temp->prev + sizeof(Block) + temp->prev->size) - ((char*)temp));
        temp = (Block*)((char*)temp->prev + sizeof(Block) + temp->prev->size);
        temp->size = newSize;
        temp ->next = next;
        temp->prev = previous;
        temp->isFree = true;
        temp->prev->next = temp;
        temp->next->prev = temp;
    }
    
    // only needed if this block's data ended near the end of the heap array
    // adds any free bytes that are not being managed by a block to this blocks size
    if (temp->next == NULL)  {
        temp->size += (((char*)vheap + HEAP_SIZE) - ((char*)temp + sizeof(Block) + temp->size));
    }  
}

/*
 * This function takes a void pointer which is pointing to some data. 
 * A Block pointer is created and set to the correct position and the 
 * size being managed by this Block is returned.
 */
size_t MemHeap::vsizeof(void* mem)
{
    Block* temp = (Block*)mem;
    temp--;
    return (size_t)temp->size;
}

/*
 * This function prints a representation of the current state of the heap
 * indicating each byte as a letter; 'I' represents a byte within a Block
 * struct, 'D' represents data that is in use and 'F' represents any free
 * bytes in the heap. The printout is formatted by using the PRINT_WIDTH 
 * constant.
 */
void MemHeap::printHeapState()
{
    Block* temp = (Block*)vheap;
    int cols = 0;
    int spaceUsed = 0;
    
    while (temp) {
        spaceUsed += temp->size + sizeof(Block);
        for (int i = 0; i < temp->size + sizeof(Block); ++i, ++cols) {
            if (cols == PRINT_WIDTH) {
                cout << "\n";
                cols = 0;
            }
            
            if (i < sizeof(Block)) {
                cout << "I"; // indicates the byte is part of a Block struct
            }
            
            else if (temp->isFree) {
                cout << "F"; // indicates that this byte is free and unallocated
            }
            
            else if (!temp->isFree){
                cout << "D"; // indicates this byte is data that has been allocated
            }
        }
        
        if (temp->next) { // Checks for space after the data of this block but before the beginning of the next block
            for (int i = 0; i < ((char*)temp->next - ((char*)temp + sizeof(Block) + temp->size)); ++i, ++cols) {
                if (cols == PRINT_WIDTH) {
                    cout << "\n";
                    cols = 0;
                }
                spaceUsed++; //each free byte found is one less to print in next loop
                cout << "F";
            }
        }
        
        else { // Checks for free bytes after last blocks data
            for (int i = 0; i < HEAP_SIZE - spaceUsed; ++i, ++cols) {
                if (cols == PRINT_WIDTH) {
                    cout << "\n";
                    cols = 0;
                }
                cout << "F";
            }
        }
        temp = (Block*)temp->next;
    }
    
    cout << "\n";
}

/*
 * This function is for testing purposes, it allows the main to ask for the size of the Block struct.
 * This allows for portability so that different architectures are represented and tested accurately.
 */
size_t MemHeap::getBlockSize()
{
    return sizeof(Block);
}

/*
 * This function is for testing purposes, it prints information about the Block that is passed in as a parameter.
 * This function is being called after every vmalloc to show where memory has been allocated to ensure proper allocation
 * and freeing of memory is occurring.
 */
void MemHeap::displayBlockInfo(Block* temp)
{
    cout << " Size of block info struct: " << sizeof(Block) << " bytes" << endl;
    cout << " Size of data that is allocated to this Block: " << temp->size << " bytes" << endl;
    cout << " =======================================" << endl;
    cout << " | Start of block:         " << temp << "   |" << endl;
    cout << " | End of block:           " << (Block*)((char*)temp + (sizeof(Block) - 1)) << "   |" << endl;
    cout << " | Start of data:          " << (Block*)((char*)temp + sizeof(Block)) << "   |" << endl;
    cout << " | End of data:            " << (Block*)((char*)temp + sizeof(Block) + (temp->size - 1)) << "   |" << endl;
    cout << " | Start of next block:    " << temp->next << ((temp->next)? "   |" : "           |") << endl;
    cout << " =======================================" << endl;
}
