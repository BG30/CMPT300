//#ifndef MYLIST_H_
#define MYLIST_H_

struct nodeStruct{
    int *ptr;
    int blocks;
    struct nodeStruct *next;
};

struct nodeStruct* createNode(int *ptr, int chunks);

void List_insert(struct nodeStruct **headRef, struct nodeStruct *node);

int List_countNodes(struct nodeStruct *head);

int find_smallest(struct nodeStruct *head);

struct nodeStruct* find_best_fit(struct nodeStruct *head, int size);

struct nodeStruct* List_findNode(struct nodeStruct *head, int *ptr);

void List_deleteNode(struct nodeStruct *headRef, struct nodeStruct *node);

struct nodeStruct* getNth(struct nodeStruct *head, int num);

struct nodeStruct* getBiggest(struct nodeStruct *head);

int find_biggest(struct nodeStruct *head);

int add_blocks(struct nodeStruct *head);

struct nodeStruct* getCondition(struct nodeStruct *head, int condition);

void merge_Emps(struct nodeStruct *head);

struct nodeStruct* getMatch(struct nodeStruct *head, int block);