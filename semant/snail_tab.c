#include <stdlib.h>
#include <string.h>
#include "snail_tab.h"
#include "misc/misc_types.h"

void *magic_val_init = NULL;
void *magic_val_deleted = (void *)1;

/* this is never generic */ 
static int str_chk(void *, int, size_t);
static int str_cmp(const void *, const void *);

void st_init(symbol_tab *st, size_t file_size, 
		unsigned long(* hash) (void *, size_t))
{
	oht_init(&st->table, sizeof(symbol), 32, hash, str_cmp, &str_chk, NULL);

	smm_init(&st->arr, sizeof(char), file_size * 2, file_size, NULL);
}

void st_dispose(symbol_tab *st)
{
	oht_dispose(&st->table);
	smm_dispose(&st->arr);
}

symbol *st_add_string(symbol_tab *st, char *str)
{
	symbol e, *test;
	e.len = strlen(str);
	e.base = NULL;
	e.str = str;
	smmblk *smm = &st->arr;

	int chk = oht_lookup(&st->table, &e, (void **)&test, NOC);
	if (chk == 0) {
		smm_append(smm, str, (void **)&(e.str), e.len + 1);
		e.base = &st->arr;
		e.index = smm_getpos(smm, (e.str));
		oht_lookup(&st->table, &e, (void **)&test, CRE);
	}
	return test;
}
/* concat_string tries to append string later after formal,
 * remember formal is in parser table, later is in lexer table.
 */
static inline char *sym_gstr(symbol *s)
{
	if (s->base == NULL)
		return  s->str;
	else
		return (char *)(((symbol *)s)->base->elems) + 
			((symbol *)s)->index;
}
static int islast(smmblk *s, smm_t pos, int len)
{
	return (pos + len == s->log_len);
}
/* they are both in the table, so use sym_gstr */
symbol *st_concat_string(symbol_tab *st, symbol *formal, symbol *later)
{
	smmblk *smm = formal->base;
	char *sym, *trash;
	//in most case, formal will be the last string in array, we just
	//append later in and we are done
	if (islast(&st->arr, formal->index, formal->len + 1)) {
		smm_delete(&st->arr,(void *) &sym, 1);
		smm_append(&st->arr, sym_gstr(later), (void *)&sym, later->len + 1);
		sym = sym_gstr(formal);	//we just need the first string addr
	} // otherwise, we have to re-install formal string again :(
	else {
		smm_append(&st->arr, sym_gstr(formal), (void *)&sym, formal->len);

		smm_append(&st->arr, sym_gstr(later), (void *)&trash, later->len+1);
	}
	//now we have to update formal's hash value
	oht_delete(&st->table, formal);
	formal->base = &st->arr;
	formal->index = smm_getpos(smm, sym);
	formal->len = formal->len + later->len + 1;
	symbol *ret;
	oht_lookup(&st->table, formal, (void **)&ret, CRE);
	return ret;
	//actually, return value should be formal again.
}
symbol *single_string(symbol_tab *st, symbol *s)
{
	return st_add_string(st, sym_gstr(s));
}
/* djb2 is a good hash function for string, it's fast and little collision.
 * The original reference: http://www.cse.yorku.ca/~oz/hash.html   
 */


static unsigned long 
djb2 (unsigned char *str)
{
	unsigned long hash = 5381;
	unsigned int c;

	while((c = *str++)) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}

unsigned long 
string_hash(void *sym, size_t hashsize)
{
	unsigned char *c = (unsigned char *)sym_gstr((symbol *)sym);
			
	unsigned long hashval = djb2(c);
	return hashval % hashsize;
}
/* I intend to use it for symbol */
unsigned long 
symbol_hash(void *sym, size_t hashsize)
{
	unsigned char *c = (unsigned char *)sym_gstr((symbol *)sym);
		    
	unsigned long hashval = 0;

	for (hashval = 0; *c != '\0'; c++) {
		hashval = *c + 31 * hashval;
	}
	return hashval % hashsize;
}
static int str_cmp(const void *a, const void *b)
{
	char *s1 = sym_gstr((symbol *)a);
	char *s2 = sym_gstr((symbol *)b);
	return strcmp(s1, s2);
}
static int str_chk(void *item, int chk_val, size_t how_many)
{
	size_t i;
	int   chk_rel = OHT_OTHER;
	char *elem = sym_gstr((symbol *)item);
	for (i = 0; i < how_many; i++) {
		// the job here is either check the item value or
		// set some value for later checking
		switch (chk_val) {
			case OHT_INIT:
				elem = magic_val_init;
				break;
			case OHT_CHK:
				chk_rel = (elem == magic_val_init) ? 
					OHT_NIL : OHT_OTHER;
				chk_rel = (elem == magic_val_deleted) ?
					OHT_DELETED : chk_rel;
				break;
			case OHT_DEL:
				elem = magic_val_deleted;
				break;
		}
		elem++;
	}
	return chk_rel;
}
/*
#include <stdio.h>
#include <assert.h>
int main()
{
	char *s = "This is a test.";
	char *t = "This is still a test.";
	FILE *f = fopen("snail_tab.c", "r");
	assert(f != NULL);
	fseek(f, 0, SEEK_END);
	size_t len = ftell(f);
	fseek(f, 0, SEEK_SET);
	symbol_tab str_table;
	st_init(&str_table, len, &string_hash);
	symbol *sym1, *sym2;
		sym1 = st_add_string(&str_table, s);
		sym2 = st_add_string(&str_table, t);
		sym1 = st_concat_string(&str_table, sym1, sym2);
		printf("%s", sym1->str);
	putchar('\n');
	return 0;
}
*/
