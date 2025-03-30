//
//  cStackF.h
//  op-sys-workspace
//
//  Created by Yu Xia on 8/20/24.
//

#ifndef cStackF_h
#define cStackF_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <math.h>

//the pointer memory is clean and unfragmented
//the actual memory will be heavily fragmented with each dynamic allocation while running
//this is not by design it was simply simpler

struct cLIST {
    int size;
    struct dataCell** startptr;
    /*
     *cLIST here is a pointer to a **ptr of dataCell
     *it stores the pointer in a neat package along with the relevent data of its size
     *this is to try and make this less messy to handle but it could result in messier data handling
     */
    //DEPENDENT
    //done
} CELLARRAY;

struct dataCell {
    short empty;
    void* ptr;
    /*
     *dataCell here is the actual structure of the data type
     *this is usually in the form of a pointer to a pointer
     *a **ptr would be made to house 'x' number of pointers to dataCell
     *those *ptr would point to the actual location where the data can be altered
     *the second pointer is needed to make realloc calls easier to manage
     *this will eventually result in heavy memory defrag but that can be managed using
     *another set of functions if the need ever arises
     */
    
    /*
     *IMPORTANT ::
     *NONE OF THE dataCell SPECIFIC FUNCTIONS HAS OUT OF BOUND CHECKS;
     *cLIST IS NEEDED FOR THAT, IT IS WISE TO NOT USE dataCel ON ITS OWN WITHOUT EXPLICIT KNOWLEDGE
     */
    //BASE
    //done
} DATACELL;

//OPERATION: release all memory in the dataCell structs
void freeALL(struct dataCell** ptr, int size) {
    for(int i = 0; i < size; i++) {
        free((*(ptr+i))->ptr);
        free(*(ptr+i));
        //free DATACELL stored value then free DATACELL itself
        //if the DATACELL stores a struct that holds pointers then this will not free all memory
        //for those kind of stored information a new deallocate function must be made
    }
    free(ptr);
    //BASE
    //done
}
//OPERATION: serves the same function as above but is for cLIST struct
void freeLST(struct cLIST* ptr) {
    freeALL(ptr->startptr, ptr->size);
    free(ptr);
    //DEPENDENT
    //done
}
//OPERATION: frees this DATACELL
void freeTHIS(struct dataCell** ptr, int index) {
    
    //BASE
    //need implement
}
//OPERATION: frees only one DATACELL and only this specific one
void freeINDEX(int index, struct cLIST* ptr) {
    if(index < 0 || index >= ptr->size) {
        return;
    }
    struct dataCell** head = ptr->startptr;
    free((*(head+index))->ptr);
    (*(head+index))->empty = 0;
    (*(head+index))->ptr = calloc(1, sizeof(char));
    //freeTHIS must initialize a new empty ptr of size char for 0
    //if this is not done for targeted free operations then future operations will attempt to access
    //non-existant memory and or free non-existant memory
    return;
    //DEPENDENT
    //need update (refer to freeTHIS)
}
//OPERATION: frees a range of DATACELLs
void freeRANGE(int begin, int end, struct cLIST* ptr) {
    for(int i = begin; i <= end; i++) {
        freeINDEX(i, ptr);
    }
    //this freeRANGE operation will free memory from begin to end INCLUSIVE
    //rememebr this is INCLUSIVE [] not UP-TO [)
    return;
    //DEPENDENT
    //done
}
//OPERATION: frees based given parameter for all memory
void freeALL_OTHER(void (*otherOP) (struct dataCell**), struct dataCell** ptr, int size) {
    for(int i = 0; i < size; i++) {
        otherOP(ptr+i);	//this one provides the pointer to the dataCell to otherOP
        free(*(ptr+i));	//this would then free the pointer pointing to actual struct
    }
    free(ptr);
    //this is for dataCell Structs, otherOP needs to be able to free memory for a unique data type
    //the data type is assumed to be a custom struct or maybe an object
    //if the data type is primitive or a supported type then the otherOP would either need
    //to incorporate those free functions or use the free functions directly
    //BASE
    //done
}
//OPERATION: serves the same function above but only for a single entry
void freeTHIS_OTHER(void (*otherOP) (struct dataCell**), struct dataCell** ptr, int index) {
    otherOP(ptr+index);
    free(*(ptr+index));
    //BASE
    //need implement
}
//OPERATION: serves the same function as the one before above but for a range of entries
void freeRANGE_OTHER(void (*otherOP) (struct dataCell**), struct dataCell** ptr, int begin, int end) {
    otherOP(ptr);
    //BASE
    //need implement
}

