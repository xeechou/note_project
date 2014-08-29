#ifndef AST_TREE_H
#define AST_TREE_H
/* This file is a collection of data structures which describe abstract syntax
 * tree, not the final storage file, storage could be much more simple.
 */

#include "snl_list.h"
#include "snail_tab.h"
extern symbol_tab id_table;
extern symbol_tab str_table;
extern symbol_tab str_psr_tab;

/* for future optimization */
struct debug_info {
	int curr_lineno;
};

typedef struct List node; 

typedef struct Program Program_; 
typedef struct Topic Topic_;
typedef struct Feature Feature_;
typedef struct Kpt Kpt_;
typedef struct Func Func_;
typedef struct Proc Proc_;
typedef struct Attr_Stat Attr_Stat_;
typedef struct Label_Stat Label_Stat_;
typedef struct Let_Stat Let_Stat_;
typedef struct Case_Stat Case_Stat_;
typedef struct Expr Expr_;
typedef struct Conn Conn_;
typedef struct Dispatch Dispatch_;
typedef struct Operation Operation_;
typedef struct Const Const_;
typedef struct Navig Navig_;


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

	symbol * name;
	struct List *features;
} ;

struct Kpt {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	struct List *attr_l;
	symbol * name;
	symbol * aka_name;
	symbol * subset_name;
};

struct Func {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);
	//function is sequences of statements
	symbol *name;
	struct List *args;
	struct List *stat_l;
};

struct Proc {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);
};
/* not a completed struct */


struct Attr_Stat {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	symbol * attr_name;
	struct List *expr;
};

/****** Statation options ******/

struct Label_Stat {
	int curr_lineno;
	struct List list;	/* as a node of Statation list */
	int (*dump) (int n, struct List *pos);

	node *navig;
	struct List *expr;
};

struct Let_Stat {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	struct List *navig_list;
	struct List *expr;
};

struct Case_Stat {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	symbol *name;
	struct List *stat_l;

};
/******* Statation options ****/









/******* expressions ****/
struct Expr {
	int curr_lineno;
	struct List list;	/* as a node of assignment */
};

struct Const {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	int type;
	union {
		symbol * id;
		Proc_  *proc;
		symbol * str;
	} con;
};

struct Navig {
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);

	int in_function;
	symbol * topic;
	symbol * kpt;
	symbol * attr;
};
/*checked */
struct Conn {
	int curr_lineno;
	struct List list;
	int (*dump) (int n, struct List *pos);
	int type;
	node* navig;

	struct List *expr;
};
struct Dispatch {
	int curr_lineno;
	struct List list;
	int (*dump) (int n, struct List *pos);

	symbol *func_name;
	struct List *expr_l;
};
struct Operation {
	int curr_lineno;
	struct List list;
	int (*dump) (int n, struct List *pos);
	int type;
	node *a;
	node *b;
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
Program_ *program(node *topics);
node *topic_simple(symbol * name, node *features);
node *kpt_feature(Kpt_* k);
node *proc_feature(Proc_ *p);
node *func_feature(Func_ *l);
Kpt_ *kpt_simple(symbol *, symbol *, symbol *, node *); 
Kpt_ *kpt_const(Kpt_ *k, struct List *s);

Func_ * function(symbol *name, node *args, node *stat_l);

node *attr_stat(symbol * a, struct List *con);

node *navig(symbol * topic, symbol * kpt, symbol * attr);

node *label_stat(node *navig, node *n);
node *let_stat(node *n, node *a);
node *case_stat(symbol *name, node *stat_l);
node *connection(node* navig, int type, node *expr);
node *dispatch(symbol *func_name, node *expr_l);
//new functions 
node *operation(node *a, node *b, int type);
node *id_node(symbol * s);
node *string_node(symbol * s);


#endif /*AST_TREE_H */
