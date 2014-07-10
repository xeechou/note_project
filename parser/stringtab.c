#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stringtab.h"
#include "snl.h"
struct str_table *idtable, *strtable;
void strtable_init()
{
	idtable = malloc(sizeof(struct str_table));
	idtable->entry = malloc(sizeof(struct Entry));
	init_list(&(idtable->entry->list));
	strtable = malloc(sizeof(struct str_table));
	strtable->entry = malloc(sizeof(struct Entry));
	init_list(&(strtable->entry->list));
}

/* the way to add string is divided into two steps, first, look up if string
 * is already in the table, if not, add it too the table */
struct Entry *add_string(struct str_table *table, char *str)
{
	struct List *head = &(table->entry->list);
	/* lookup */
	struct List *l;
	for (l = head->next; l != head; l = l->next)
	{
		struct Entry *tmp =
			list_entry(l, struct Entry, list);
		if (strcmp(str, tmp->str) == 0)
			return tmp;
	}

	/* insert */
	int len = strlen(str);
	struct Entry *e = (struct Entry *)
		malloc(sizeof(struct Entry) + len);  /* string space */

	/* total length should be strlen+1, end with '\0', but we have
	 * str[1] */
	if (strcpy((e->str), str) == (e->str));
	e->len = len+1;		/* include in '\0' */
	struct List *node = &(e->list);
	list_add(node, head->prev, head);
	return e;
}

struct Entry *lookup_string(struct str_table *table, char *str)
{
	struct List *head = &(table->entry->list);
	struct List *l;
	for (l = head->next; l != head; l = l->next)
	{
		struct Entry *tmp = 
			list_entry(l, struct Entry, list);
		if (strcmp(str, tmp->str) == 0)
			return tmp;
	}
	return NULL;
}
