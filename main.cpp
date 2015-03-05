/* 
 * File:   main.cpp
 * Author: Matthew Britt
 * This program tests the various functions implemented in the MemHeap class.
 * Created on 20 April 2013, 11:14 PM
 */

#include <iostream>
#include "mem_heap.h"

using namespace std;

void testAllocationAndFree();
void testFillToEnd();
void testCallocAndSizeOf();
void testPrintHeapState();

/*
 * The main of this program displays messages detailing the tests being performed
 * and then calls the function which will carry out the appropriate test. 
 */
int main()
{
    cout << endl << "==========================================================================" << endl;
    cout << endl << "First set of tests is to check vcalloc() and vsizeof() for correct outputs " << endl;
    cout << endl << "==========================================================================" << endl;
    testCallocAndSizeOf();
    
    cout << endl << "==============================================================================================" << endl;
    cout << endl << "Second set of tests is to check vmalloc() and vfree() and printHeapState() for correct outputs " << endl;
    cout << endl << "==============================================================================================" << endl;
    testAllocationAndFree();

    cout << endl << "====================================================================" << endl;
    cout << endl << "Third set of tests is to check if out of heap allocation is possible" << endl;
    cout << endl << "====================================================================" << endl;
    testFillToEnd();

    cout << endl << "===================================================================================" << endl;
    cout << endl << "Fourth set of tests is to check various scenarios for printHeapState() to represent" << endl;
    cout << endl << "===================================================================================" << endl;
    testPrintHeapState();

    return 0;
}

/*
 * This function allocates several integers and chars with the MemHeap object that is instantiated
 * printing out the state of the heap at particular points in the course of the operations to show
 * the state of the heap and that it is being correctly allocated and freed. Also tested is the MemHeap's
 * ability to merge free blocks together when the situation arises, and also that any free bytes between a
 * used block and free one will be taken up by the free block, which is essentially shuffled into the free space.
 */
