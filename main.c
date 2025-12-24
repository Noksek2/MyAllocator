//#pragma optimize("", off)
#include "myallocator.h"


int main() {
	myallocator alc;
	allocator_new(&alc, ARENA_1MB);
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
	}

	allocator_free(&alc);
	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}
