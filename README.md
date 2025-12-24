# MyAllocator
simple arena allocator in C

## UNSTABLE (v0.1.1)

## Implementation explain


## Functions
- allocator_new
- arena_alloc
- arena_free
- arena_reset
- arena_rewind
- arena_check_new


`myallocator.h`
```
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
#ifdef _DEBUG //It's just debug macro
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
```
## Code Example (main.c)
```

int main() {
	myallocator alc;
	allocator_new(&alc, ARENA_1MB);

	//nok_data *vm_reg;
	int* arr;
	myarena_check check;
	for (size_t i = 0; i < 300; i++) {
		if (i % 3 == 0)
			check = arena_check_new(&alc);
		arr = allocator_alloc(&alc, ARENA_1MB/2);
		*arr = i;
		printf("vm[%d]=%d\n", i, *arr);
		if (i % 3 == 2)
			allocator_rewind(&alc, &check);
		if (i % 100 == 0)
			allocator_reset(&alc);
	}

	allocator_free(&alc);
	_getch();
}
```

## License 
- **apache license 2.0**
