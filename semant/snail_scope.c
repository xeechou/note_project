#include "misc/misc_types.h"
#include "snail.h"
#include "snail_scope.h"
/* XXX:This file has not been tested!!! */

/* these tree globals appears needed for type checking */
static symbol *curr_topic;
static symbol *curr_kpt;
static symbol *curr_func;
static symbol *curr_attr;

void scope_init(scope *s)
{
	stack_init(&s->env_stack, sizeof(type_env), 4, NULL, NULL);

	err_pstrt_init(&s->items, sizeof(scope_item), 8192, NULL);
}

void scope_enter(scope *s, type_env *newenv)
{
	newenv->len = 0;
	newenv->head = NULL;
	stack_top(&s->env_stack, &newenv->parent);

	stack_push(&s->env_stack, (void *)newenv);
}
void scope_exit(scope *s)
{
	void *trash;
	scope_item *head = (s->env_stack).head;
	//delete all the items in the env
	while (head) {
		scope_item *i = head->next;
		pstrt_delete(&s->items, head);
		head = i;
	}
	stack_pop(&s->env_stack, &trash);
}

/* insert at end of link to make process faster */
scope_item *scope_add_item(scope *s, type_env *cur_env, scope_item *item)
{
	item->next = cur_env->head;
	pstrt_insert(&s->items, item, (void **)&cur_env->head);
	cur_env->len += 1;
	return cur_env->head;
}

/* the function is both for find and remove */
scope_item *scope_op_item(type_env *env, symbol *a, int opt)
{
	scope_item **i, ret; 		//ret can be a optimize option
	for (i = &env->head; *i != NULL;) {
		scope_item *entry = *i;
		if (entry->name == a) {
			if (SCOPE_RM == (OPER_OPT & opt)) //remove
				*i = entry->next;
			return entry;
		}
		else
			i = &entry->next;
	}
	if (	env->parent != NULL && 
		(SCOPE_REC == (opt & SCOPE_REC)) )
		return scope_op_item(env->parent, a, opt);
	else
		return NULL;
}

/*XXX:*** test use data *****/
static ohash_tab global_tab;


/************************* things done by preparing step:
 ************************* *adding all the global information in.
 ************************* *checking if all the ids are unique.
 ************************* */
/*assume ht has been initialized */
/* don't check TOPIC repeatation, SNAIL allows that */
static page_strt *ast_nodes;
static page_strt *nodes_data;
static inline int assert_data(node_data *n, int type)
{
	return (n->type == type) ? 1 : 0;
}
static global *prepare_topic(ohash_tab *ht, ast_node *t);
static global *prepare_feature(ohash_tab *ht, ast_node *n);
static global *prepare_kpt(ohash_tab *ht, ast_node *kpt);
static global *prepare_func(ohash_tab *ht, ast_node *func);
static global *prepare_attr(ohash_tab *ht, ast_node *attr);
static global *prepare_proc(ohash_tab *ht, ast_node *proc);
static global *prepare_args(ohash_tab *ht, ast_node **arg);

void prepare_globals(ohash_tab *ht, ast_node *prog)
{
	//there is no way topic_list to be empty
	node_data *nd = n_node(prog, OD_PROG_TOP);
	ast_node *topic = nd->n;
	global *t;
	if (!topic)
		return;
	//check all the topics, let's say there are at least one topic 
	do {
		t = prepare_topic(ht, topic);
		//do somewhat check on t
		topic = topic->next;
	} while (topic != nd->n);
}

static global *do_while_check (ohash_tab *ht, ast_node *start, node_data *head,
		global *(* prepare_func)(ohash_tab *, ast_node *))
{
	ast_node *next, *f = start;
	global *result;
	global *last_result = NULL;
	do {
		next = f->next;
		if ((result = prepare_func(ht, f)) == NULL)
			rm_ast_node(f, head, ast_nodes);

		 else {
			result->next = last_result;
			last_result = result;
		}
		f = next;
	} while (next != start && head->n != NULL);
	return result;
}
static global *prepare_topic(ohash_tab *ht, ast_node *t)
{
	curr_topic = n_node(t, OD_TOPIC_NAME)->sym;
	global g, *ret;
	g.topic = curr_topic;
	g.feature = NULL;
	g.attr = NULL;
	g.data = NULL;
	g.type = TP_NONE;
	//we don't check the topic collision, that is allowed in SNAIL
	oht_lookup(ht, &g, (void **)&ret, HT_CRE);

	node_data *head = n_node(t, OD_TOPIC_FEAT);
	ast_node *feats = head->n;
	if (feats == NULL)
		return ret;
	//we add a chk variable to avoid another if statement
	global *chk = 
		do_while_check(ht, feats, head, prepare_feature);
	chk->next = ret->data;
	ret->data = chk;
	return ret;
}

