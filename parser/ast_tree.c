#include "ast_tree.h"
#include "snl_const.h"
#include "dumptype.h"
#include "snl_stringtab.h"
#include <stdio.h>
#include <stdlib.h>
extern int curr_lineno;
/* here we need to install basic attrs
 * what, when, how, why, where
 */
/* put it in snl.h */

static void initialize_constants(void)
{
	/* fix the exit(1) later */
	int err;
	what  = malloc(sizeof(* what));
	what->len = 4;
	if (err = 
	   add_stringtab(&id_arr, "what", 5, &(what->str)) ) {
		exit(1);
	};

	when  = malloc(sizeof(* when));
	when->len = 4;
	if (err = 
	   add_stringtab(&id_arr, "when", 5, &(when->str)) ) {
		exit(1);
	};

	where = malloc(sizeof(*where));
	where->len = 5;
	if (err = 
	   add_stringtab(&id_arr, "where", 6, &(where->str)) ) {
		exit(1);
	}

	who   = malloc(sizeof(*  who));
	who->len = 3;
	if (err = 
	add_stringtab(&id_arr, "who", 4, &(who->str)) ) {
		exit(1);
	}

	why   = malloc(sizeof(*  why));
	why->len = 3;
	if (err = 
	add_stringtab(&id_arr, "why", 4, &(why->str)) ) {
		exit(1);
	}

	how   = malloc(sizeof(*  how));
	how->len = 3;
	if (err = 
	add_stringtab(&id_arr, "how", 4, &(how->str)) ) {
		exit(1);
	}
}

node *topic_simple(Symbol name, node *features)
{
	Topic_ *t = malloc(sizeof(Topic_));
	t->curr_lineno = curr_lineno;
	t->dump = &dump_topic;

	t->features = features;
	t->name = single_string(name, &id_arr);
	return &(t->list);
}


/* TODO: write these three function into a more generalized function */
node *kpt_feature(Kpt_* k)
{
	return &(k->list);
}

node *proc_feature(Proc_ *p)
{
	return &(p->list);
}

node *list_feature(List_ *l)
{
	return &(l->list);
}

Kpt_ *single_kpt(str_symbol name, 
		 str_symbol aka_name,
		 str_symbol subset_name,
		 node * expr_list)
{
	Kpt_ *k = malloc(sizeof(Kpt_));
	k->curr_lineno = curr_lineno;
	k->name = name;
	k->aka_name = aka_name;
	k->subset_name = subset_name;
	k->attr_l = expr_list;
	k->dump = &(dump_kpt);//not implemented
	return k;
}

Kpt_ *kpt_const(Kpt_ *k, struct List *s)
{
	k->attr_l = s;
	return k;
}

List_ * list_simple(node *n)
{
	List_ * l = malloc(sizeof(List_));
	l->curr_lineno = curr_lineno;
	l->dump = &dump_list;

	l->nlist = n;
	return l;
}
/*leaf*/
node *basic_expr(str_symbol a, struct List *con)
{
	Basic_expr_ *be = malloc(sizeof(Basic_expr_));
	be->curr_lineno = curr_lineno;
	be->attr_name = a;

	/* con is not initialized in id_/string_ node function, 
	 * I better initialize it here
	 */
	con->prev = con->next = con;
	be->con = con;
	be->dump = &dump_expr;
	return &(be->list);
}


node *id_node(Symbol s)
{
	Const_ *cons = malloc(sizeof(Const_));
	cons->curr_lineno = curr_lineno;
	cons->dump = &dump_const;

	cons->type = ID;
	cons->con.id = single_string(s, &id_arr);
	return &(cons->list);
}

node *string_node(str_symbol s)
{
	Const_ *cons = malloc(sizeof(Const_));
	cons->curr_lineno = curr_lineno;
	cons->dump = &dump_const;

	cons->type = STRING;
	cons->con.str = s;
	return &(cons->list);
}

node *navig(Symbol topic, Symbol kpt, str_symbol attr)
{
	int check = 0;
	struct Formal *f = malloc(sizeof(struct Formal));
	f->curr_lineno = curr_lineno;
	f->dump = &dump_navig;

	if (topic == NULL)
		f->topic = NULL;
	else {
		str_symbol t = single_string(topic, &id_arr);
		f->topic = t;
		check += 1;
	}
	if (kpt == NULL) 
		f->kpt = NULL;
	else {
		str_symbol k = single_string(kpt, &id_arr);
		f->kpt = k;
		check += 1;
	}
	f->attr = attr;
	check += (attr == NULL) ? 0 : 1;
	if (check == 0) 
		exit(1);	/* change this */

	return &(f->list);
}


node *formal_navig(node* formal, node *n)
{
	Formal_navig_ *f = malloc(sizeof(Formal_navig_));
	f->curr_lineno = curr_lineno;
	f->dump = &dump_formal_navig;

	f->formal = formal;
	f->alist = n;
	return &(f->list);
}


node *let_navig(node *n, node *a)
{
	Let_navig_ *l = malloc(sizeof(Let_navig_));
	l->curr_lineno = curr_lineno;
	l->dump = &dump_let_navig;

	l->formal_list = a;
	l->alist = a;
	return &(l->list);
}

node *connection(node *formal, int type, Symbol name, node *alist)
{
	struct Conn *conn = malloc(sizeof(struct Conn));
	conn->curr_lineno = curr_lineno;
	conn->dump = &(dump_conn);
	conn->formal = formal;
	conn->type = type; 
	conn->name = single_string(name, &id_arr);
	conn->alist = alist;
	return &(conn->list);
}
