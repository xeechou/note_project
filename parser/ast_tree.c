#include "ast_tree.h"
//#include <stdarg.h> since function is defined in header files, the head file
//must include <stdarg.h>
#include "misc/misc_types.h"

static void del_node_data(node_data *to_del, page_strt *p);
extern int curr_lineno;			// a notorious global we have :p
static inline void info_debug(debug_info *info, int type, int (*func)(int, ast_node *))
{
	info->curr_lineno = curr_lineno;
	info->type = type;
	info->dump = func;
}
/* generate one line in syntax tree */
ast_node *syntax_line(char *start, char *end, 
		      	int type, node_data *items,
			unsigned int n_nodes, page_strt *p)
{
	ast_node n, *ret;
	info_debug(&n.info, type, dump_type); //the first function we added in will add all the
					 //other related functions in
					 //debug_info
	n.start.name = start;
	n.end.name = end;
	n.items = items;
	n.n_nodes = n_nodes;

	pstrt_insert(p, &n, (void **)&ret);
	return ret;
}

/* we tries to delete a node_data n from a ast_node, check if n is the first
 * node of ast_node */
void syntax_line_del(node_data *n, node_data **pos, page_strt *p)
/* this funtion assumes that only restraint now is head problem */
{
	node_data *d = *pos;
	if (n->next == n)  		//the only node...
		*pos == NULL;
	else if (n == d)  		//Oops, first node
		*pos = n->next; 
	del_node_data(n, p);		//delete it anyway
	return;
}

void clear_node_data(ast_node *n, page_strt *p)
{
	node_data *to_del = n->items;
	
	while (to_del->next != n->items) {
		node_data *entry = to_del;
		to_del->next = to_del->next->next;
		del_node_data(entry, p);
	}
	del_node_data(to_del, p);
	n->items = NULL;
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

static void del_node_data(node_data *to_del, page_strt *p)
{
	node_data *prev = to_del->prev;
	node_data *next = to_del->next;
	prev->next = next;
	next->prev = prev;
	pstrt_delete(p, to_del);
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
node_data *data_sym(symbol *sym, page_strt *p)
{
	node_data d, *ret;
	d.sym = sym;
	d.type = NT_SYM;
	pstrt_insert(p, &d, (void **)&ret);
	return ret;
}

node_data *data_int(int i, page_strt *p)
{
	node_data d, *ret;
	d.i = i;
	d.type = NT_INT;
	pstrt_insert(p, &d, (void **)&ret);
	return ret;
}

node_data *data_node(ast_node *n, page_strt *p)
{
	node_data d, *ret;
	d.n = n;
	d.type = NT_NODE;
	pstrt_insert(p, &d, (void **)&ret);
	return ret;
}
/* test region */
/*
static page_strt nodes_data;
static page_strt ast_nodes;
static char *start = NULL;
static char *end = NULL;
#include <stdio.h>
int curr_lineno;
FILE *stream;
int main(void) {
	stream = stdin;
	err_pstrt_init(&nodes_data, sizeof(node_data), 4096, NULL);
	err_pstrt_init(&ast_nodes, sizeof(ast_node), 4096, NULL);
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
