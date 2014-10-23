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
	node_data* next;		/* thanks god the index never changes */
	node_data* prev;
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
	node_data* items;
	unsigned int n_nodes;
	union {
		char *name;	/* used in code gen or other, eg: </kpt> */
	}end;
	ast_node *prev;
	ast_node *next;
} __attribute__ (( aligned(8) ));


/************************ functions ************************/
ast_node *syntax_line(char *start, char *end, 
		      	int type, node_data *items,
			unsigned int n_nodes, page_strt *p);
/* rm one node */
void rm_node_data(node_data *n, node_data **pos, page_strt *p);
void rm_ast_node(ast_node *, node_data *, page_strt *p);
/* rm all the nodes in ast_node n */
void clear_node_data(ast_node *n, page_strt *p);
node_data *compose_node_data(int n, node_data *first, ...);
node_data *append_node_data(int n,  node_data *first, ...);
node_data *n_node(ast_node *a, unsigned int n);
node_data *data_sym(symbol *sym, page_strt *p);
node_data *data_int(int i, page_strt *p);
node_data *data_node(ast_node *n, page_strt *p);
#endif /* ast_tree_new */
