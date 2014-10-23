#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "misc_types.h"
#include <stdio.h>


int count = 0;
typedef volatile struct hash_elem {
	volatile struct hash_elem *next;
	char data[1];
} hash_elem;


typedef struct {
	hash_elem *(*table);	/* hash_elem *table[] */
	int esize;		/* the true data size */
	int hesize;		/* hash elem's size */
	int log_len;
	int alloc_len;

//	slots loc;	/* where I stores true elems */
	int (* hash) (void *, int);	/* this is not a generic hash func prototype,
					 * coz I use int to do index, the best
					 * size is ind_t, but it suffers '0-1 > 0'
					 * problem. */ 
} hash_tab;

void ht_init(hash_tab *ht, int esize, int init_alloc, 
		int (*hash) (void *, int), 
		void (*func) (void *))
{
	ht->table = calloc(init_alloc, sizeof(hash_elem *));

	ht->esize = esize;
	//I could optimize it, since I know the how hash_elem's mem maps
	ht->hesize = sizeof(volatile struct hash_elem) -1 + esize;
	ht->log_len = 0;
	ht->alloc_len = init_alloc;
	ht->hash = hash;

//	slots_init(&ht->loc, ht->hesize, init_alloc, func);
}

void ht_dispose(hash_tab *ht)
{
	free(ht->table);
//	slots_dispose(&ht->loc);
}

/* the mainjob of remap is change the hash_elem * in slots, coz we don't wanna
 * rewrite whole slots, thats costy */
static void ht_remap(hash_tab *ht, hash_elem **tab)
{
	hash_elem *symbol;
	int i;
	int h;
	hash_elem *magic = (hash_elem *)ht->table + 
		ht->log_len;
	for (i = 0; i < ht->log_len; i++) {
		while (ht->table[i] != NULL) {
			symbol = ht->table[i];
			printf("%d, %p\n", count, symbol);
			count++;
			if (symbol >= magic) {
				ht->table[i] = NULL;
				break;
			}
			ht->table[i] = symbol->next; 	//unhook
			//now we need just add symbol new tab :)
			h = ht->hash((void *)&symbol->data, ht->alloc_len);
			symbol->next = tab[h];
			tab[h] = symbol;
		}
	}
}
static void ht_grow(hash_tab *ht)
{
	int trash = 0;
	ht->alloc_len *= 2;
	hash_elem **new_table = calloc(ht->alloc_len, sizeof(hash_elem *));
	assert(new_table);
	
	ht_remap(ht, new_table );
	free(ht->table);
	ht->table = new_table;
}

void *ht_lookup(hash_tab *ht, void *elem_addr, int create)
{
	if (ht->log_len == ht->alloc_len)
		ht_grow(ht);
	// find
	hash_elem *symbol;
	int h = ht->hash(elem_addr, ht->alloc_len);
	symbol = ht->table[h];
	while(symbol != NULL) {
		if (memcmp((void *)&symbol->data, elem_addr, ht->esize) == 0)
			return &symbol->data;
		symbol = symbol->next;
	}
	//no in table, insert it in
	if (create) {
		symbol = malloc(sizeof(ht->hesize));
		symbol->next = ht->table[h];			//copy data
		memcpy(&symbol->data, elem_addr, ht->esize);	//copy data

		//symbol = (hash_elem *)slots_insert(&ht->loc, (void *)symbol);
		ht->table[h] = symbol;
		ht->log_len++;
		return &symbol->data;
	}
}

void ht_delete(hash_tab *ht, void *elem_addr)
{
	assert(ht->log_len > 0);

	hash_elem *symbol, *trash;
	int h = ht->hash(elem_addr, ht->alloc_len);
	symbol = ht->table[h];
	while(ht->table[h] != NULL) {
		if (memcmp((void *)&symbol->data, elem_addr, ht->esize) == 0) {
			ht->table[h] = symbol->next;
			free(symbol);
			return;
		}
	}
}


/* eh, my hash function will be .... */
int silly_hash(void *d, int len)
{
	int a = (unsigned)-1 >> 1;
	return ((*(int *)d) * 31 & a) % len;
}

int main()
{
	hash_tab ht;
	ht_init(&ht, sizeof(int), 4, &silly_hash, NULL);
	int i,*a;
	for(i = 0; i < 200; i++) {
		a = (int *)ht_lookup(&ht, &i, 1);
		//printf("%p\t", a);
	}
	putchar('\n');
}
