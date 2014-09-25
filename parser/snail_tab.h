#ifndef SNAIL_TAB_H
#define SNAIL_TAB_H

#include <stddef.h>
#include "misc/misc_types.h"
#define CRE 	1
#define NOC	0
typedef struct {
	int len;
	smmblk *base;
	smm_t index;
	char *str;
} symbol;

typedef struct {
	ohash_tab table;
	smmblk arr;
} symbol_tab;


void st_init(symbol_tab *st, size_t file_size, 
		unsigned long(* hash) (void *, size_t));
		//, int (* hash_cmp) (const void *, const void *));


void st_dispose(symbol_tab *st);
symbol *st_add_string(symbol_tab *st, char *str);
symbol *st_concat_string(symbol_tab *st, symbol *formal, symbol *later);
symbol *single_string(symbol_tab *st, symbol *sym);
#define append_string(a, b, c) 	  st_concat_string(a, b, c)
/* two extral hash function */
unsigned long 
string_hash(void *sym, size_t hashsize);
unsigned long 
symbol_hash(void *sym, size_t hashsize);

#endif /* snail_tab.h */
