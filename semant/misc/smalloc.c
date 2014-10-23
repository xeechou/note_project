#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "misc_types.h"
/* salloc is actually a stack, only it allows to append and pop numbers of
 * element at once */

/* we call it smalloc coz we hope we won't use the grow function, smalloc
 * tries to alloc enought memory at once
 */


void smm_init(smmblk *s, ind_t esize, ind_t init_alloc, ind_t sec_size,
		void (*func) (void *))
{
	while ( (s->elems = malloc(init_alloc * esize)) 
			== NULL) {
		init_alloc /= 2;
		assert(init_alloc >= sec_size);
	}
	s->esize = esize;
	s->log_len = 0;
	s->alloc_len = init_alloc;
	s->sec_size = sec_size;
	s->func = func;
}

void smm_dispose(smmblk *s)
{
	if (s->func) {
		while(s->log_len > 0)
			s->log_len--;
			s->func ((char *) s->elems + s->esize * s->log_len);
	}
	free(s->elems);
}

static void smm_grow(smmblk *s, ind_t size)
{
	do {
		s->alloc_len *= 2;
	} while(s->alloc_len <= size);

	s->elems = realloc(s->elems, s->alloc_len * s->esize); 
	assert(s->elems);
}

void smm_append(smmblk *s, void *elem_addr, void **ret_addr, ind_t num)
{
	ind_t size = s->log_len + num;
	if (size >= s->alloc_len)
		smm_grow(s, size);
	*ret_addr = (char *)s->elems + s->esize * s->log_len;
	memcpy(*ret_addr, elem_addr, s->esize * num);
	s->log_len = size;
}

/* move pointer backwards, return address, copy it if you need */
void smm_delete(smmblk *s, void **ret_addr, ind_t num)
{
	assert(num <= s->log_len);
	s->log_len -= num;
	*ret_addr = (char *) s->elems + s->log_len * s->esize;
}
/* just like smm_delete, only it copy data for you, so you need prepare memory
 * for it.*/
void smm_pop(smmblk *s, void *ret_addr, ind_t num)
{
	assert(num <= s->log_len);
	s->log_len -= num;
	memcpy(ret_addr,
		(char *) s->elems + s->log_len * s->esize,
		s->esize * num);
}
/*
#include <stdio.h>
char *st= "This is a test.";
int main()
{
	FILE *f = fopen("smalloc.c", "r");
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);

	smmblk s;
	smm_init(&s, sizeof(char), 2*len, len, NULL);
	int i;
	void *trash;
	for (i = 0; i < 100; i++) {
		smm_append(&s, st, &trash, strlen(st) + 1);
		smm_delete(&s, &trash, 1);
	}
	char a = '\0';
	smm_append(&s, &a,&trash, 1);
	printf("%s", s.elems);
}
*/
