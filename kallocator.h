#ifndef __KALLOCATOR_H__
#define __KALLOCATOR_H__

enum allocation_algorithm {FIRST_FIT, BEST_FIT, WORST_FIT};

void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm);

void* kalloc(int _size);
void kfree(void* _ptr);
int available_memory();
void print_statistics();
int compact_allocation(void** _before, void** _after);
void destroy_allocator();

void setBlock_meta(void* ptr, int size);

//gcc -Wall -g -std=c99 -D_POSIX_C_SOURCE=199309L main.c kallocator.c -o kallocator

#endif