void testAllocationAndFree()
{
    MemHeap test;

    cout << endl << "Testing vmalloc() function" << endl;
    cout << endl << "Allocating first int" << endl << endl;
    int *myInt1 = (int*)test.vmalloc(sizeof(int));
    *myInt1 = 100;
    cout << endl << "Value of first int: " << *myInt1 << endl << endl;
    cout << endl;
    

    cout << endl << "Allocating second int" << endl << endl;
    int *myInt2 = (int*)test.vmalloc(sizeof(int));
    *myInt2 = 200;
    cout << endl << "Value of second int: " << *myInt2 << endl;
    cout << endl;
    
    cout << endl << "Allocating third int" << endl << endl;
    int *myInt3 = (int*)test.vmalloc(sizeof(int));
    *myInt3 = 300;
    cout << endl << "Value of third int: " << *myInt3 << endl;
    cout << endl;
    
    cout << endl << "Allocating fourth int" << endl << endl;
    int *myInt4 = (int*)test.vmalloc(sizeof(int));
    *myInt4 = 400;
    cout << endl << "Value of fourth int: " << *myInt4 << endl;
    cout << endl;
    
    cout << endl << "Allocating fifth int" << endl << endl;
    int *myInt5 = (int*)test.vmalloc(sizeof(int));
    *myInt5 = 500;
    cout << endl << "Value of fifth int: " << *myInt5 << endl;
    cout << endl;
    
    cout << endl << "Allocating sixth int" << endl << endl;
    int *myInt6 = (int*)test.vmalloc(sizeof(int));
    *myInt6 = 600;
    cout << endl << "Value of fifth int: " << *myInt6 << endl;
    cout << endl;

    cout << endl << "Allocating first char" << endl << endl;
    char *myChar1 = (char*)test.vmalloc(sizeof(char));
    *myChar1 = 'A';
    cout << endl << "Value of first char: " << *myChar1 << endl;
    cout << endl;

    cout << endl << "Allocating second char" << endl << endl;
    char *myChar2 = (char*)test.vmalloc(sizeof(char));
    *myChar2 = 'B';
    cout << endl << "Value of second char: " << *myChar2 << endl;
    cout << endl;

    cout << endl << "Allocating third char" << endl << endl;
    char *myChar3 = (char*)test.vmalloc(sizeof(char));
    *myChar3 = 'C';
    cout << endl << "Value of third char: " << *myChar3 << endl;
    cout << endl;

    cout << endl << "Allocating fourth char" << endl << endl;
    char *myChar4 = (char*)test.vmalloc(sizeof(char));
    *myChar4 = 'D';
    cout << endl << "Value of fourth char: " << *myChar4 << endl;
    cout << endl;

    cout << endl << "State of the heap after allocations: " << endl << endl;
    test.printHeapState();
    cout << endl << endl;

    cout << "Testing vfree() function" << endl << endl;

    cout << "Freeing second int" << endl << endl;
    test.vfree(myInt2);
    cout << "State of heap after free: " << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Freeing fourth int" << endl << endl;
    test.vfree(myInt4);
    cout << "State of heap after free: " << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Freeing third int to show merging of free space to left and right" << endl << endl;
    test.vfree(myInt3);
    cout << "State of heap after free: " << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Allocating third int once again to show insertion into first found free space" << endl << endl;
    myInt3 = (int*)test.vmalloc(sizeof(int));
    *myInt3 = 300;
    cout << endl << "Value of third int: " << *myInt3 << endl;
    cout << endl;
    cout << "State of heap after allocation: " << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Freeing first int and allocating a new char to show 3 free bytes between data and next block" << endl << endl;
    test.vfree(myInt1);
    char *myChar5 = (char*)test.vmalloc(sizeof(char));
    *myChar5 = 'C';
    cout << endl << "Value of new char: " << *myChar5 << endl;
    cout << endl;
    cout << "State of heap after allocation: " << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Freeing third int again to free block to the right of new char that is at beginning of heap" << endl << endl;
    test.vfree(myInt3);
    cout << "State of heap after free (Block should shuffle left to fill free bytes): " << endl << endl;
    test.printHeapState();
    cout << endl;
    
    cout << endl << "Allocating two new ints to show the free space between two blocks being utilised" << endl << endl;
    int *myInt7 = (int*)test.vmalloc(sizeof(int));
    *myInt7 = 700;
    int *myInt8 = (int*)test.vmalloc(sizeof(int));
    *myInt8 = 800;
    
    cout << endl << "State of heap after two allocations with previously free space now in use" << endl << endl;
    test.printHeapState();
    cout << endl;
}

/*
 * This function tests vcalloc() by filling an array with some characters from the ascii table and after
 * freeing that block performing a calloc to show that the values have been completely zeroed.
 * Also tested is vsizeof() which shows that the MemHeap correctly returns the size of data allocated to each block.
 */
