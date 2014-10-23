#include <stdlib.h>
#include <assert.h>
typedef int bool;
#define TRUE 	1
#define FALSE   ~TRUE
/* A C vector is just a growing array, since we can index it, iterate it...*/
typedef int (*VecCmpF) (const void *elemAddr1,
			const void *elemAddr2);
typedef void (*VecFreeF) (void *elemAddr);
typedef void (*VecMapF) (void *elemAddr, void *auxData);

typedef struct {
	void *elems;
	int elemsize;
	int log_len;
	int alloc_len;
	void (*func) (void *);
} vector;

int  vector_init(vector *v, int elemsize,
		void (*freefunc)(void *), int init_alloc)
{
	v->elems = (void *) malloc (elemsize * init_alloc);
	v->elemsize = elemsize;
	v->log_len = 0;
	v->alloc_len = init_alloc;
	v->func = freefunc;
	return (v->elems) ? 0 : -1;
}

void  vector_dispose(vector *v)
{
	if (v->func) {
		v->log_len--;
		while (v->log_len >= 0) 
			v->func ( (char *) v->elems + 
					   v->elemsize * v->log_len);
	}
	free(v->elems);
}
int   vector_length(const vector *v) {return v->log_len;}

void *vector_nth(const vector *v, int pos) 
{
	return (char *)v->elems +
		       pos * elemsize;	
}

static void vector_grow(vector *v)
{
	v->alloc_len * 2;
	v->elems = realloc(v->elems,
			v->elemszie * v->alloc_len);
	assert(v->elems != NULL);
}

void  vector_insert(vector *v, const void *elemAddr, int pos)
{
	if (pos > v->log_len){
		fprintf(stderr, "Error: try to insert to invalid address.\n");
		return;
	}

	if (v->log_len == v->alloc_len)
		VectorGrow(v);
	void *mvAddr = v->elemsize + pos * v->elemsize;
	int size = v->elemsize * (v->log_len - pos);
	memmove((char *)mvAddr + v->elemsize, mvAddr, size);
	memcpy(mvAddr, elemAddr, v->elemsize);
	v->log_len++;
}
void  VectorAppend(vector *v, const void *elemAddr)
{
	if (v->log_len == v->alloc_len)
		VectorGrow(v);
	memcpy((char *)v->elems + v->log_len * v->elemsize, 
			elemAddr, v->elemsize);
	v->log_len++;
}

void  VectorReplace(vector *v, const void *elemAddr, int pos)
{
	if (pos >= v->log_len) {
		fprintf(stderr, "Error: try to replace at invalid address.\n");
		return;
	}
}

void  VectorDelete(vector *v, int pos);
{
	if (pos >= v->log_len) {
		fprintf(stderr, "Error: try to delete at invalid address.\n");
		return;
	}
	void *deladdr = (char *)v->elems +
				v->elemsize * pos;
	int size = v->elemsize * (v->log_len - pos - 1);
	memmove(deladdr, (char *)deladdr + v->elemsize, size);
	v->log_len--;
}

void  VectorSearch(const vector *v, const void *key,
		   VecCmpF searchfn, int startIndex,
		   int isSorted);

void VectorSort(vector *v, VecCmpF cmpfn);

void VectorMap(vector *v, VecMapF mapfn, void *auxData);


