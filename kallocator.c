#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"
#include "kallocator.h"
#include <string.h>

struct KAllocator {
    enum allocation_algorithm aalgorithm;
    int size;
    void* memory;
    int entries;
    
    //global cariables which will contain the free and allocated lists
    struct nodeStruct *free_list;
    struct nodeStruct *aloc_list;
};

struct KAllocator kallocator;


void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm) {
    assert(_size > 0);
    kallocator.aalgorithm = _aalgorithm;
    kallocator.size = _size;
    kallocator.entries = 0;
    
    //the memory to be allocated
    kallocator.memory = malloc((size_t)kallocator.size);

    //make _size nodes with 1 block each
    kallocator.free_list = NULL;

    List_insert(&kallocator.free_list, createNode(kallocator.memory, _size));

    //no allocated memory in beginning
    kallocator.aloc_list = NULL;   
}


void destroy_allocator() {        
    //free the free_list nodes and kallocator.free_list
    if(List_countNodes(kallocator.free_list) == 1){
        free(kallocator.memory);
        free(kallocator.free_list);
    }
    else{//multiple empty nodes
        struct nodeStruct* current = kallocator.memory;
        struct nodeStruct* next = current->next;
        List_deleteNode(kallocator.free_list, current);
        current = next;
        while(current != NULL){
            next = current->next;
            List_deleteNode(kallocator.free_list, current);
            current = next;//current->next = next
        }
    }
    
    //free the aloc_list nodes
    if(List_countNodes(kallocator.aloc_list) == 0){//no nodes
        ;
    }
    else if(List_countNodes(kallocator.aloc_list) == 1){//1 node
        struct nodeStruct* one = getNth(kallocator.aloc_list, 0);
        free(one->ptr);//newAdd
        List_deleteNode(kallocator.aloc_list, one);
    }
    else{//more than 1 node
        struct nodeStruct* current = getNth(kallocator.aloc_list, 0);
        //struct nodeStruct* next = NULL;
        
        while(current != NULL){
            struct nodeStruct* next = current->next;
            free(current->ptr);
            free(current);
            current = next;
        }
    }
}

void* kalloc(int _size) {
    void* ptr = NULL;
    ptr = malloc(_size);

    switch(kallocator.aalgorithm){
        case FIRST_FIT:
            //check if blocks are available
            if((kallocator.entries + 1) > find_biggest(kallocator.free_list) ){
                printf("Cannot allocate more memory\n");
                free(ptr);
                ptr = NULL;
            }
            else{ //memory is available
                //update entries
                kallocator.entries = kallocator.entries + 1;
                //get the node that satiisfies the conditon from free list
                struct nodeStruct *fit = getCondition(kallocator.free_list, kallocator.entries);
                //same amount of blocks
                if (fit->blocks == kallocator.entries){
                    List_insert(&kallocator.aloc_list, createNode(ptr, kallocator.entries));
                
                    List_deleteNode(kallocator.free_list, fit);
                }
                else {//different amount of block sizes
                    int remainder_blocks = (fit->blocks) - kallocator.entries;
                    fit->blocks = remainder_blocks;
                    //reallocate memory
                    //fit->ptr = (void*)realloc(fit->ptr, remainder_blocks);
                    
                    //insert into aloc_list
                    List_insert(&kallocator.aloc_list, createNode(ptr, kallocator.entries));
                }
            }
            break;
        case BEST_FIT:
            //check if blocks available
            if((kallocator.entries + 1) > find_biggest(kallocator.free_list) ){
                printf("Cannot allocate more memory\n");
                free(ptr);
                ptr = NULL;
            }
            else{//memory is available
                 //update entries
                kallocator.entries = kallocator.entries + 1;
                struct nodeStruct *fit = find_best_fit(kallocator.free_list, kallocator.entries);
                //same amount of blocks
                if (fit->blocks == kallocator.entries){
                    List_insert(&kallocator.aloc_list, createNode(ptr, kallocator.entries));
                    List_deleteNode(kallocator.free_list, fit);
                }
                else{//differnt amount of block sizes
                    int remainder_blocks = (fit->blocks) - kallocator.entries;
                    fit->blocks = remainder_blocks;
                    //reallocate memory
                    //fit->ptr = (int*)realloc(fit->ptr, remainder_blocks);
                    List_insert(&kallocator.aloc_list, createNode(ptr, kallocator.entries));
                }                
            }
            break;
        case WORST_FIT:
            //check if blocks available
            if((kallocator.entries + 1) > find_biggest(kallocator.free_list) ){
                printf("Cannot allocate more memory\n");
                free(ptr);
                ptr = NULL;
            }
            else{//memory is available
                //update entries
                kallocator.entries = kallocator.entries + 1;
                struct nodeStruct* fit = getBiggest(kallocator.free_list);
                //same amount of blocks
                if(fit->blocks == kallocator.entries){
                    List_insert(&kallocator.aloc_list, createNode(ptr, kallocator.entries));
                    List_deleteNode(kallocator.free_list, fit);
                }
                else{//different amount of block sizes
                    int remainder_blocks = (fit->blocks) - kallocator.entries;
                    fit->blocks = remainder_blocks;
                    //reallocate memory
                    //fit->ptr = (int*)realloc(fit->ptr, remainder_blocks);
                    List_insert(&kallocator.aloc_list, createNode(ptr, kallocator.entries));
                }
            }
            break;
        default: 
            printf("Algorithm not specified for kalloc\n");
    }

    return ptr;
}


