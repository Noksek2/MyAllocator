
/*
https://github.com/Noksek2/MyAllocator Noksek2
Is there any problem in this code, use Issues please.
*/

#pragma once

#ifndef __MYALLOCATOR_H__
#define __MYALLOCATOR_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define ARENA_MAX 1024*1024


typedef struct myarena{
	struct myarena* next;
	size_t len:32;
	size_t capa : 32;
	uint8_t ptr[];
}myarena;

typedef struct myarena_check {
	myarena* tail;
	myarena* head;
	size_t len;
}myarena_check;

typedef struct myallocator {
	myarena* head;
	myarena* current;
	size_t arena_size;
}myallocator;
extern void allocator_new(myallocator* alc);
extern void allocator_new_size(myallocator* alc, size_t arena_size);
extern void* allocator_alloc(myallocator* alc, size_t len);
extern void allocator_free(myallocator* alc);
extern void allocator_reset(myallocator* alc);
extern myarena_check arena_check_new(myallocator* alc);
extern void allocator_rewind(myallocator* alc, myarena_check* checkpoint);

#ifdef __USE_CLEAN__
//dont use it very slow.
//extern void allocator_clean(myallocator* alc,myarena_check* checkpoint);

#endif



//extern void* allocator_huge_alloc(myallocator* alc);
#endif