static global *prepare_feature(ohash_tab *ht, ast_node *n)
{
	global *ret;
	switch(n->items->type) {
		case TP_KPT:
			ret = prepare_kpt(ht, n);
			break;
		case TP_FUNC:
			ret = prepare_func(ht, n);
			break;
		case TP_PROC:
			ret = prepare_proc(ht, n);
			break;
	}
	return ret;
}

/* the speciality of prepare_kpt is that it has aka name */
static global *prepare_kpt(ohash_tab *ht, ast_node *kpt)
{
	curr_kpt = n_node(kpt, OD_KPT_NAME)->sym;
	global g, *ret, *ret2;
	g.topic = curr_topic;
	g.feature = curr_kpt;
	g.attr = NULL;
	g.type = TP_KPT;
	oht_lookup(ht, &g, (void **)&ret, HT_NOC);
	if (ret) {
		fprintf(stderr, "ERROR: Redeclaration of KPT %s\n", 
				sym_genstr(curr_kpt));
		return NULL;
	} else {
		oht_lookup(ht, &g, (void **)ret, HT_CRE);
	}
	symbol *aka_name = n_node(kpt, OD_KPT_AKA)->sym;
	if (aka_name) {
		g.feature = aka_name;
		oht_lookup(ht, &g, (void **)&ret2, HT_NOC);
		if (ret2) {
			fprintf(stderr, "ERROR: Redeclaration of KPT %s\n", 
					sym_genstr(aka_name));
			return NULL;
		} else {
			oht_lookup(ht, &g, (void **)ret, HT_CRE);
		}
	}
	//TODO: do subsets check
	//scan attrs
	node_data *head = n_node(kpt, OD_KPT_ATTRS);
	ast_node *attr = head->n;
	if (!attr)
		return ret;
	else {
		ret->data = do_while_check(ht, attr, head, prepare_attr);
		ret2->data = ret->data;
	}
	return ret;
}
static global *prepare_attr(ohash_tab *ht, ast_node *attr)
{
	global g, *ret;
	curr_attr = n_node(attr, OD_ATTR_NAME)->sym;
	g.topic = curr_topic;
	g.feature = curr_kpt;
	g.attr = curr_attr;
	g.type = TP_ATTR;
	g.data = NULL;

	oht_lookup(ht, &g, (void **)&ret, HT_NOC);
	if (ret == NULL)
		oht_lookup(ht, &g, (void **)&ret, HT_NOC);
	else
		ret = NULL;
	return ret;
}
static global *prepare_func(ohash_tab *ht, ast_node *func)
{
	global g, *ret;
	curr_func = n_node(func, OD_FUNC_NAME)->sym;
	g.topic = curr_topic;
	g.feature = curr_func;
	g.attr = NULL;
	g.type = TP_NONE;
	oht_lookup(ht, &g, (void **)&ret, HT_NOC);
	if (ret) {
		fprintf(stderr, "ERROR: Redeclaration of function %s\n",
				sym_genstr(curr_func));
		return NULL;
	} else {
		oht_lookup(ht, &g, (void **)&ret, HT_CRE);
	}
	node_data *head = n_node(func, OD_FUNC_ARG);
	ast_node *arg = head->n;
	if (!arg)
		return ret;
	else 
		ret->data = prepare_args(ht, &arg);
	return ret;
}
/* args is special, coz we store args directly as node_data list */
static global *prepare_args(ohash_tab *ht, ast_node **args)
{
	global g;
	node_data *head = (*args)->items;
	curr_attr = head->sym;
	g.topic = curr_topic;
	g.feature = curr_kpt;
	g.attr = curr_attr;
	g.type = TP_NONE;
	g.data = NULL;
	node_data *a = head;
	node_data *prev;		//use prev coz arguments order is important
	global *ret, *cur = NULL;
	do {
		prev = a->prev;		//prepare the next node in the first
					//place so we can avoid a if statement
		oht_lookup(ht, &g, (void **)&ret, HT_NOC);
		if (ret == NULL) {
			oht_lookup(ht, &g, (void **)&ret, HT_CRE);
			ret->next = cur;	//this method adds node in reverse order
			cur = ret;
		}
		else {
			fprintf(stderr, "repeated function arguments %s\n",
			       		sym_genstr(g.attr));
			rm_node_data(a, &head, nodes_data);
		}
		a = prev;
		g.attr = a->sym;
	} while (a != head && head != NULL);
	if (head == NULL)
		*args = NULL;
	return ret;
}
static global *prepare_proc(ohash_tab *ht, ast_node *proc)
{
	global g, *ret;
	g.topic = curr_topic;
	g.feature = n_node(proc, OD_PROC_NAME)->sym;
	g.attr = NULL;

	oht_lookup(ht, &g, (void **)&ret, HT_NOC);
	if (ret == NULL)
		oht_lookup(ht, &g, (void **)&ret, HT_CRE);
	else 
		fprintf(stderr, "repeated proc arguments %s\n",
				sym_genstr(g.feature));
	return ret;
}