void kfree(void* _ptr) {
    assert(_ptr != NULL);

    //add pointer blocks to free
    struct nodeStruct *pNode = List_findNode(kallocator.aloc_list, _ptr);
    
    //free(pNode->ptr);   
    int chunks = pNode->blocks;
    List_deleteNode(kallocator.aloc_list, pNode);
    free(_ptr);
    _ptr=NULL;


    List_insert(&kallocator.free_list, createNode(NULL, chunks));//pNode);


    //merge empty nodes
    //merge_Emps(kallocator.free_list);
}


int compact_allocation(void** _before, void** _after) {
    int compacted_size = 0;
    
    //transferring item before to after
    struct nodeStruct* present = NULL;

    for (int i = 0; i < kallocator.entries; i++){
        present = List_findNode(kallocator.aloc_list, _before[i]);
       //check if pointer in allocated list  
        if (present != NULL ){
            _after[i] = _before[i];//adder pointer to allocated list
        }
    }

    //updating the free_list to contain 1 big free block
    if(List_countNodes(kallocator.free_list) == 1){//already has 1 free block
        ;
    }
    else{//has multiple free blocks
        List_findNode(kallocator.free_list, kallocator.memory)->blocks = add_blocks(kallocator.free_list);
        //reallocate memory
        //kallocator.memory = (int*)realloc(kallocator.memory, add_blocks(kallocator.free_list));
        
        while(List_countNodes(kallocator.free_list) != 1){
            struct nodeStruct* next = getNth(kallocator.free_list, 1);
            
            List_deleteNode(kallocator.free_list, next);
            
        }
        
    }
    //update the system's view on the number of entries
    kallocator.entries = List_countNodes(kallocator.aloc_list);
    compacted_size = kallocator.entries;
    return compacted_size;
}

int available_memory() {
    int available_memory_size = 0;
    available_memory_size = add_blocks(kallocator.free_list);

    return available_memory_size;
}

void print_statistics() {
    //merge_Emps(kallocator.free_list);
    int allocated_size = add_blocks(kallocator.aloc_list);
    int allocated_chunks = List_countNodes(kallocator.aloc_list);
    int free_size = available_memory();
    int free_chunks = List_countNodes(kallocator.free_list);
    int smallest_free_chunk_size = find_smallest(kallocator.free_list);
    int largest_free_chunk_size = find_biggest(kallocator.free_list);

    printf("Allocated size = %d\n", allocated_size);
    printf("Allocated chunks = %d\n", allocated_chunks);
    printf("Free size = %d\n", free_size);
    printf("Free chunks = %d\n", free_chunks);
    printf("Largest free chunk size = %d\n", largest_free_chunk_size);
    printf("Smallest free chunk size = %d\n", smallest_free_chunk_size);
}