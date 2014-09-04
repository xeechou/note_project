#include "ast_tree.h"
#include "snl_const.h"
#include "dumptype.h"
#include <stdlib.h>
/* global constant */
int curr_lineno;
int in_function = 0;

/* TODO:
 * -for now, every struct is alloced directly by malloc, use the smalloc or
 *  slots struct to manage them, since we only alloc and free them altogether.
 * 
 * -use union to compute the biggest memory of all the struct, so we can use
 *  slot to manage it, although this method will waste some memory.
 */
Program_ *program(node *topics)
{
	Program_ *p = malloc(sizeof(Program_));
	p->topic_list = topics;
	return p;
}
node *topic_simple(symbol * name, node *features)
{
	Topic_ *t = malloc(sizeof(Topic_));
	info_debug(&t->node, curr_lineno, dump_topic);

	t->features = features;
	t->name = name;
	return &(t->list);
}


node *kpt_feature(Kpt_* k)
{
	return &(k->list);
}

node *proc_feature(Proc_ *p)
{
	return &(p->list);
}

node *func_feature(Func_ *l)
{
	return &(l->list);
}

Kpt_ *kpt_simple(symbol *name, 
		 symbol *aka_name,
		 node *subsets)
{
	Kpt_ *k = malloc(sizeof(Kpt_));
	info_debug(&k->node, curr_lineno, dump_kpt);

	k->name = name;
	k->aka_name = aka_name;
	k->subsets = subsets;
	k->attr_l = NULL;
	return k;
}

Kpt_ *kpt_const(Kpt_ *k, struct List *s)
{
	k->attr_l = s;
	return k;
}

Func_ * function(symbol *name, node *args, node *stat_l)
{
	Func_ * f = malloc(sizeof(Func_));
	info_debug(&f->node, curr_lineno, dump_func);

	f->name = name;
	f->stat_l = stat_l;

	f->args = args;
	return f;
}
/*leaf*/
node *attr_stat(symbol * a, struct List *expr)
{
	Attr_Stat_ *be = malloc(sizeof(Attr_Stat_));
	info_debug(&be->node, curr_lineno, dump_attr);

	be->attr_name = a;
	be->expr = expr;
	return &(be->list);
}



node *label_stat(node* navig, node *n)
{
	Label_Stat_ *f = malloc(sizeof(Label_Stat_));
	info_debug(&f->node, curr_lineno, dump_label_stat);

	f->navig = navig;
	f->expr = n;
	return &(f->list);
}


node *let_stat(node *n, node *expr)
{
	Let_Stat_ *l = malloc(sizeof(Let_Stat_));
	info_debug(&l->node, curr_lineno, dump_let_stat);

	l->navig_list = n;
	l->expr = expr;
	return &(l->list);
}

node *case_stat(symbol *name, node *stat_l)
{
	Case_Stat_ *c = malloc(sizeof(Case_Stat_));
	info_debug(&c->node, curr_lineno, dump_case_stat);

	c->name = name;
	c->stat_l = stat_l;
	return &(c->list);
}
/* checked */
node *connection(node *navig, int type, node *expr)
{
	Conn_ *c = malloc(sizeof(struct Conn));
	info_debug(&c->node, curr_lineno, dump_conn);

	c->navig = navig;
	c->type = type; 
	c->expr = expr;
	return &(c->list);
}

node *dispatch(symbol *func_name, node *expr_l)
{
	Dispatch_ *d = malloc(sizeof(Dispatch_));
	info_debug(&d->node, curr_lineno, dump_dispatch);

	d->func_name = func_name;
	d->expr_l = expr_l;
	return &(d->list);
}
node *operation(node *a, node *b, int type)
{
	Operation_ *o = malloc(sizeof(Operation_));
	info_debug(&o->node, curr_lineno, dump_operation);

	o->type = type;
	o->a = a;
	o->b = b;
	return &(o->list);
}

/*checked */
node *navig(symbol * topic, symbol * kpt, symbol * attr)
{
	Navig_ *f = malloc(sizeof(struct Navig));
	info_debug(&f->node, curr_lineno, dump_navig);

	f->in_function = in_function;
	f->topic = topic;
	f->kpt = kpt;
	f->attr = attr;

	return &(f->list);
}

node *id_node(symbol * s)
{
	Const_ *cons = malloc(sizeof(Const_));
	info_debug(&cons->node, curr_lineno, dump_const);

	cons->type = ID;
	cons->con.id = s;
	return &(cons->list);
}

node *string_node(symbol * s)
{
	Const_ *cons = malloc(sizeof(Const_));
	info_debug(&cons->node, curr_lineno, dump_const);

	cons->type = STRING;
	cons->con.str = s;
	return &(cons->list);
}

Proc_ *procedure(symbol *name, node *proc_l)
{
	Proc_ *p = malloc(sizeof(Proc_));
	info_debug(&p->node, curr_lineno, dump_proc);

	p->name = name;
	p->proc_l = proc_l;
	return p;
}
node *step(int type, node *c0, node *c1, node *c2)
{
	Proc_Component_ *pc = malloc(sizeof(Proc_Component_));
	info_debug(&pc->node, curr_lineno, dump_step);

	pc->type = type;/* string	IF 	IF_ELSE	WHILE  	*/
	pc->c0 = c0;	/* string	pc	pc	pc	*/
	pc->c1 = c1;	/* NULL		pc	pc	pc	*/
	pc->c2 = c2;	/* NULL		NULL	pc	NULL	*/
	return &pc->list;
}
