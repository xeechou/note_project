#include <stdio.h>
#include "dumptype.h"
#include "ast_tree.h"
#include "snl_list.h"
#include "snl_const.h"
typedef struct List node;
/* This is for debugging */

static FILE *stream = NULL;
static void init_streams(void) __attribute__((constructor));
static void init_streams(void)
{
	stream = stdout;
}

static char *padding = "                                                                                ";      // 80 spaces for padding

/* pad function will point to the exact position of padding,
 * then return the position */
static inline char *pad(int n)
{
	if (n > 80) 
		return padding;
	if (n <= 0)
		return "";
	else
		return padding + (80 - n);
}


/* this is the key factor to implement dump familys,
 * it can replace 'this' in C++
 */
typedef struct {
	struct List list;
	debug_info node;
} node_struct;


void dump_line(int n, int lineno)
{
	char *p = pad(n);
	fprintf(stream, "%s #%d\n", p, lineno);
}

void dump_symbol(int n, symbol *sym)
{
	char *p = pad(n);
	fprintf(stream, "%s #%s\n", p, sym->str);		
}


/* the dump_type function family goes as follow
 * quite straight forward
 *
 * int dump_type(int n, node *pos)
 * {
 *	//get type 
 *	type *t = list_entry(pos, type, list);
 *
 *	//print line
 *	dump_line(n, t->curr_lineno);
 *
 *	//dump some symbols
 *	dump_symbol(...);
 *	...
 *
 *	//get the node the do while loop to dump subnodes
 * }
 */

int dump_program(Program_ *p)
{
	node *guard, *f;
	guard = f = p->topic_list;
	if (!f)
		return 0;
	do {
		dump_topic(0, f);
		f = f->next;
	} while (f != guard);
	return 0;
}

