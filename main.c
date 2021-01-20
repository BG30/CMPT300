#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "kallocator.h"
#include "mylist.h"

int main(int argc, char* argv[]) {
    initialize_allocator(100, FIRST_FIT);
    //initialize_allocator(100, BEST_FIT);
    //initialize_allocator(100, WORST_FIT);

    printf("Using first fit algorithm on memory size 100\n");
    //printf("Using best fit algorithm on memory size 100\n");
    //printf("Using worst fit alogorithm on memory size 100\n");

    int* p[50] = {NULL};
    for(int i=0; i<12; ++i) {
        p[i] = kalloc(sizeof(int));
        if(p[i] == NULL) {
            printf("Allocation failed\n");
            continue;
        }
        *(p[i]) = i;
        printf("p[%d] = %p ; *p[%d] = %d\n", i, p[i], i, *(p[i]));
    }
  
    print_statistics();


//Custom free test

printf("\n\nAFTER FREEING\n\n");
    kfree(p[1]);
    kfree(p[3]);
    kfree(p[5]);
    kfree(p[7]);
    kfree(p[9]);
    kfree(p[2]);
   kfree(p[6]);

    print_statistics();
//    printf("\n");

/*
    for(int i=0; i<10; ++i) {
        if(i%2 == 0)
            continue;

        printf("Freeing p[%d]\n", i);
        kfree(p[i]);
        p[i] = NULL;
    }
*/    
//Compacting

    printf("\navailable_memory %d\n\n", available_memory());

        void* before[100] = {NULL};
    for(int i=0; i<12; ++i) {
       before[i] = p[i];
    }
    void* after[100] = {NULL};

    int count = compact_allocation(before, after);
    
    for (int i = 0; i < count; ++i){
        //update pointers
        p[i] = after[i];
    }

    print_statistics();

/*
    // You can assume that the destroy_allocator will always be the 
    // last funciton call of main function to avoid memory leak 
    // before exit
*/
    destroy_allocator();

    return 0;
}