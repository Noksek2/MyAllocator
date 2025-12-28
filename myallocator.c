/*
https://github.com/Noksek2/MyAllocator Noksek2 v0.1.1
Is there any problem in this code, use Issues please.
*/
#include "myallocator.h"

#ifdef _DEBUG
size_t g_id = 0u;
#endif
void allocator_new(myallocator* alc, mysize_t arena_size) {
	//new allocator - arena lists

	myarena* arena = (myarena*)calloc(arena_size + sizeof(myarena), 1);
	if (arena == NULL) return;

	//init arena.
	//arena->next = 0;
	//arena->len = 0;
	arena->capa = arena_size;

	alc->arena_size = arena_size;
	alc->current = arena;
	alc->head = arena;

#ifdef _DEBUG
	alc->head->id = g_id++;
	printf("new allocator [%X](%lld)\n", alc->head, arena_size);
#endif
}

void* allocator_alloc(myallocator* alc, mysize_t append_len) {
	/*
	1. just move pointer (arena->len + append_len <= arena->capa)
	2-1. next ptr (=<next->capa)
	2-2. new ptr (new_arena->next=arena->next, arena->next=new_arena)

	*/
	if (append_len == 0)return 0;
	void* ptr = NULL;
	myarena* arena = alc->current;
	mysize_t padding = ((append_len - 1) / sizeof(size_t) + 1) * sizeof(size_t);
#ifdef _DEBUG
	printf("try id:%llu [%X] alloc(%d,%d)... ", arena->id, arena, append_len, padding, arena->capa);
#endif
	if (arena->len + append_len <= arena->capa) {
#ifdef _DEBUG
		printf("Done [%X]\n", arena->ptr + arena->len);
#endif
		ptr = arena->ptr + arena->len;
		arena->len += padding;
	}
	else if (arena->next != NULL) {
		if (append_len <= arena->next->capa) {
			arena->next->len = 0u;
			alc->current = arena->next;
			ptr = alc->current->ptr;
#ifdef _DEBUG
			printf("Enable next block id:%llu [%X]\n", alc->current->id, alc->current);
#endif
		}
	}
	//arena->next ==0 || append_len > arena->next->capa 
	if (ptr==NULL) {
		mysize_t new_size = max(alc->arena_size, padding);
		myarena* new_arena = (myarena*)calloc(1, new_size + sizeof(myarena));
		if (new_arena == NULL)return NULL;
		new_arena->len = padding;
		new_arena->capa = new_size;
		new_arena->next = arena->next;
		arena->next = new_arena;
		alc->current = new_arena;
		ptr = alc->current->ptr;
#ifdef _DEBUG
		alc->current->id = g_id++;
		printf("full! new allocator id:%llu [%X], Done [%X]\n", alc->current->id, alc->current, alc->current->ptr);
#endif
	}
	return ptr;
	//arena_alloc(alc->head, len);
}
void allocator_reset(myallocator* alc) {
	alc->head->len = 0;
	alc->current = alc->head;
}
void allocator_rewind(myallocator* alc, myarena_check* checkpoint) {
#ifdef _DEBUG
	printf("before : ");

	for (myarena* head = alc->head; head != 0; head = head->next) {

		printf("[%llu:%u/%u] ", head->id, head->len, head->capa);

	}

	printf("\n");
#endif

	alc->current = checkpoint->tail;
	alc->current->len = checkpoint->len;

#ifdef _DEBUG
	printf("rewind : ");

	for (myarena* head = alc->head; head != 0; head = head->next) {

		printf("[%llu:%u/%u] ", head->id, head->len, head->capa);

	}


	printf("\n");
#endif
}
void allocator_free(myallocator* alc) {
	myarena* next = 0;
	myarena* now = alc->head;
	while (now != NULL) {
		next = now->next;
#ifdef _DEBUG
		printf("free block id:%llu [%X]..\n", now->id, now);
#endif
		free(now);
		now = next;
	}
	alc->head = 0;
	alc->current = 0;
}

myarena_check arena_check_new(myallocator* alc) {
	myarena_check ret = {
		.head = alc->head,
		.tail = alc->current,
		.len = alc->current->len,
	};
	return ret;
}
void* allocator_realloc(myallocator* alc, void* p, mysize_t old_capa, mysize_t new_capa) {
	uint8_t* arena_ptr = (alc->current->ptr + alc->current->len);

	if ((arena_ptr == (uint8_t*)p + old_capa) && (new_capa <= alc->current->capa)) {
		return arena_ptr;
	}
	return allocator_alloc(alc, new_capa);
}