#include "ast_tree.h"
//#include <stdarg.h> since function is defined in header files, the head file
//must include <stdarg.h>
#include "misc/misc_types.h"
#define OFF_LIMIT 		(slot_ind_t)(-1)
static void del_node_data(slot_ind_t to_del, slots *s);

inline node_data *data_nth(slot *s, slot_ind_t n) 
{
	return (node_data *)slot_nth(s, n);
}

inline ast_node  *node_nth(slot *s, slot_ind_t n) 
{
	return (ast_node *)slot_nth(s, n);
}

extern int curr_lineno;			// a notorious global we have :p
static inline void info_debug(debug_info *info, int type, int (*func)(int, ast_node *))
{
	info->curr_lineno = curr_lineno;
	info->type = type;
	info->dump = func;
}
/* generate one line in syntax tree */
slot_ind_t syntax_line(char *start, char *end, 
		      	int type, slot_ind_t items,
			unsigned int n_nodes, slots *s)
{
	ast_node n, *ret;
	info_debug(&n.info, type, dump_type); //the first function we added in will add all the
					 //other related functions in
					 //debug_info
	n.start.name = start;
	n.end.name = end;
	n.items = items;
	n.n_nodes = n_nodes;

	ret = slots_insert(s, &n);
	return slots_getpos(s, ret);
}

/* we tries to delete a node_data n from a ast_node, check if n is the first
 * node of ast_node */
void syntax_line_del(slot_ind_t n, slot_ind_t *head, slots *s)
{
	node_data *d = data_nth(s, *head);
	if (d->next == *head)			//the only node...
		*head == OFF_LIMIT;		//set it to too big to reach
	else if (n == d)  			//Oops, first node
		*head = data_nth(s, n)->next; 
	del_node_data(n, s);			//delete it anyway
	return;
}

void clear_node_data(ast_node *n, slots *s)
{
	node_data *entry = data_nth(s, n->items);
	nth = n->items;
	int i; slot_ind_t next;
	//every step we get next index and data first, then we delete current
	//one
	for (i = 0; i < n->n_nodes; i++){
		next = entry->next;
		entry = data_nth(entry->next);

		del_node_data(s, nth);
		nth = next;
	}
	n->items = OFF_LIMIT;
}

node_data *compose_node_data(int num, node_data *first, ...)
{
	if (first == NULL)
		return NULL;
	first->prev = first->next = first;	//initialize
	va_list ap;

	node_data *i, *prev;
	va_start(ap, first);
	int count = 0;
	for (prev = i = first; count < num; i = va_arg(ap, node_data *)) {
		i->prev = prev;		//first iteration is trival
		i->next = first;	//first iteration is trival
		prev->next = i;
		first->prev = i;

		prev = first->prev;	//100% correctness
		count++;
	}
	va_end(ap);
	return first;
}


node_data *append_node_data(int num, node_data *first, ...)
{
	if (first == NULL)
		return NULL;
	va_list ap;

	node_data *i, *prev, *next;
	prev = first->prev;
	next = first->next;
	va_start(ap, first);

	// the first iteration is little tricky, coz you want to add head to
	// where head belong. so the inital env has to be right:
	// prev = first->prev;
	// next = first->next;
	//
	// after that, prev is always first->prev, and next is always
	// first
	int count = 0;
	for (i = first; count <= num; i = va_arg(ap, node_data *)) {
		i->prev = prev;		//first iteration is trival
		i->next = next;		//first iteration is trival
		prev->next = i;
		next->prev = i;
		next = first;
		prev = first->prev;	//100% correctness
		count++;
	}
	va_end(ap);
	return first;
}

static void del_node_data(slot_ind_t to_del, slots *s)
{
	node_data *n = slot_nth(s, to_del);

	slot_ind_t prev = n->prev;
	slot_ind_t next = n->next;
	data_nth(s, prev)->next = next;
	data_nth(s, next)->prev = prev;
	void *trash;
	slots_delete(s, to_del, &trash);
}
/* To ensure n is not to large, 
 * this function has to be invoked by ast_node related function */
/* start from 0 */
node_data *n_node_data(ast_node *a, unsigned int n)
{
	if (n >= a->n_nodes)
		return NULL;
	int i = 0;
	node_data *pos = a->items; // at least excute once
	while (i < n) {
		pos = pos->next;
		i++;
	}
	return pos;
}

/****************** primitive type specific function ******************/
slot_ind_t data_sym(symbol *sym, slots *s)
{
	node_data d, *ret;
	d.sym = sym;
	d.type = NT_SYM;
	ret = slots_insert(s, &d);
	return ret;
}

slot_ind_t data_int(int i, slots *s)
{
	node_data d, *ret;
	d.i = i;
	d.type = NT_INT;
	ret = slots_insert(s, &d);
	return ret;
}

slot_ind_t data_node(ast_node *n, slots *s)
{
	node_data d, *ret;
	d.n = n;
	d.type = NT_NODE;
	ret = slots_insert(s, &d);
	return ret;
}
/* test region */
/*
static slots nodes_data;
static slots ast_nodes;
static symbol *start = NULL;
static symbol *end = NULL;
int main(void) {
	slots_init(&nodes_data, sizeof(node_data), 4, NULL);
	slots_init(&ast_nodes, sizeof(ast_node), 4, NULL);
	ast_node *b = syntax_line(start, end, 1, NULL, 0, &ast_nodes);

	node_data *nd = compose_node_data(4, 
					  data_int (1, &nodes_data),
					  data_int (2, &nodes_data),
					  data_int (3, &nodes_data), 
					  data_node(b, &nodes_data));
	ast_node *a = syntax_line(start, end, 1, nd, 4, &ast_nodes);
	unsigned int i;

	for (i = 0; i < 3; i++) {
		int c = n_node_data(a, i)->i;
		printf("%d\n", c);
	}
	return 0;
}
*/
