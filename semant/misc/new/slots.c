#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "misc_types.h"
#include "utils.h"

static int _pages_init(pages *p, size_t page_size)
{
	stack_init(&p->plist, sizeof(void *), 4, NULL, NULL);
	p->page_size = page_size;

	p->n_pages = 0;
	page_new(p);
	return 0;
}
int pages_init(pages *p, size_t page_size)
{
	if (!(page_size >= getpagesize()))
		return -2;
	return _pages_init(p, page_size);
}


void pages_dispose(pages *p)
{
	void *fpage;
	while (p->n_pages > 0) {
		stack_pop(&p->plist, &fpage);
		xfree(fpage);
		p->n_pages -= 1;
	}
	stack_dispose(&p->plist);
}
static void *page_new(pages *p)
{
	void *new_page = xvalloc(p->page_size);
	p->n_pages += 1;
	p->curr_page = new_page;
	stack_push(&p->plist, &p->curr_page);

	return new_page;
}
static void _page_dump(pages *p)
{
	//remove current page from stack
	void *addr;
	stack_pop(&p->plist, &addr);
	p->n_pages -= 1;

	xfree(p->curr_page);
	if (p->n_pages <= 0)
		addr = NULL;
	p->curr_page = addr;
	return;
}
int page_dump(pages *p)
{
	if (p->n_pages <= 0)
		return -1;
	else
		_page_dump(p);
	return 0;
}
/* return 0 ~ n-1 page base address */
void *pages_nth(pages *p, size_t i)
{
	return *(void **)stack_nth(&p->plist, i); 
}


static int stack_cmp(const void *a, const void *b)
{
	return *(char **)a != *(char **)b;
}

static void  _slots_init(slots *s, size_t esize,
		 void (*func)(void *))
{

	s->esize = esize;
	s->log_len = 0;
	s->page_log_len = 0;
	s->func = func;


	/* in slots, we don't need to compare stack elems */	
	pages_init(&s->elems, MAGIC_PAGE_SIZE);
	s->elems_per_page = (unsigned int)(s->elems.page_size / s->esize);
	stack_init(&s->frags, sizeof(void *), 4, &stack_cmp, NULL);
}

int slots_init(slots *s, size_t esize,
		 void (*func)(void *))
{
	if (MAGIC_PAGE_SIZE <= esize)
		return -1;

	_slots_init(s, esize, func);
	return 0;
}
static void clear_page( slots *s, void *base) 
{

	void *addr;
	size_t limit = s->page_log_len;
	size_t esize = s->esize;

	while (limit > 0) {
		limit--;
		addr = (char *)base + limit * esize;
		if (!stack_find(&s->frags, &addr) )
			s->func(addr);
	}
}
void  slots_dispose(slots *s)
{
	while (s->elems.curr_page) {
		if (s->func)
			clear_page(s, s->elems.curr_page);
		page_dump(&s->elems);
		//rest pages are assume fully used
		s->log_len -= s->page_log_len;
		s->page_log_len = s->elems_per_page;	
		}
}

//size_t   slots_length(const slots *s) {return s->log_len;}

void *slots_nth(slots *s, size_t pos) 
{
	char *base = (char *)pages_nth(&s->elems, pos / s->elems_per_page);
	size_t i = (pos % s->elems_per_page);

	if (base) {
		return (stack_find(&s->frags, &pos)) ?
			NULL :
			base + i * s->esize;
	} else 
		return NULL;
}

static void slots_grow(slots *s)
{
	s->page_log_len = 0;
	page_new(&s->elems);
}

/* the insert function will prepare an empty slot for you */
void *slots_insert(slots *s, const void *elemAddr)
{
	void *slot;
	size_t inc = 0;
	if (stack_isempty(&s->frags)) {		//use fragments first
		stack_pop(&s->frags, &slot);
		inc = 0;
	}
	else {
		if (s->page_log_len >= s->elems_per_page)
			slots_grow(s);
		slot = (char *)s->elems.curr_page + 
			s->page_log_len * s->esize;
		inc = 1;
	}
	memcpy(slot, elemAddr, s->esize);
	s->page_log_len += inc;
	s->log_len += inc;
	return slot;
}

int slots_replace_safe(slots *s, const void *from, size_t pos)
{
	void *to = slots_nth(s, pos);
	if (to != NULL)
		return slots_replace(s, from, to);
	else
		return -1;
}
/* DO NOT invoke slots_replace directly unless you know what you are doing */
int slots_replace(slots *s, const void *from, void *to)
{
	if ( stack_find(&s->frags, (const void *)&to) )
		return -1;
	else
		memcpy(to, from, s->esize);
	return 0;
}
size_t slots_getpos(slots *s, void *addr)
{
	size_t npages = s->elems.n_pages;
	int i, hitted = 0; char *base;

	// check the where the addr is located 
	for (i = npages; i > 0;) {
		i--;
		base = (char *)pages_nth(&s->elems, i);

		if ((char *)addr >= base  &&
		    (char *)addr < base + s->elems.page_size)
			hitted = 1;
			break;
	}
	if (hitted)
		return ( (char *)addr - (char *)base ) / s->esize;
	else
		return TOOLARGE;
}

int slots_delete_safe(slots *s, const size_t pos, void *ret_addr)
{
	void *from = slots_nth(s, pos);
	if (from != NULL) {
		slots_delete(s, from, ret_addr);
		return 0;
	}
	else
		return -1;

}
static int slot_islast(slots *s, const void *addr)
{
	void *last_elem_addr = (char *)s->elems.curr_page + 
				(s->elems_per_page - 1) * s->esize;
	return ((addr == last_elem_addr) ? 1 : 0);
}
void  slots_delete(slots *s, void *del_from, void *ret_addr)
{
	if (stack_find(&s->frags, &del_from))
		del_from = NULL;

	else if (slot_islast(s, del_from)) { 
		slots_pop(s, ret_addr);
	}
	else {
		stack_push(&s->frags, (const void *)&del_from);
	}
	memcpy(ret_addr, del_from, s->esize);
}

static void slots_shrink(slots *s)
{
	s->log_len--;
	if (s->page_log_len == 0) {
		page_dump(&s->elems);
		s->page_log_len = s->elems_per_page;
	} else
		s->page_log_len--;
}
void slots_pop(slots *s, void *ret_addr)
{
	if (s->log_len == 0)
		return;
	slots_shrink(s);
	void *from = (char *)s->elems.curr_page + 
			s->page_log_len * s->esize;
	memcpy(ret_addr, from, s->esize);
}
/*
#include <stdio.h>
int main()
{
	slots s;
	slots_init(&s, sizeof(int), NULL);
	int i, a = 3;
	int c;
	void *z;
	for (i = 0; i < 200; i++) {
		z = slots_insert(&s, &a);
		printf("%p\n", z);
	}
	putchar('\n');
	int b = 5;
	slots_delete_safe(&s, 65, (void *)&c);
	printf("%d\n", c);
	slots_delete_safe(&s, 75, (void *)&c);
	printf("%d\n", c);
	slots_delete_safe(&s, 98, (void *)&c);
	printf("%d\n", c);
	slots_insert(&s, &b);
	slots_insert(&s, &b);
	slots_insert(&s, &b);
	for (i = 0; i < 200; i++) {
		slots_pop(&s, &a);
		printf("%d", a);
	}
	putchar('\n');
	slots_dispose(&s);
	return 0;
}
*/
