#include <errno.h>
#include "utils.h"

int main()
{
	void *a = xmalloc(4096);
	void *b = xrealloc(a, 2048);
	printf("%p\t%p\n", a, b);

	void *c= xvalloc(8192);
	void *d = xvrealloc(c, 0xfffffffffff);
	printf("%p\t%p\n", c, d);
}
