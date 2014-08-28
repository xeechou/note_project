#ifndef SNL_LIST_H
#define SNL_LIST_H
/* TODO: change this shit to single_list so that we can support
 * nil_expressions */
struct List {
	struct List *next, *prev;
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
static inline void list_rm(struct List *del,
			   struct List *next,
			   struct List *prev)
{
	prev->next = next;
	next->prev = prev;
}
/* append a node at the end of list */
static inline void list_append (struct List *list, struct List *node)
{
	struct List *last = list->prev;
	last->next = node;
	node->prev = last;
	node->next = list;
	list->prev = node;
}
/* List func ends */

#endif /* SNL_LIST_H */
