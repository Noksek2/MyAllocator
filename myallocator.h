
/*
https://github.com/Noksek2/MyAllocator Noksek2 v0.1.1
Is there any problem in this code, use Issues please.
*/

#pragma once

#ifndef __MYALLOCATOR_H__
#define __MYALLOCATOR_H__

#ifndef RESTRICT
#if defined(_MSC_VER)
#define RESTRICT __restrict  // MSVC용
#elif defined(__GNUC__)
#define RESTRICT __restrict  // GCC/Clang용
#else
#define RESTRICT restrict    // 표준 C용
#endif
#endif
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define ARENA_1MB (1024*1024-sizeof(myarena))
#define ARENA_4MB (1024*1024*4-sizeof(myarena))

#ifndef max
#define max(A,B) ((A>B)?(A):(B))
#endif
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
#ifdef __cplusplus
extern "C" {
#endif
	extern void allocator_new(myallocator* alc, mysize_t arena_size);
	extern void* allocator_alloc(myallocator* alc, mysize_t len);
	extern void allocator_free(myallocator* alc);
	extern void allocator_reset(myallocator* alc);
	extern myarena_check arena_check_new(myallocator* alc);
	extern void allocator_rewind(myallocator* alc, myarena_check* checkpoint);

	extern void* allocator_realloc(myallocator* alc, void* p, mysize_t old_capa, mysize_t new_capa);

#ifdef __USE_CLEAN__
	//dont use it very slow.
	//extern void allocator_clean(myallocator* alc,myarena_check* checkpoint);

#ifdef __cplusplus
}
#endif

#endif

#endif