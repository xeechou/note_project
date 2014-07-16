#ifndef AST_TREE_H
#define AST_TREE_H
/* This file is a collection of data structures which describe abstract syntax
 * tree, not the final storage file, storage could be much more simple.
 */

#include "snl_list.h"
#include "stringtab.h"
#include "snl_stringtab.h"


/* for future optimization */
struct debug_info {
	int curr_lineno;
};

typedef struct List node; 

typedef struct Program Program_; 
typedef struct Topic Topic_;
typedef struct Feature Feature_;
typedef struct Kpt Kpt_;
typedef struct List_ List_;
typedef struct Proc Proc_;
typedef struct Basic_expr Basic_expr_;
typedef struct Formal Formal_;
typedef struct Navig Navig_;
typedef struct Formal_navig Formal_navig_;
typedef struct Enum_navig Enum_navig_;
typedef struct Let_navig Let_navig_;
//typedef struct Case_navig Case_navig_;
typedef struct Assign Assign_;
typedef struct Conn Conn_;
typedef struct Const Const_;



/* the struct is used to represent a subset of certern structs,
 * every struct is started with curr_lineno, list, (*dump)(int n),
 * (dump function points to the exact function that will display
 * information of expressions,
 * */

/* TODO: use node_struct in this way :
 * struct some_struct {
 * 	struct node_struct;
 * 	type data;
 * 	...
 * };
 * This mechanism may increase the complexity of dump function */


struct Program {
	int curr_lineno;
	struct List *topic_list;	/* which points to the first list in Topic_ */
};

struct Topic {
	int curr_lineno;
	struct List list;
	int (*dump) (int n, struct List *pos);

	str_symbol name;
	struct List *features;
} ;

struct Kpt {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	struct List *attr_l;
	str_symbol name;
	str_symbol aka_name;
	str_symbol subset_name;
};

struct List_ {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	struct List *nlist;
};

struct Proc {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);
};
/* not completed struct */

struct Formal {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);	/* actually there is no dump function for formal, coz it is not necessary */

	str_symbol topic;
	str_symbol kpt;
	str_symbol attr;
};

struct Basic_expr {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	str_symbol attr_name;
	struct List *con;
};

/****** navigation options ******/
/* I guess I still need a function
 * to handle difference between navigations
 */

struct Formal_navig {
	int curr_lineno;
	struct List list;	/* as a node of navigation list */
	int (*dump) (int n, struct List *pos);

	node *formal;
	struct List *alist;	/* point to the first assignment node */
	/* problem, I cannot be sure is which kind of assignment */
};




struct Let_navig {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	struct List *formal_list;
	struct List *alist;
};

/******* navigation options ****/

/******* assignment options ****/
struct Assign {
	int curr_lineno;
	struct List list;	/* as a node of assignment */
};

/* fix it in the future */
struct Const {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	int type;
	union {
		str_symbol id;
		Proc_  *proc;
		str_symbol str;
	} con;
};

struct Conn {
	int curr_lineno;
	struct List list;
	int (*dump) (int n, struct List *pos);
	int type;
	node* formal;
	str_symbol name;
	struct List *constant;
};

/* the widely used function... */
static inline node *single_node(node *n)
{
	init_list(n);
	return n;
}

static inline node *append_node(node *a, node *b)
{
	list_append(a,b);
	return a;
}

/*functions */
extern Program_ *program(node *topics);
extern node *topic_simple(Symbol name, node *features);
extern node *kpt_feature(Kpt_* k);
extern node *proc_feature(Proc_ *p);
extern node *list_feature(List_ *l);
extern Kpt_ *kpt_simple(str_symbol, str_symbol, str_symbol, node *); 
extern Kpt_ *kpt_const(Kpt_ *k, struct List *s);

extern List_ *list_simple(node *n);

extern node *basic_expr(str_symbol a, struct List *con);
/* formal */
extern node *navig(Symbol topic, Symbol kpt, str_symbol attr);

extern node *formal_navig(node *formal, node *n);
extern node *let_navig(node *n, node *a);

extern node *connection(node* formal, int type, Symbol name, node *alist);
extern node *id_node(Symbol s);
extern node *string_node(str_symbol s);


#endif /*AST_TREE_H */
