#include <errno.h>
#include "utils.h"
/* so we don't need to include the headers */
extern char *strerror(int errnum);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void sleep(unsigned int seconds);

void *xmalloc(size_t size)
{
	void *val = malloc(size);
	if (unlikely(val == NULL))
		val = retry_alloc(size);
	if (val == NULL)
		fatal(errno);
	return val;
}

void *xrealloc(void *ptr, size_t size)
{
	if (!ptr)
	       return NULL;	
	void *val = realloc(ptr, size);
	if (unlikely(val == NULL))
		val = retry_alloc(size);
	if (val == NULL)
		fatal(errno);
	return val;
}

void xfree(void *ptr)
{
	if (!ptr)
		return;
	free(ptr);
}

void *xvalloc(size_t size)
{
	void *val = valloc(size);
	if (unlikely(val == NULL))
		val = retry_alloc(size);
	if (val == NULL)
		fatal(errno);
	return val;
}

void *xvrealloc(void *ptr, size_t size)
{
	if (!ptr)
		return NULL;
	void *val = xvalloc(size);
	size_t cp_size = malloc_usable_size(ptr);
	cp_size = (cp_size < size) ? cp_size : size;

	memcpy(val, ptr, cp_size);
	xfree(ptr);

	return val;
}


void *retry_alloc(size_t size)
{
	void *val;
	int count = 0;
	do {
		sleep(1);
		val = malloc(size);
		if (val)
			break;
		count++;
	} while (count < 3);
	return val;
}
void fatal(int err_num)
{
	strerror(err_num);
	abort();
}
