#ifndef AST_TREE_NEW_H
#define AST_TREE_NEW_H

#include <stdarg.h>
#include "snail_tab.h"
#define NT_SYM		0
#define NT_NODE		1
#define NT_INT		2

/* REMEMBER, this generic design make typing functions heavily rely on the
 * data sequence order */

/************************ structs ************************/ 
typedef struct node_data node_data;
typedef struct ast_node ast_node;
struct node_data {
	union {		/* I can still use offsetof to get node_data */
		ast_node *n;	/* data can be a pointer to its subtree */
		symbol *sym;	/* data can be a name, or SNAIL string */
		int i;
	};
	int type;
	size_t next;		/* thanks god the index never changes */
	size_t prev;
} __attribute__ (( aligned(8) ));

int dump_type (int, ast_node *);

typedef struct {
	int type;
	int curr_lineno;
	int (*dump) (int, ast_node *);
} debug_info;


struct ast_node {
	debug_info info;	/* type, curr_line, ... */
	union {
		char *name;	/* used in dump and code gen or other, eg: <kpt> */
	}start;
	size_t items;
	unsigned int n_nodes;
	union {
		char *name;	/* used in code gen or other, eg: </kpt> */
	}end;
	size_t prev;
	size_t next;
} __attribute__ (( aligned(8) ));


/************************ functions ************************/
ast_node *syntax_line(char *start, char *end, 
		      	int type, size_t items,
			unsigned int n_nodes, slots *s);

void syntax_line_del(node_data *n, node_data **pos, slots *s);
void clear_node_data(ast_node *n, slots *s);
size_t compose_node_data(int n, node_data *first, ...);
size_t append_node_data(int n,  node_data *first, ...);
size_t n_node_data(ast_node *a, unsigned int n);
size_t data_sym(symbol *sym, slots *s);
size_t data_int(int i, slots *s);
size_t data_node(ast_node *n, slots *s);
#endif /* ast_tree_new */