//OPERATION: allocate memory in dataCell structs
struct dataCell** allocCELL_MEM(int size) {
    struct dataCell** init = calloc(size, sizeof(DATACELL));
    for(int i = 0; i < size; i++) {
        *(init+i) = calloc(1, sizeof(DATACELL));
        (*(init+i))->ptr = calloc(1, sizeof(char));
    }
    return init;
    //BASE
    //done
}
//OPERATION: serves the same function as above but is for cLIST struct
struct cLIST* allocCELL_LST(int len) {
    struct cLIST* init = calloc(1, sizeof(CELLARRAY));
    init->size = len;
    init->startptr = allocCELL_MEM(len);
    return init;
    //DEPENDENT
    //done
}

//OPERATION: the following are for the addition & the insertion of dataCell struct
int addCELL(struct dataCell** cells, struct dataCell* addition, int size) {
    //addendum needed - realloc if the list is full, set it as a flag on compile time
    //addendum cont'd - realloc isn't always needed and have the capabilities be off by default
    for(int i = 0; i < size; i++) {
        if((*(cells+i))->empty == 0) {
            struct dataCell** release = (cells+i);
            free((*(cells+i))->ptr);
            free(*(cells+i));
            //free old placeholder memory
            *(cells+i) = addition;
            (*(cells+i))->empty = 1;
            //insert new memory and then set empty flag to 1 to signal non-empty DATACELL
            return i;
        }
    }
    //there is probably a good way to check for a free dataCell without iterating through the entire thing
    //maybe set a variable in the cList pointing to the most recent empty dataCell
#ifdef AUTO_RE_ALLOC
    realloc(cells, size * 2 * sizeof(DATACELL));
    addCELL(cells, addition, 2 * size);
    return -2;
    //the return value of -2 will need a catch case to double the value of size outside of this
    //function as the value size is a pass by value and not pass by reference
    //if defined when compiled the function here will re-alloc automatically when the for loop
    //cannot find a dataCell to write to; otherwise it will simply return -1 to signify that
    //it cannot find a place to insert it
#endif
    /*
    OPERATION:
     	this addition call will try to find a free space to add the data struct
     	if no free memory is found then the call will return -1 to signify no space found
    */
    return -1;
    //BASE
    //done
}
struct dataCell* insertCELL(struct dataCell** cells, struct dataCell* insertion, int here, int size) {
    //cells is the array of data stored
    //insertion is the data to be inserted
    //here is the index to insert
    //size is the size of the array of data
    if(here < 0 || here >= size) {return insertion;}
    else {
        struct dataCell* retptr = *(cells+here);
        *(cells+here) = insertion;
        return retptr;
    }
    /*
	OPERATION:
    	this insertion call for an array of structs will not free the struct stored at "here"
     	the new pointer will take the place of "here" and "here" will be assigned to "retptr"
    	then "retptr" will be returned so either assign or free it after calling this function
     	if the insertion is not possible then "insertion" will be returned instead
    */
    //BASE
    //done
}

//OPERATION: the following are for the addition & the insertion of cLIST struct
int addCELL_LST(struct dataCell* addition, struct cLIST* array) {
    return addCELL(array->startptr, addition, array->size);
    //DEPENDENT
    //done
}
struct dataCell* insertCELL_LST(struct dataCell* insertion, int here, struct cLIST* array) {
    return insertCELL(array->startptr, insertion, here, array->size);
    //DEPENDENT
    //done
}

#endif /* cStackF_h */
