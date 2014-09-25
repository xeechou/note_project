#include "misc_types.h"
#include <stdlib.h>
void *memcpy(void *dest, const void *src, size_t n);


static int pstrt_init(page_strt *p, size_t esize, size_t page_size, 
		void (* cp) (void *, const void *))
{
	int err = 0;
	p->esize = esize;
	p->page_size = page_size;
	p->elems_per_page = (unsigned int)(p->page_size / p->esize);

	stack_init(&p->pages, sizeof(void *), 4, NULL, NULL);
	if (err)
		return err;

	p->curr_page = malloc(page_size);
	stack_push(&p->pages, &p->curr_page);	//push first page in
	p->log_len = 0;
	p->n_pages = 1;
	stack_init(&p->frags, sizeof(void *), 4, NULL, NULL);
	p->cp = cp;
	return err;
}
int err_pstrt_init(page_strt *p, size_t esize, size_t page_size,
		void (* cp) (void *, const void *))
{
	if ( (esize > 0 && page_size > 0) == 0)
		return -2;
	if ( esize > page_size )
		return -2;
	return pstrt_init(p, esize, page_size, cp);
}


void pstrt_dispose(page_strt *p)
{
	void *fpage;
	int i;
	for (i = 0; i < p->n_pages; i++) {
		stack_pop(&p->pages, &fpage);
		free(fpage);
	}
	stack_dispose(&p->pages);
	stack_dispose(&p->frags);
}
static int page_new(page_strt *p)
{
	void *new_page = malloc(p->page_size);
	p->n_pages += 1;
	p->curr_page = new_page;

	stack_push(&p->pages, &new_page);
	p->log_len = 0;
	return 0;
}
int pstrt_insert(page_strt *p,  const void *elemAddr, void **ret_addr)
{
	size_t ret = 0, inc = 0;
	void *addr;
	if (stack_isempty(&p->frags)) {
		stack_pop(&p->frags, &addr);
		inc = 0;
	} else {
		if (p->log_len >= p->elems_per_page)
			ret = page_new(p);
		addr = (char *)p->curr_page + p->log_len * p->esize;
		inc = 1;
	}
	if (ret)
		return ret;
	if (p->cp)
		p->cp(addr, elemAddr);
	else
		memcpy(addr, elemAddr, p->esize);
	*ret_addr = addr;
	p->log_len += inc;
	return 0;
}
/* XXX: test functions below */
static void page_dispose(page_strt *p)
{
	free(p->curr_page);
	void *addr;
	stack_pop(&p->pages, &addr);
	p->curr_page = addr;
	p->n_pages -= 1;

	p->log_len = p->elems_per_page;
}

void pstrt_delete(page_strt *p, void *rm_addr)
{
	if (stack_find(&p->frags, rm_addr))
		return;
	else if (rm_addr == 
			(char *)p->curr_page + p->esize * (p->log_len - 1)) {
		if (p->log_len == 1)
			page_dispose(p);
		else {
			p->log_len--;
		}
	} else
		stack_push(&p->frags, &rm_addr);
}
/*
#include <stdio.h>

void main(void)
{
	char *str = "Hello, world";
	page_strt p;
	pstrt_init(&p, 12, 4096, NULL);
	int i;
	void *trash;
	for (i = 0; i < 500; i++) {
		pstrt_insert(&p, str, &trash);
		printf("%p\t",trash);
	}
	printf("%s\n", (char *)p.curr_page);
}
*/
