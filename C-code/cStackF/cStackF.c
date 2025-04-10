//
//  cStackF.c
//  op-sys-workspace
//
//  Created by Yu Xia on 8/20/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <math.h>

#include "cStackF.h"

int main(int argc, char** argv) {
    struct dataCell** testptr = allocCELL_MEM(10);
    for(int i = 0; i < 10; i++) {
        fprintf(stdout, "pointer is %p\n", testptr+i);
        fprintf(stdout, "memory there is %d\n", (*(testptr+i))->empty);
    }
    int retInt = 0;
    struct dataCell* testAddCell = calloc(1, sizeof(DATACELL));
    int * genericT = calloc(1, sizeof(int));
    *genericT = 22;
    testAddCell->ptr = (void *) genericT;
    retInt = addCELL(testptr, testAddCell, 10);
    fprintf(stdout, "\n\nTest after addition operation...\n");
    for(int i = 0; i < 10; i++) {
        fprintf(stdout, "pointer is %p\n", testptr+i);
        fprintf(stdout, "memory there is %d\n", *(int*) (*(testptr+i))->ptr);
    }
    
    struct cLIST* testList = allocCELL_LST(10);
    //IMPORTANT NOTICE: the following is the construction process for a new data entry
    //data entry will differ for whatever generic type so each constructor will be unique
    struct dataCell* testAddCell2 = calloc(1, sizeof(DATACELL));
    int * genericT2 = calloc(1, sizeof(int));
    *genericT2 = 23;
    testAddCell2->ptr = (void *) genericT2;
    retInt = addCELL_LST(testAddCell2, testList);
    //
    struct dataCell* testAddCell3 = calloc(1, sizeof(DATACELL));
    int * genericT3 = calloc(1, sizeof(int));
    *genericT3 = 32;
    testAddCell3->ptr = (void *) genericT3;
    retInt = addCELL_LST(testAddCell3, testList);
    
    fprintf(stdout, "\n\nTesting addition operation for cList...\n");
    for(int i = 0; i < 10; i++) {
        fprintf(stdout, "memory address is %p\n", (((testList->startptr)+i)));
        fprintf(stdout, "memory here is %d\n", *(int*) (*((testList->startptr)+i))->ptr);
    }
    
    freeINDEX(1, testList);
    fprintf(stdout, "\n\nTesting targeted free operation for cList...\n");
    for(int i = 0; i < 10; i++) {
        fprintf(stdout, "memory address is %p\n", (((testList->startptr)+i)));
        fprintf(stdout, "memory here is %d\n", *(int*) (*((testList->startptr)+i))->ptr);
    }
    
    //
    struct dataCell* testAddCell4 = calloc(1, sizeof(DATACELL));
    int * genericT4 = calloc(1, sizeof(int));
    *genericT4 = 77;
    testAddCell4->ptr = (void *) genericT4;
    retInt = addCELL_LST(testAddCell4, testList);
    
    fprintf(stdout, "\n\nChecking the memory for stack after inclusion of 77 for cList...\n");
    for(int i = 0; i < 10; i++) {
        fprintf(stdout, "memory address is %p\n", (((testList->startptr)+i)));
        fprintf(stdout, "memory here is %d\n", *(int*) (*((testList->startptr)+i))->ptr);
    }
    
    freeRANGE(0, 1, testList);
    
    fprintf(stdout, "\n\nTesting range free of variables in cList...\n");
    for(int i = 0; i < 10; i++) {
        fprintf(stdout, "memory address is %p\n", (((testList->startptr)+i)));
        fprintf(stdout, "memory here is %d\n", *(int*) (*((testList->startptr)+i))->ptr);
    }
    
    freeLST(testList);
    freeALL(testptr, 10);
}
