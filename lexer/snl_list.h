#ifndef SNL_LIST_H
#define SNL_LIST_H

struct List {
	struct List *next, *prev;	/* include this struct in your data struct */
};
/*
 * offsetof
 */
#ifndef offsetof
/* it defines a macro return the member offset of given struct 
 * define here in case we miss gnu's container of */
#define offsetof(TYPE, MEMBER) \
	(size_t)(&((TYPE *)0)->MEMBER )
/* zero here means the struct's address is 0 */
#endif /* offsetof */

/*
 * container_of
 */
#ifndef container_of
#define container_of(PTR, TYPE, MEMBER) \
	(TYPE *)( (void *)(PTR) - offsetof(TYPE, MEMBER) )
#endif /* container_of */

#ifndef list_entry
/* get the address of struct type that contains list
 * @ptr, the pointer of List
 * @type, the type that contains List
 * @member, list domain
 * a typical use "list_entry(list_node, struct test list, list)"
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)
#endif /*list_entry*/

/* List functions */
static inline void init_list(struct List *list)
{
	list->next = list;
	list->prev = list;
}
/* Insert a new entry between two know entries */
static inline void list_add (struct List *new,
			     struct List *prev,
			     struct List *next)
{
	next->prev = new;
	new->next = next;
	prev->next = new;
	new->prev = prev;
}
/* List func ends */

#endif /* SNL_LIST_H */
