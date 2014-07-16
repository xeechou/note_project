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
typedef struct Node_struct{
	int curr_lineno;
	struct List list;
	int (*dump)(int n, struct List *pos);
} node_struct;


void dump_line(int n, int lineno)
{
	char *p = pad(n);
	fprintf(stream, "%s #%d\n", p, lineno);
}

void dump_symbol(int n, str_symbol sym)
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
	Topic_ *t = list_entry(pos, Topic_, list);
	dump_line(n, t->curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _topic\n", p);
	dump_symbol(n+2, t->name);

	/* the dump pointer function */
	node *guard, *f;
	guard = f = t->features;
	if (!f)
		return 0;
	do {
		node_struct *feature = list_entry(f ,node_struct, list);
		dump_type = feature->dump;
		if ((*dump_type) (n+2, &(feature->list)))
			/* do somthing? */;
		f = f->next;
	} while(f != guard);
	return 0;
}

int dump_kpt(int n, node *pos)
{
	Kpt_ *k = list_entry(pos, Kpt_, list);
	dump_line(n, k->curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _kpt\n", p);
	dump_symbol(n+2, k->name);

	if (k->aka_name)
		dump_symbol(n+2, k->aka_name);
	if (k->subset_name)
		dump_symbol(n+2, k->subset_name);

	node *guard, *a;
	guard = a = k->attr_l;
	if (!a) //a stands for attr
		return 0;
	do {
		node_struct *attr = list_entry(a, node_struct, list);
		dump_type = attr->dump;
		if ( (*dump_type) (n+2, &(attr->list)) )
		/*do something*/;
		a = a->next;
	} while (a != guard);
	return 0;
}

int dump_list(int n, node *pos)
{
	List_ *l = list_entry(pos, List_, list);
	dump_line(n, l->curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _list\n", p);

	node *guard, *nl;
	guard = nl = l->nlist;
	if (!nl)
		return 0;
	do {
		node_struct *expr = list_entry(nl, node_struct, list);
		dump_type = expr->dump;
		if ((*dump_type) (n+2, &(expr->list)));
		nl = nl->next;
	} while (nl != guard);
	return 0;
}
/* dump the basic expr */
int dump_expr(int n, node *pos)
{
	Basic_expr_ *be = list_entry(pos, Basic_expr_, list);
	dump_line(n, be->curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _basic_expr\n", p);
	
	/* leaves */
	dump_symbol(n+2, be->attr_name);
	/* basic expr doesn't allow const list, but single const is just
	 * list, but we don't need a loop here
	 */
	if (be->con)
		dump_const(n+2, be->con);
}

int dump_const(int n, node *pos)
{
	Const_ *c = list_entry(pos, Const_, list);
	dump_line(n, c->curr_lineno);

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
	Formal_ *f = list_entry(pos, Formal_, list);

	char *p = pad(n);
	fprintf(stream, "%s _formal\n",p);
	p = pad(n+2);
	if (f->topic);
	fprintf(stream, "%s _topic\n", p);
	dump_symbol(n, f->topic);

	if (f->kpt);
	fprintf(stream, "%s _kpt\n",p);
	dump_symbol(n, f->kpt);
	if (f->attr);
	fprintf(stream, "%s _attr\n",p);
	dump_symbol(n, f->attr);
	return 0;
}
int dump_formal_navig(int n, node *pos)
{
	Formal_navig_ *fn = list_entry(pos, Formal_navig_, list);
	dump_line(n, fn->curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _formal_navig\n",p);

	/* dump formal */
	dump_navig (n+2, fn->formal);
	node *guard, *al;
       	guard = al = fn->alist;
	if (!al)
		return 0;
	do {
		node_struct *attr = list_entry(al, node_struct, list);
		dump_type = attr->dump;
		if ((*dump_type) (n+2, &(attr->list)))
			;
		al = al->next;
	} while (al != guard);
	return 0;
}


int dump_let_navig(int n, node *pos)
{
	Let_navig_ *ln = list_entry(pos, Let_navig_, list);
	dump_line(n, ln->curr_lineno);

	char *p = pad(n);
	fprintf(stream, "%s _let_navig\n", p);

	//dump formal_list
	node *guard, *fl;
	guard = fl = ln->formal_list;
	if (fl) {
		do {
			//node_struct *formal_list = list_entry(fl, node_struct, list);
			//dump_type = formal_list->dump;
			//(*dump_type) (n+2, &(fl->list));
			dump_navig(n+2, fl);
			fl = fl->next;
		} while (fl != guard);
	}
	/* fl here stands for alist */
	guard = fl = ln->alist;
	if (fl) {
		do {
			node_struct *alist = list_entry(fl, node_struct, list);
			dump_type = alist->dump;
			(*dump_type) (n+2, &(alist->list));
			fl = fl->next;
		}while (fl != guard);
	}
	return 0;
}

int dump_conn(int n, node *pos)
{
	Conn_ *c = list_entry(pos, Conn_, list);
	dump_line(n, c->curr_lineno);

	char *p = pad(n);

	fprintf(stream, "%s %d_conn\n", c->type, p);

	//dump formal
	dump_navig (n+2, c->formal);
	//dump OBJECID
	dump_symbol(n+2, c->name);

	dump_const (n+2, c->constant);
}
