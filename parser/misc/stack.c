#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "misc_types.h"

void stack_init(stack *s, size_t esize,  size_t init_alloc, 
		int (*cmp) (const void *, const void *),
		void (*func)(void *))
{
	s->elems = (void *) malloc(esize * init_alloc);
	assert(s->elems != NULL);
	s->esize = esize;
	s->log_len = 0;
	s->alloc_len = init_alloc;
	s->cmp = cmp;
	s->func = func;
}

void stack_dispose(stack *s)
{
	if (s->func) {
		while(s->log_len > 0) {
			s->log_len--;
			s->func ((char *) s->elems + s->esize * s->log_len);
		}
	}
	free(s->elems);
}

int stack_find(stack *s, const void *elemaddr)
{
	int cmp_val;
	
	size_t len = s->log_len;
	char *addr;
	while (len > 0) {
		len--;
		addr = (char *)s->elems + s->esize * len;
		if(s->cmp)
			cmp_val = s->cmp(elemaddr, addr);
		else  
			cmp_val = memcmp(elemaddr,addr, s->esize);
		if (cmp_val == 0)
			return 1;
	}
	return 0;
}

static void stack_grow(stack *s)
{
	s->alloc_len *= 2;
	s->elems = realloc(s->elems,
			s->esize * s->alloc_len);
	assert(s->elems != NULL);
}

int stack_isempty(stack *s) { return s->log_len; };

void stack_pop(stack *s, void *ret_addr)
{
	assert(s->log_len > 0);
	s->log_len--;
	memcpy(ret_addr, (char *)s->elems +
			 s->log_len * s->esize,
			 s->esize);
}

void stack_push(stack *s, const void *elem_addr)
{
	if (s->log_len == s->alloc_len)
		stack_grow(s);
	memcpy((char *) s->elems + 
		s->log_len * s->esize,
		elem_addr,
		s->esize);
	s->log_len++;
}
/*
#include <stdio.h>
size_t main()
{
	stack s;
	stack_init(&s, sizeof(size_t), 4, NULL, NULL);
	size_t i, a= 3;
	for (i = 0; i < 200; i++)
		stack_push(&s, &a);
	size_t b;
	for (i = 0; i < 200; i++) {
		stack_pop(&s, &b);
		printf("%d", b);
	}
	putchar('\n');
	stack_dispose(&s);
}
*/
