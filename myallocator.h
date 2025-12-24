
/*
https://github.com/Noksek2/MyAllocator Noksek2 v0.1.1
Is there any problem in this code, use Issues please.
*/

#pragma once

#ifndef __MYALLOCATOR_H__
#define __MYALLOCATOR_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define ARENA_1MB (1024*1024-sizeof(myarena))
#define ARENA_4MB (1024*1024*4-sizeof(myarena))
#ifdef _DEBUG
extern size_t g_id;
#endif
typedef uint32_t mysize_t;
typedef struct myarena {
	struct myarena* next;
#ifdef _DEBUG
	size_t id;
#endif
	mysize_t len;
	mysize_t capa;
	uint8_t ptr[];
}myarena;

typedef struct myarena_check {
	myarena* tail;
	myarena* head;
	mysize_t len;
}myarena_check;

typedef struct myallocator {
	myarena* head;
	myarena* current;
	mysize_t arena_size;
}myallocator;
extern void allocator_new(myallocator* alc, mysize_t arena_size);
extern void* allocator_alloc(myallocator* alc, mysize_t len);
extern void allocator_free(myallocator* alc);
extern void allocator_reset(myallocator* alc);
extern myarena_check arena_check_new(myallocator* alc);
extern void allocator_rewind(myallocator* alc, myarena_check* checkpoint);

#ifdef __USE_CLEAN__
//dont use it very slow.
//extern void allocator_clean(myallocator* alc,myarena_check* checkpoint);

#endif

#endif