void testCallocAndSizeOf()
{
    MemHeap test;
    cout << endl << "Testing vCalloc() function" << endl << endl;
    cout << "Creating char array with vmalloc and filling with data" << endl << endl;
    char *myCharArray1 = (char*)test.vmalloc(sizeof(char) * HEAP_SIZE - test.getBlockSize());
    
    for (int i = 0, j = 48; i < HEAP_SIZE - test.getBlockSize(); ++i, ++j) {
        *(myCharArray1 + i) = j;
        if (j == 122) {
            j = 47;
        }
    }
    
    cout << endl << "Printing contents of full array" << endl << endl;
    for (int i = 0; i < HEAP_SIZE - test.getBlockSize(); ++i) {
        cout << *(myCharArray1 + i);
    }
    cout << endl << endl;

    cout << endl << "State of heap with char array" << endl << endl;
    test.printHeapState();
    cout << endl;
    
    cout << endl << "Freeing char array" << endl << endl;
    test.vfree(myCharArray1);
    
    cout << "State of heap after free: " << endl << endl;
    test.printHeapState();
    cout << endl;
    
    cout << endl << endl << "Creating char array of same size with vcalloc() (Memory addresses should match first char array)" << endl << endl;
    char *myCharArray2 = (char*)test.vcalloc(sizeof(char) * HEAP_SIZE - test.getBlockSize());
    
    cout << endl << "Printing contents of array after vcalloc() (Should not print any leftover junk - should simply be blank): " << endl << endl;
    bool testPassed = true;
    for (int i = 0; i < HEAP_SIZE - test.getBlockSize(); ++i) {
        cout << *(myCharArray2 + i);
        if (*(myCharArray2 + i) != 0) {
            testPassed = false;
        }
    }
    cout << endl;
    
    if (testPassed) {
        cout << endl << "Test Passed" << endl;
    }
    else {
        cout << endl << "Test Failed" << endl;
    }


    cout << endl << "State of heap after vcalloc()" << endl << endl;
    test.printHeapState();
    cout << endl;
    
    cout << endl << "Testing vsizeof() function" << endl;
    cout << endl << "Size of char array: " << test.vsizeof(myCharArray2) << " Bytes" << endl << endl;
    
    if (test.vsizeof(myCharArray2) == HEAP_SIZE - test.getBlockSize()) {
        cout << "Test Passed" << endl << endl;
    }
    else {
        cout << "Test Failed" << endl << endl;
    }
}

/*
 * This function tests a situation that involves the heap to be filled completely 
 * and then an attempt is made to allocate another piece of memory which should not succeed.
 */
void testFillToEnd() 
{
    MemHeap test;

    cout << endl << "Creating array of chars long enough to fill heap entirely" << endl << endl;
    char *myCharArray1 = (char*)test.vcalloc(sizeof(char) * HEAP_SIZE - test.getBlockSize());
    
    cout << endl << "State of heap after filling heap" << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Attempting to create myInt1 to show no data can be allocated outside of array" << endl << endl;
    int *myInt1 = (int*)test.vcalloc(sizeof(int));
    
    if (myInt1 != NULL) {
        *myInt1 = 100;
        cout << endl <<"Value of myInt1: " << *myInt1 << endl;
        cout << endl << "Test Failed" << endl;
    }
    else {
        cout << "myInt1 was not created" << endl;
        cout << endl << "Test Passed" << endl;
    }

    cout << endl << "State of heap after attempted allocation" << endl << endl;
    test.printHeapState();
    cout << endl;
}

/*
 * This function simply creates various situations in the heap to be printed, 
 * especially the way the class leaves free bytes after a block of data if the space
 * after it would not be sufficient for another struct plus data. 
 */
void testPrintHeapState()
{
    MemHeap test;
    cout << endl << "This test simply shows the printHeapState() function as it accurately \n";
    cout << "represents the heap depending on what has been allocated or freed" << endl << endl;

    cout << "Allocating memory for a char array that will not leave enough space after it to fit a new information block plus any data \n";
    cout << "(No block is created if that block wouldn't be able to hold any data)" << endl << endl;
    char *myCharArray1 = (char*)test.vmalloc(sizeof(char) * HEAP_SIZE - ((test.getBlockSize() * 2) + 8));
    
    cout << endl << "State of heap after allocation" << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Freeing memory allocated for array" << endl << endl;
    test.vfree(myCharArray1);
    
    cout << endl << "State of heap after free" << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Allocating char array again, this time with enough space to fit more data after it" << endl << endl;
    myCharArray1 = (char*)test.vmalloc(sizeof(char) * HEAP_SIZE - (test.getBlockSize() * 3));
    
    cout << endl << "State of heap after allocation" << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Allocating an int to the heap" << endl << endl;
    int *myInt1 = (int*)test.vmalloc(sizeof(int));
    
    cout << endl << "State of the heap after allocation" << endl << endl;
    test.printHeapState();
    cout << endl;

    cout << endl << "Freeing char array" << endl << endl;
    test.vfree(myCharArray1);
    
    cout << "State of heap after free" << endl << endl;
    test.printHeapState();
    cout << endl;
}
