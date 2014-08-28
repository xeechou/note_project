#include "ast_tree.h"
#include "snl_const.h"
#include "dumptype.h"
#include <stdlib.h>
/* global constant */
int curr_lineno;
int in_function = 0;


Program_ *program(node *topics)
{
	Program_ *p = malloc(sizeof(Program_));
	p->topic_list = topics;
	return p;
}
node *topic_simple(symbol * name, node *features)
{
	Topic_ *t = malloc(sizeof(Topic_));
	t->curr_lineno = curr_lineno;
	t->dump = &dump_topic;

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
		 symbol *subset_name,
		 node * attr_l)
{
	Kpt_ *k = malloc(sizeof(Kpt_));
	k->curr_lineno = curr_lineno;
	k->name = name;
	k->aka_name = aka_name;
	k->subset_name = subset_name;
	k->attr_l = attr_l;
	k->dump = &(dump_kpt);//not implemented
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
	f->curr_lineno = curr_lineno;
	f->dump = &dump_func;

	f->name = name;
	f->stat_l = stat_l;

	f->args = args;
	return f;
}
/*leaf*/
node *attr_stat(symbol * a, struct List *expr)
{
	Attr_Stat_ *be = malloc(sizeof(Attr_Stat_));
	be->curr_lineno = curr_lineno;
	be->attr_name = a;

	be->expr = expr;
	be->dump = &dump_attr;
	return &(be->list);
}



node *label_stat(node* navig, node *n)
{
	Label_Stat_ *f = malloc(sizeof(Label_Stat_));
	f->curr_lineno = curr_lineno;
	f->dump = &dump_label_stat;

	f->navig = navig;
	f->expr = n;
	return &(f->list);
}


node *let_stat(node *n, node *expr)
{
	Let_Stat_ *l = malloc(sizeof(Let_Stat_));
	l->curr_lineno = curr_lineno;
	l->dump = &dump_let_stat;

	l->navig_list = n;
	l->expr = expr;
	return &(l->list);
}

node *case_stat(symbol *name, node *stat_l)
{
	Case_Stat_ *c = malloc(sizeof(Case_Stat_));
	c->curr_lineno = curr_lineno;
	c->dump = &dump_case_stat;

	c->name = name;
	c->stat_l = stat_l;
	return &(c->list);
}
/* checked */
node *connection(node *navig, int type, node *expr)
{
	Conn_ *c = malloc(sizeof(struct Conn));
	c->curr_lineno = curr_lineno;
	c->dump = &(dump_conn);
	c->navig = navig;
	c->type = type; 
	c->expr = expr;
	return &(c->list);
}

node *dispatch(symbol *func_name, node *expr_l)
{
	Dispatch_ *d = malloc(sizeof(Dispatch_));
	d->curr_lineno = curr_lineno;
	d->dump = &dump_dispatch;

	d->func_name = func_name;
	d->expr_l = expr_l;
	return &(d->list);
}
node *operation(node *a, node *b, int type)
{
	Operation_ *o = malloc(sizeof(Operation_));
	o->curr_lineno = curr_lineno;
	o->dump = &dump_operation;
	o->type = type;
	o->a = a;
	o->b = b;
	return &(o->list);
}

/*checked */
node *navig(symbol * topic, symbol * kpt, symbol * attr)
{
	Navig_ *f = malloc(sizeof(struct Navig));
	f->curr_lineno = curr_lineno;
	f->dump = &dump_navig;

	f->in_function = in_function;
	f->topic = topic;
	f->kpt = kpt;
	f->attr = attr;

	return &(f->list);
}

node *id_node(symbol * s)
{
	Const_ *cons = malloc(sizeof(Const_));
	cons->curr_lineno = curr_lineno;
	cons->dump = &dump_const;

	cons->type = ID;
	cons->con.id = s;
	return &(cons->list);
}

node *string_node(symbol * s)
{
	Const_ *cons = malloc(sizeof(Const_));
	cons->curr_lineno = curr_lineno;
	cons->dump = &dump_const;

	cons->type = STRING;
	cons->con.str = s;
	return &(cons->list);
}
