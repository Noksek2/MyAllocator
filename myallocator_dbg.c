/*
https://github.com/Noksek2/MyAllocator Noksek2
Is there any problem in this code, use Issues please.
*/

#include "myallocator.h"

void _arena_add(myarena* arena0,myarena* arena1, myarena* arena2) {
	//arena0 > (arena1 > ... > arena2) > arena3
	myarena* arena3 = arena0->next;
	arena0->next = arena1;
	arena2->next = arena3;
}

void allocator_new(myallocator* alc) {
	size_t size = sizeof(myarena)+ARENA_MAX;
	alc->head = (myarena*)calloc(size,1);
	if (alc->head == 0) return;
	alc->current = alc->head;
	alc->arena_size = size;

	alc->head->len = 0;
	alc->head->capa = size;
	alc->head->next = 0;
	printf("new allocator [%X](%lld)\n", alc->head, size);
}
void allocator_new_size(myallocator* alc,size_t arena_size) {
	arena_size += sizeof(myarena);
	alc->head = (myarena*)calloc(sizeof(myarena) + arena_size, 1);
	if (alc->head == 0) return;
	alc->current = alc->head;
	alc->arena_size = arena_size;
	alc->head->len = 0;
	alc->head->capa = arena_size;
	alc->head->next = 0;
	printf("new allocator [%X](%lld)\n", alc->head, arena_size);
}
void* allocator_alloc(myallocator* alc, size_t len) {
	myarena* arena = alc->current;
	void* ptr = arena->ptr + arena->len;
	if (len == 0)return 0;

	size_t padding = ((len - 1) / sizeof(size_t) + 1) * sizeof(size_t);
	printf("try[%X] alloc(%d,%d)... ", arena, len, padding);
	if (arena->len + len <= arena->capa) {
		printf("Done [%X]\n", arena->ptr + arena->len);
		arena->len += padding;
		return ptr;
	}
	else if(arena->next!=NULL){
		if (len <= arena->next->capa) {
			alc->current = arena->next;
			printf("Enable next block [%X]\n", alc->current);
			return arena->next;
		}
	}

	//arena->next ==0, arena fulled
	if (len <= alc->arena_size) {
		arena->next = (myarena*)calloc(1, alc->arena_size);
		if (arena->next == NULL)return NULL;
		alc->current = arena->next;
		alc->current->len = padding;
		printf("full! new allocator [%X], Done [%X]\n", alc->current, alc->current->ptr);
		return alc->current->ptr;
	}
	else {
		arena = (myarena*)calloc(1, padding);
		if (arena == NULL)return NULL;
		arena->capa = padding;
		arena->len = padding;
		arena->next = alc->head;
		alc->head = arena;
		printf("full! too big! Done [%X]\n", alc->head);
		return arena->ptr;
	}

	//arena_alloc(alc->head, len);
}
void allocator_reset(myallocator* alc) {
	alc->head->len = 0;
	alc->current = alc->head;
	//arena_alloc(alc->head, len);
}
void allocator_rewind(myallocator* alc, myarena_check* checkpoint) {
	myarena* head;
	myarena* tail;

	printf("before : ");
	for (head = alc->head; head != 0; head = head->next) {
		printf("[%X] ", head);
	}
	printf("\n");

	head = checkpoint->tail;
	head->len = checkpoint->len;
	head = head->next;

	while (head != 0 && head != alc->current) {
		head->len = 0;
		head = head->next;
	}
	alc->current = checkpoint->tail;



	head = alc->head;
	tail = 0;
	while (alc->head != checkpoint->head) {
		alc->head->len = 0;
		tail = alc->head;
		alc->head = alc->head->next;
	}
	if (tail != 0)
		_arena_add(checkpoint->tail, head, tail);
	
	printf("rewind : ");
	for (head = alc->head; head != 0; head = head->next) {
		printf("[%X] ", head);
	}
	printf("\n");
}
void allocator_free(myallocator* alc) {
	myarena* next = 0;
	myarena* now = alc->head;
	while (now != NULL) {
		next = now->next;
		free(now);
		printf("free block [%X]..\n", now);
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
