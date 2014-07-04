#ifndef STRINGTAB_H
#define STRINGTAB_H 	1

#ifndef	PUBLIC
#define PUBLIC 
#endif  /* public */

#ifndef PRIVATE
#define PRIVATE static
#endif	/* private */

#include "snl_list.h"
typedef struct Entry {
	int len;		/* the length of the string */
	int index;		/* a unique index of the string */
	struct List list;	/* insert List as member of Entry */
	char str[1];		/* the string, it should be on the last */
} *Symbol;
/* you may need this as short hand */
#define E_NEXT list->next
#define E_PREV list->prev


struct str_table {
	int index;		/* current index */
	struct Entry *entry;	/* currnet pointer of entry */
}; 


/* it is good that we dont need del_string function for now, so I can
 * implement add_string function more quickly */
struct Entry *lookup_string(struct str_table *table, char *string);
struct Entry *add_string(struct str_table *table, char *string);


/* Global variables */
extern struct str_table *idtable;
extern struct str_table *strtable;

#endif	/* STRINGTAB_H */
