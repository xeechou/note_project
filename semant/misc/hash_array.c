#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "misc_types.h"


void lht_init(lhash_tab *lht, size_t esize, size_t init_alloc, 
		unsigned long (*hash) (void *, size_t), 
		int (*cmp)  (const void *, const void *),
		void (*func) (void *))
{
	lht->table = calloc(init_alloc, sizeof(lhash_elem *));

	lht->esize = esize;
	// The true memory size I need is 12, but 16 should be faster
	lht->hesize = sizeof(lhash_elem) + esize;
	lht->log_len = 0;
	lht->alloc_len = init_alloc;
	lht->hash = hash;
	lht->cmp = cmp;
	slots_init(&lht->loc, lht->hesize, func);
}

void lht_dispose(lhash_tab *lht)
{
	free(lht->table);
	slots_dispose(&lht->loc);
}
/* okay problem found: I reused the memory that I freed.
 * everytime I invoke lht_remap, I just re-direct all the all the pointers.
 * It is okay in re_map, but after that, it is gonna invoke slots_grow, when
 * problem occurs. 
 *
 * You do remember all the pointer addresses is actually in the range of slots
 * So, you realloc, the those pointers are no longer valid. When you remap
 * again, god knows what those addresses stores.
 * 
 * Solution:
 * To fix this,You have to alloc memory for new slots when lht_remap, and map
 * address in new_slots.
 */
static void lht_remap(lhash_tab *lht, lhash_elem **tab, slots *new_slots)
{
	lhash_elem *symbol;
	int i;
	size_t h;
	for (i = 0; i < lht->log_len; i++) {
		while (lht->table[i] != NULL) {
			symbol = lht->table[i];
			lht->table[i] = symbol->next; 	//unhook

			h = (size_t) lht->hash((void *)&symbol->data, lht->alloc_len);
			symbol->next = tab[h];

			tab[h] = (lhash_elem *)slots_insert(new_slots, 
							(const void *)symbol);
		}
	}
}
static void lht_grow(lhash_tab *lht)
{
	lht->alloc_len *= 2;
	lhash_elem **new_table = calloc(lht->alloc_len, sizeof(lhash_elem *));
	/* you need new slots to stores information */
	slots new_slots;
	slots_init(&new_slots, lht->hesize, lht->loc.func);
	assert(new_table);
	
	lht_remap(lht, new_table, &new_slots);
	slots_dispose(&lht->loc);
	memcpy(&lht->loc, &new_slots, sizeof(slots));
	free(lht->table);
	lht->table = new_table;
}

void *lht_lookup(lhash_tab *lht, void *elem_addr, int create)
{
	if (lht->log_len == lht->alloc_len)
		lht_grow(lht);
	// find
	lhash_elem *symbol;
	size_t h = (size_t) lht->hash(elem_addr, lht->alloc_len);
	symbol = lht->table[h];

	while(symbol != NULL) {
		if (lht->cmp((void *)&symbol->data, elem_addr) == 0)
			return &symbol->data;
		symbol = symbol->next;
	}
	//no in table, insert it in
	if (create) {
		char s[lht->hesize];
		symbol = (lhash_elem *)s;
		symbol->next = lht->table[h];			//copy data
		memcpy(&symbol->data, elem_addr, lht->esize);	//copy data
		symbol = (lhash_elem *)slots_insert(&lht->loc, (void *)symbol);
		lht->table[h] = symbol;
		lht->log_len++;
		return &symbol->data;
	}
	return NULL;
}

void lht_delete(lhash_tab *lht, void *elem_addr)
{
	assert(lht->log_len > 0);

	lhash_elem *symbol, *trash;
	size_t h =(size_t) lht->hash(elem_addr, lht->alloc_len);
	symbol = lht->table[h];
	while(symbol != NULL) {
		if (lht->cmp((void *)&symbol->data, elem_addr) == 0) {
			slots_delete_safe(&lht->loc, 
				     slots_getpos(&lht->loc, (void *)symbol), 
				     (void**)&trash);
			lht->table[h] = symbol->next;
			return;
		}
		symbol = symbol->next;
	}
}


/*
 eh, my hash function will be .... 
 */
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
#include <stdio.h>
int main()
{
	lhash_tab lht;
	lht_init(&lht, sizeof(int), 4, &silly_hash, &stack_cmp, NULL);
	int i,*a;
	for(i = 0; i < 200; i++) {
		a = (int *)lht_lookup(&lht, &i, 1);
		printf("%p\t", a);
	}
	putchar('\n');
}
*/
