#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <malloc.h>
#ifndef likely
#define likely(x)      __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x)    __builtin_expect(!!(x), 0)
#endif

/* malloc function family */
void *xmalloc(size_t);
void *xrealloc(void *, size_t);
void xfree(void *);

void *xvalloc(size_t);	/* page allocations */
void *xvrealloc(void *, size_t);
void *xalloca(size_t);


/* debug routines */


/* when allocating fails */
void *retry_alloc(size_t);

/* general fatal function */
void fatal(int err_num);

#endif /* utils.h */