int dump_topic(int n, node *pos)
{
	Topic_ *t = (Topic_ *) pos;
	dump_line(n, t->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _topic\n", p);
	dump_symbol(n+2, t->name);

	/* the dump pointer function */
	node *guard, *f;
	guard = f = t->features;
	if (!f)
		return 0;
	do {
		node_struct *feature = (node_struct *) f; 
		if ( feature->node.dump (n+2, &(feature->list)))
			/* do somthing? */;
		f = f->next;
	} while(f != guard);
	return 0;
}
/* TODO: fix the line below */
int dump_kpt(int n, node *pos)
{
	Kpt_ *k = (Kpt_ *) pos;
	dump_line(n, k->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _kpt\n", p);
	dump_symbol(n+2, k->name);

	if (k->aka_name)
		dump_symbol(n+2, k->aka_name);

	node *guard, *a;
	guard = a = k->attr_l;
	if (!a)
		return 0;
	do {
		node_struct *attr = (node_struct *) a;
		if ( attr->node.dump (n+2, &(attr->list)) )
		/*do something*/;
		a = a->next;
	} while (a != guard);
	return 0;
}
int dump_func(int n, node *pos)
{
	Func_ *f = (Func_ *)pos;
	dump_line(n, f->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _Func\n", p);
	dump_symbol(n+2, f->name);

	//dump_args
	if (f->args == NULL)
		return 0;
	else {
		node *guard, *a;
		guard = a = f->args;
		do {
			dump_const(n+2, a);
		} while (a != guard);
	}
	return 0;
}

/* dump the basic expr */
int dump_attr(int n, node *pos)
{
	Attr_Stat_ *be = (Attr_Stat_ *)pos;
	dump_line(n, be->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _attr_stat\n", p);
	
	dump_symbol(n+2, be->attr_name);
	if (be->expr) {
		//dump the expr
		node_struct *e = (node_struct *) be->expr;
		e->node.dump(n+2, be->expr); 
	}
	return 0;
}
/* it could either called in id_list or const */
int dump_const(int n, node *pos)
{
	Const_ *c = (Const_ *)pos;
	dump_line(n, c->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _const\n",p);

	if (c->type == ID) {
		dump_symbol(n+2, (c->con).id);
	} else {
		dump_symbol(n+2, (c->con).str);
	}
	return 0;
}
int dump_navig(int n, node *pos)
{
	Navig_ *f = (Navig_ *) pos;

	char *p = pad(n);
	fprintf(stream, "%s _navig\n",p);
	p = pad(n+2);
	if (f->topic)
	fprintf(stream, "%s _topic\n", p);
	dump_symbol(n, f->topic);

	if (f->kpt)
	fprintf(stream, "%s _kpt\n",p);
	dump_symbol(n, f->kpt);
	if (f->attr)
	fprintf(stream, "%s _attr\n",p);
	dump_symbol(n, f->attr);
	return 0;
}
int dump_label_stat(int n, node *pos)
{
	Label_Stat_ *fn = (Label_Stat_ *) pos;
	dump_line(n, fn->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _label_Stat\n",p);

	/* dump navig */
	dump_navig (n+2, fn->navig);

	node_struct *attr = (node_struct *)fn->expr;
	attr->node.dump (n+2, &(attr->list));
	return 0;
}


int dump_let_stat(int n, node *pos)
{
	Let_Stat_ *ln = (Let_Stat_ *) pos;
	dump_line(n, ln->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _let_Stat\n", p);

	//dump navig_list
	if (!pos)
		return 0;
	node *guard, *fl;
	guard = fl = ln->navig_list;
	if (fl) {
		do {
			dump_navig(n+2, fl);
			fl = fl->next;
		} while (fl != guard);
	}
	/* fl here stands for alist */
	fl = ln->expr;
	node_struct *expr;
	if (fl) {
		expr = (node_struct *) fl;
		expr->node.dump (n+2, &(expr->list));
	}
	return 0;
}
int dump_case_stat(int n, node *pos)
{
	Case_Stat_ *c = (Case_Stat_ *)pos;
	dump_line(n, c->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _Case_Stat\n", p);

	dump_symbol(n+2, c->name);

	//forget about the statement, I'm tired
	return 0;
}

int dump_conn(int n, node *pos)
{
	Conn_ *c = (Conn_ *) pos;
	dump_line(n, c->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s %d_conn\n", p, c->type);

	//dump navig
	dump_navig (n+2, c->navig);
	node_struct *s;
	if (c->expr) {
		s = (node_struct *) (c->expr);
		s->node.dump(n+2, &(s->list));
	}
	return 0;
}
int dump_dispatch(int n, node *pos)
{
	Dispatch_ *d = (Dispatch_ *) pos;
	dump_line(n, d->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _dispatch\n", p);
	//again, I'm so tired of it
	return 0;

}
int dump_operation(int n, node *pos)
{
	Operation_ *o = (Operation_ *) pos;
	dump_line(n, o->node.curr_lineno);

	char *p = pad(n);

	fprintf(stream, "%s %d_operation\n", p, o->type);
	//dump expr a
	node_struct *expr;
	expr = (node_struct *) o->a;
	expr->node.dump (n+2, &(expr->list));
	//dump expr b
	expr = (node_struct *) o->b;
	expr->node.dump (n+2, &(expr->list));
	return 0;
}

int dump_proc(int n, node *pos) 
{
	Proc_ *proc = (Proc_ *)pos;
	dump_line(n, proc->node.curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _procedure\n", p);
	
	dump_symbol(n+2, proc->name);
	//dump steps
	node *pl = proc->proc_l;
	do {
		dump_step(n+2, pl);
		pl = pl->next;
	} while (pl != proc->proc_l);
	return 0;
}
int dump_step(int n, node *pos) 
{
	Proc_Component_ *pc = (Proc_Component_ *)pos;
	symbol *str = NULL;

	char *p = pad(n);
	fprintf(stream, "%s %d\n", p, pc->type);
	switch (pc->type) {
		case PC_STR:
			str = ((Const_ *)pc->c0)->con.str;
			dump_symbol(n+2, str);
			break;
		case PC_IF:
		case PC_LOOP:
			dump_step(n+2, pc->c0);
			dump_step(n+2, pc->c1);
			break;
		case PC_IF_ELSE:
			dump_step(n+2, pc->c0);
			dump_step(n+2, pc->c1);
			dump_step(n+2, pc->c2);
	}
	return 0;
}
