#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "misc_types.h"
/* hash table using open addressing method */
/* there si a problem, in open addressing I cannot decide which symbol is legal
 * since I don't know what users store in table. Generally, pointer is easy to
 * tell legal or not, but it does not apply to all the other types, especially
 * struct.
 * 
 * The best way to do so is leaving this to user. request them to provide check
 * function.
 * The check funtion do three things, first we use it in init_function, pass
 * OHT_INIT in, setup OHT_NIL, we also use it in lookup, pass OHT_CHK in, and 
 * see the result. and we use it in delete function, pass OHT_DEL in, setup
 * OHT_DELETED.
 */


void 
oht_init(ohash_tab *oht, size_t esize, size_t init_alloc, 
		size_t (*hash) (void *, size_t), 
		int (*cmp) (const void *, const void *),
		int (*check) (void *, int, size_t),
		void (*cp) (void *, void *))
{
	oht->table = malloc(2 * init_alloc * esize);

	oht->esize = esize;
	// The true memory size I need is 12, but 16 should be faster
	oht->log_len = 0;
	oht->alloc_len = 2 * init_alloc;
	oht->hash = hash;
	oht->cmp = cmp;
	oht->check = check;
	oht->cp = cp;
	assert(oht->check);
	oht->check(oht->table, OHT_INIT, init_alloc);

}

void oht_dispose(ohash_tab *oht)
{
	free(oht->table);
}
/* now the open_addressing function will not return a null value */
static int open_addressing(const ohash_tab *oht, void *elem_addr, size_t h,
				void **symbol)
{
	*symbol = (char *)oht->table + oht->esize * h;
	//initialize step
	int inc = 0;
	int step = 0;		
	int chk = OHT_NIL;
	while ((chk = oht->check(*symbol, OHT_CHK, 1)) == OHT_OHTER) {
		if (oht->cmp) {
			chk = oht->cmp(*symbol, elem_addr);
		} else {
			chk = memcmp(*symbol, elem_addr, oht->esize);
		}
		if (chk == 0) 
			return OHT_OHTER; 	//find it

		// use Quadradic probing method 
		inc += step * step; 
		h = (h + inc) % oht->alloc_len;
		*symbol = (char *)oht->table + oht->esize * h;
		step +=  1;
	}
	return chk;
}
static void oht_grow(ohash_tab *oht) 
{
	ohash_tab newht;
	memcpy (&newht, oht, sizeof(ohash_tab));
	newht.log_len = 0;
	newht.alloc_len *= 2;

	void *new_tab = malloc(oht->alloc_len * oht->esize);
	assert (new_tab != NULL);

	oht->check(new_tab, OHT_INIT, newht.alloc_len);

	newht.table = new_tab;
	
	size_t i;
	void *symbol;
	int chk;
	//remap
	for (i = 0; i < oht->alloc_len; i++) {
		symbol = (char *)oht->table + oht->esize * i;
		if ((chk = oht->check(symbol, OHT_CHK, 1)) == OHT_OHTER)
			oht_lookup(&newht, symbol, HT_CRE);
		
	}
	free(oht->table);
	oht->table = new_tab;
	oht->alloc_len *= 2;
}

void *oht_lookup(ohash_tab *oht, void *elem_addr, int create)
{
	if (oht->log_len * 2 >= oht->alloc_len )
		oht_grow(oht);
	size_t h = (size_t) oht->hash(elem_addr, oht->alloc_len);
	
	void *symbol;  
	int chk = open_addressing(oht, elem_addr, h, &symbol);
	if (chk == OHT_OHTER)
		return symbol;
	//no in table, insert it in
	else if (create) {
		if (oht->cp)
			oht->cp(symbol, elem_addr);
		else 
			memcpy(symbol, elem_addr, oht->esize);

		oht->log_len++;
		return symbol;
	}
	return NULL;
}

void oht_delete(ohash_tab *oht, void *elem_addr)
{
	assert(oht->log_len > 0);
	size_t h = (size_t) oht->hash(elem_addr, oht->alloc_len);
	void *symbol; 
	int chk = open_addressing(oht, elem_addr, h, &symbol);
	if (chk == OHT_OHTER) {
		oht->check(symbol, OHT_DEL, 1);
		oht->log_len--;
	}//else do nothing
}
/*
unsigned long silly_hash(void *d, size_t len)
{
	unsigned long a = (unsigned long)-1 >> 1;
	return ((*(unsigned long *)d) & a) % len;
}

static int stack_cmp(const void *a, const void *b)
{
	return *(int *)a != *(int *)b;
}

int silly_check(void *item, int chk_val, size_t how_many)
{
	static int magic_val_init = 2147483647;
	static int magic_val_deleted = 2147483646;
	size_t i;
	int  chk_rel = OHT_OHTER;
	int *elem = (int *)item;
	for (i = 0; i < how_many; i++) {
		// the job here is either check the item value or
		// set some value for later checking
		switch (chk_val) {
			case OHT_INIT:
				*elem = magic_val_init;
				break;
			case OHT_CHK:
				chk_rel = (*elem == magic_val_init) ? 
					OHT_NIL : OHT_OHTER;
				chk_rel = (*elem == magic_val_deleted) ?
					OHT_DELETED : chk_rel;
				break;
			case OHT_DEL:
				*elem = magic_val_deleted;
				break;
		}
		elem++;
	}
	return chk_rel;
}

void int_cp(void *a, void *b)
{
	*(int *)a = *(int *)b;
}
#include <stdio.h>
int main()
{
	ohash_tab oht;
	oht_init(&oht, sizeof(int), 4, &silly_hash, &stack_cmp, 
			&silly_check, &int_cp);
	int i,*a;
	for(i = 0; i < 200; i++) {
		a = (int *)oht_lookup(&oht, &i, 1);
		printf("%p\t", a);
	}
	putchar('\n');
}
*/
