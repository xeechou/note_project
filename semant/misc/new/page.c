#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "misc_types.h"
#include "utils.h"
extern void *memcpy(void *dest, const void *src, size_t n);

typedef struct {
	size_t page_size;
	size_t n_pages;
	void *curr_page;

	stack plist;	//array that for store page addresses;
} pages;


static int _pages_init(pages *p, size_t page_size)
{
	stack_init(&p->plist, sizeof(void *), 4, NULL, NULL);

	p->curr_page = xvalloc(page_size);
	p->n_pages = 1;
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
		free(fpage);
		p->n_pages -= 1;
	}
	stack_dispose(&p->plist);
}
static void *page_new(pages *p)
{
	stack_push(&p->plist, &p->curr_page);	//push first page in
	void *new_page = xvalloc(p->page_size);
	p->n_pages += 1;
	p->curr_page = new_page;

	return new_page;
}
static int page_dump(pages *p)
{
	free(p->curr_page);
	void *addr;
	if (p->n_pages <= 0)
		return -1;
	stack_pop(&p->plist, &addr);
	p->curr_page = addr;
	p->n_pages -= 1;
}

/*
#include <stdio.h>

void main(void)
{
	char *str = "Hello, world";
	pages p;
	pages_init(&p, 12, 4096, NULL);
	int i;
	void *trash;
	for (i = 0; i < 500; i++) {
		pages_insert(&p, str, &trash);
		printf("%p\t",trash);
	}
	printf("%s\n", (char *)p.curr_page);
}
*/
