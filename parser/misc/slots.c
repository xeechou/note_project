#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "misc_types.h"

static int stack_cmp(const void *a, const void *b)
{
	return *(int *)a != *(int *)b;
}
void  slots_init(slots *s, size_t esize,
		 size_t init_alloc, 
		 //int (*cmp)(const void *, const void *),
		 void (*func)(void *))
{
	s->elems = (void *) malloc (esize * init_alloc);
	assert(s->elems != NULL);
	s->esize = esize;
	s->log_len = 0;
	s->alloc_len = init_alloc;
	s->func = func;
	/* in slots, we don't need to compare stack elems */	
	stack_init(&s->frags, sizeof(size_t), 4, &stack_cmp, NULL);
}

void  slots_dispose(slots *s)
{
	if (s->func) {
		while (s->log_len > 0) {
			s->log_len--;
			if (!stack_find(&s->frags, (void *) &s->log_len))
				s->func ( (char *) s->elems + s->esize * s->log_len);
		}
	}
	free(s->elems);
}
size_t   slots_length(const slots *s) {return s->log_len;}

inline void *slots_nth(slots *s, size_t pos) 
{
	return (stack_find(&s->frags, &pos)) ?
		NULL :
		(char *)s->elems + pos * s->esize;
}

static void slots_grow(slots *s)
{
	s->alloc_len *= 2;
	s->elems = realloc(s->elems,
			s->esize * s->alloc_len);
	assert(s->elems != NULL);
}
/* since it is slots, we will not allow you to insert anywhere you want */
void *slots_insert(slots *s, const void *elemAddr)
{
	size_t slot;
	size_t inc = 0;
	if (stack_isempty(&s->frags)) {
		stack_pop(&s->frags, &slot);
		inc = 0;
	}
	else {
		slot = s->log_len;
		if (s->log_len == s->alloc_len)
			slots_grow(s);
		inc = 1;
	}
	void *mvAddr = (char *)s->elems + slot * s->esize;
	memcpy(mvAddr, elemAddr, s->esize);
	s->log_len += inc;
	return mvAddr;
}

void  slots_replace(slots *s, const void *elemAddr, size_t pos)
{
	assert (pos < s->log_len);
	if (stack_find(&s->frags, &pos))
		return;
	else
		memcpy((char *)s->elems + pos * s->esize,
			elemAddr,
			s->esize);
}
/* there is three case, one, you delete a empty slot,
 * 				you delete the tail,
 * 				you delete some place */
/* I will just give you the address, you should copy it yourself, otherwise you may
 * lose your data */
size_t slots_getpos(slots *s, void *addr)
{
	assert(addr >= s->elems);
	return ( (char *)addr - (char *)s->elems) / s->esize;
}
void  slots_delete(slots *s, const size_t pos, void **ret_addr)
{
	void *addr;// = &pos;
	assert(s->log_len > 0 && pos < s->log_len);

	if (stack_find(&s->frags, &pos))
		addr = NULL;
	else if (pos == s->log_len -1)
		addr = (char *)s->elems + --(s->log_len) * s->esize;
	else {
		addr = (void *)&pos;
		stack_push(&s->frags, (const void *)addr);
	}
	*ret_addr = addr;
}
void slots_pop(slots *s, void *ret_addr)
{
	assert(s->log_len > 0);
	s->log_len--;
	memcpy(ret_addr, (char *)s->elems + 
			  s->log_len * s->esize, s->esize);
}
/*
#include <stdio.h>
int main()
{
	slots s;
	slots_init(&s, sizeof(int), 4, NULL);
	int i, a = 3;
	void *c;
	for (i = 0; i < 200; i++) {
		c = slots_insert(&s, &a);
		printf("%p\n", c);
	}
	putchar('\n');
	int b = 5;
	slots_delete(&s, 65, &c);
	slots_delete(&s, 75, &c);
	slots_delete(&s, 98, &c);
	slots_insert(&s, &b);
	slots_insert(&s, &b);
	slots_insert(&s, &b);
	for (i = 0; i < 200; i++) {
		slots_pop(&s, &a);
		printf("%d", a);
	}
	putchar('\n');
	slots_dispose(&s);
}
*/
