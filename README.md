# MyAllocator
simple arena allocator in C

## v0.1.2

## Implementation explanation

## Structure
```c
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
```
## Functions
- void allocator_new(myallocator* alc, mysize_t arena_size);
- void* allocator_alloc(myallocator* alc, mysize_t len);
- void allocator_free(myallocator* alc);
- void allocator_reset(myallocator* alc);
- void allocator_check(myallocator* alc, myarena_check* checkpoint);
- void allocator_rewind(myallocator* alc, myarena_check* checkpoint);
- void* allocator_realloc(myallocator* alc, void* p, mysize_t old_capa, mysize_t new_capa);

- myarena_check arena_check_new(myallocator* alc);

Defined in `myallocator.h`

## Code Example (main.c)
```c
#include "myallocator.h"
int main() {
	myallocator alc;
	allocator_new(&alc, ARENA_1MB);
	int* arr;
	myarena_check check;
	for (size_t i = 0; i < 300; i++) {
		if (i % 3 == 0)
			//check = arena_check_new(&alc);
			allocator_check(&alc, &check);
		arr = allocator_alloc(&alc, ARENA_1MB/2);
		*arr = i;
		printf("vm[%d]=%d\n", i, *arr);
		if (i % 3 == 2)
			allocator_rewind(&alc, &check);
	}

	allocator_free(&alc);
	
	//msvc
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}
```

## NOTICE
This code is stable on Windows 11 x64 (MSVC).
It has not been verified on Linux, etc.

## License 
- **apache license 2.0**
