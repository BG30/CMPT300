#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"
#include <stdbool.h>

struct nodeStruct* createNode(int *ptr, int chunks){
    struct nodeStruct *pNode = malloc(sizeof(struct nodeStruct));
    
    pNode->ptr = ptr;
    pNode->blocks = chunks;

    return pNode;
}

void List_insert(struct nodeStruct **headRef, struct nodeStruct *node){
    node->next = NULL;

    if( *headRef == NULL){
        *headRef = node;
    }
    //handle empty list
    else{
        //find tail and insert
        struct nodeStruct *current = *headRef;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = node;
    }
}

void List_insertHead(struct nodeStruct *headref, struct nodeStruct *node){
    node->next = headref;
    headref = node;
}

int List_countNodes(struct nodeStruct *head){
    int count = 0;
    struct nodeStruct *current = head;
    while(current != NULL){
        current = current->next;
        count++;
    }
    return count;
}

struct nodeStruct* List_findNode(struct nodeStruct *head, int *ptr){
    struct nodeStruct *current = head;
    while(current != NULL){
        if(current->ptr == ptr){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void List_deleteNode(struct nodeStruct *headRef, struct nodeStruct *node){
    if (headRef == node)
        headRef = node->next;
    
    else{
        struct nodeStruct *previous = headRef;
        while(previous->next != node){   
            previous = previous->next;
        }
        previous->next = node->next;    
    }
    //free(node->ptr);
    free(node);
}

struct nodeStruct* getNth(struct nodeStruct *head, int num){
	struct nodeStruct *current = head;
	for (int i = 0; i < num; i++){
		current = current->next;
	}
	return current;
}


int find_smallest(struct nodeStruct *head){
    struct nodeStruct *current = head;
    int smallest = current->blocks;
    
    while(current != NULL){
        if(current->blocks < smallest)
            smallest = current->blocks;
        
        current = current->next;
    }

    return smallest;
}

int find_biggest(struct nodeStruct *head){
    struct nodeStruct *current = head;
    int smallest = current->blocks;

    while(current != NULL){
        if(current->blocks > smallest)
            smallest = current->blocks;
        
        current = current->next;
    }

    return smallest;
}

struct nodeStruct* find_best_fit(struct nodeStruct *head, int size){
    struct nodeStruct* current = head;
    struct nodeStruct* best = NULL;

    while(current != NULL){
        if(current->blocks == size){//exact match
            best = current;
            return best;
        }
        else if(current->blocks > size ){
            best = current;
            current = current->next;
        }
        else{
            current = current->next;
        }
    }
    printf("best is %d\n", best->blocks);
    return best;
}

struct nodeStruct* getBiggest(struct nodeStruct *head){
    struct nodeStruct* current = head;

    while(head != NULL){
        if (head->blocks > current->blocks){
            current = head;
        }
        head = head->next;        
    }

    return current;
}

int add_blocks(struct nodeStruct *head){
    struct nodeStruct *current = head;
    int sum = 0;

    while(current != NULL){
        sum = sum + current->blocks;
        current = current->next;
    }

    return sum;
}


//returns the next node that minimally satisfies the condition
struct nodeStruct* getCondition(struct nodeStruct *head, int condition){
    struct nodeStruct *current = head;
    
    for (int i = 0; i < List_countNodes(current); i++){
        if (current->blocks >= condition){
            return current;
        }
        current = current->next;
    }
    return current;
}


struct nodeStruct* getMatch(struct nodeStruct *head, int block){
    struct nodeStruct *current = head;
    for (int i = 0; i < List_countNodes(head); i++){
        if(current->blocks == block){
            return current;
        }
        current = current->next;
    }
    return NULL;
}



//returns smallest node or continuous nodes
void merge_Emps(struct nodeStruct *head){
    if(List_countNodes(head) == 1){//only 1 empty node
        ;
    }
    
    else{//more than 1 empty node
        struct nodeStruct* current = head;
        struct nodeStruct* next = NULL;
        int size = current->blocks;

        while(current != NULL){//check if end of list     
            size = current->blocks;
            next = getMatch(head, size + 1);
            
            while(next != NULL){//check if consecutive free block
                current->blocks = current->blocks + next->blocks;
                List_deleteNode(head, next);

                next = getMatch(head, size + 1);
            }
            
            current = current->next;
        }
    
    }
}