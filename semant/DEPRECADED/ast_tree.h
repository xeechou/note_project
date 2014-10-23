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

typedef struct List node; 

typedef struct Program Program_; 
typedef struct Topic Topic_;
typedef struct Feature Feature_;
typedef struct Kpt Kpt_;
typedef struct Func Func_;
typedef struct Proc Proc_;
typedef struct Proc_Component Proc_Component_;
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
 */

/* TODO: use debug_info in this way :
 * struct some_struct {
 * 	struct debug_info;
 * 	type data;
 * 	...
 * };
 * This mechanism may increase the complexity of dump function */
/* this is the key hack to replace in the parser struct */
typedef struct {
	int curr_lineno;
	int (*dump) (int n, struct List *pos);
} debug_info;

struct Program {
	int curr_lineno;
	struct List *topic_list;	/* which points to the first list in Topic_ */
};

struct Topic {
	struct List list;
	debug_info node;

	symbol * name;
	struct List *features;
};

struct Kpt {
	struct List list;
	debug_info node;

	struct List *attr_l;
	symbol * name;
	symbol * aka_name;
	struct List *subsets;
};

struct Func {
	struct List list;
	debug_info node;

	//function is sequences of statements
	symbol *name;
	struct List *args;
	struct List *stat_l;
};

struct Proc {
	struct List list;
	debug_info node;
	
	symbol *name;
	struct List *proc_l;
};

struct Proc_Component {
	struct List list;
	debug_info node;

	int type;	
	struct List *c0;
	struct List *c1;
	struct List *c2;
};


struct Attr_Stat {
	struct List list;
	debug_info node;

	symbol * attr_name;
	struct List *expr;
};

/****** Statation options ******/

struct Label_Stat {
	struct List list;
	debug_info node;

	node *navig;
	struct List *expr;
};

struct Let_Stat {
	struct List list;
	debug_info node;

	struct List *navig_list;
	struct List *expr;
};

struct Case_Stat {
	struct List list;
	debug_info node;

	symbol *name;
	struct List *stat_l;

};
/******* Statation options ****/







/******* expressions ****/
struct Const {
	struct List list;
	debug_info node;

	int type;
	union {
		symbol * id;
		Proc_  *proc;
		symbol * str;
	} con;
};

struct Navig {
	struct List list;
	debug_info node;

	int in_function;
	symbol * topic;
	symbol * kpt;
	symbol * attr;
};
/*checked */
struct Conn {
	struct List list;
	debug_info node;

	int type;
	node* navig;

	struct List *expr;
};
struct Dispatch {
	struct List list;
	debug_info node;

	symbol *func_name;
	struct List *expr_l;
};
struct Operation {
	struct List list;
	debug_info node;

	int type;
	node *a;
	node *b;
};
/* the widely used function... */
static inline node *nil_node(void)
{
	return NULL;
}
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

static inline void info_debug(debug_info *node, 
			     int lineno, 
			     int (*dump) (int n, struct List *pos))
{
	node->curr_lineno = lineno;
	node->dump = dump;
}
/* this struct is used for uniform alloc */
typedef union {
	Program_ program;
	Topic_ topic;
	Kpt_ kpt;
	Func_ func;
	Proc_ proc;
	Attr_Stat_ attr;
	Label_Stat_ label;
	Let_Stat_ let;
	Conn_ conn;
	Dispatch_ dispatch;
	Operation_ oper;
	Const_ con;
	Navig_ navig;
} node_union;

/*functions */
Program_ *program(node *topics);
node *topic_simple(symbol * name, node *features);
node *kpt_feature(Kpt_* k);
node *proc_feature(Proc_ *p);
node *func_feature(Func_ *l);
Kpt_ *kpt_simple(symbol *, symbol *, node *); 
Kpt_ *kpt_const(Kpt_ *k, struct List *s);

Func_ * function(symbol *name, node *args, node *stat_l);
Proc_ *procedure(symbol *name, node *proc_l);
node *step(int type, node *c0, node *c1, node *c2);

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
