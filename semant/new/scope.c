
/* screw it, ALL THE TOPICS, KPTS, FUNCS, PROCS, ATTRS should be exposed to
 * whole file, that is the only way to do type checking!!! 
 * If you chose to use a, then there will be a three hierarchies. You still
 * need slots for storage. OR using hash table, then you will need a special
 * hash function. My decision is hash tables */

static global *prepare_topics(ohash_tab *ht, Topic_ *t)
{
	curr_topic = t->name;
	global g, *feature, *next, *ret;
	g.topic = curr_topic;
	g.feature = NULL;
	g.attr = NULL;
	g.type = TP_TOPIC;
	//g.data = (void *)t->features;
	ret = oht_lookup(ht, &g, 1);
	if (t->features == NULL)
		return ret;
	node *f = t->features;
	next = NULL;
	do {
		if ((feature = prepare_features(ht, f)) == NULL) 
		//this means we meet a bad feature, just getover with it
		{
			//remove the f from features. So this step does
			//reproduce a healthier AST tree.
			node_rm(f);	//this step is also dangerous, it will
					//fail when we try to rm first node of
					//feature node of topic
			continue;	
					
		}
		feature->next = next;
		next = feature;
		f = (node *)f->list.next;
	} while(f != t->features);
	ret->data = feature;
	return ret;

}
static global *prepare_features(ohash_tab *ht, node *n)
{
	global *ret;
	switch(n->node.type) {
		case TP_TOPIC:
			ret = prepare_topic(ht, (Kpt_ *)n);
			break;
		case TP_FUNC:
			ret = prepare_func(ht, (Func_ *)n);
			break;
		case TP_PROC:
			ret = prepare_proc(ht, (Proc_ *)n);
			break;
	}
	return ret;
}
/* don't check repeat for attr, coz they allowed ASSIGN_A */
static global *prepare_attr(ohash_tab *ht, Attr_Stat_ *at)
{
	gloabl g, *ret;
	curr_attr = at->attr_name;
	g.topic = curr_topic;
	g.feature = curr_kpt;
	g.attr = curr_attr;
	g.type = TP_ATTR;
	g.data = NULL;
	return oht_lookup(ht, &g, 1);

}
/* Don't we check repeatation? */
static global *prepare_kpt(ohash_tab *ht, Kpt_ *k)
{
	global g, *next, *attr, *ret, *ret2;
	curr_kpt = k->name;
	g.topic = curr_topic;
	g.feature = curr_kpt;
	g.attr = NULL;
	g.type = TP_KPT;
	//g.data = (void *)k->subsets;
	ret = oht_lookup(ht, &g, 0);
	if (ret) { 	//redeclaration of kpt
		//XXX: replace with error function 
		fprintf(stderr, "ERROR: Redeclaration of KPT %s\n", curr_kpt->str);
		return NULL;	//then we stop checking
	}
	if (k->aka_name) {
		g.feature = k->aka_name;
		ret2 = oht_lookup(ht, &g, 1);
	}
	if (t->attrs == NULL)
		return ret;
	node *a = t->subsets;
	next = NULL;
	do {
		attr = prepare_attrs(ht, (Attr_Stat_ *)a);
		attr->next = next;
		next = attr;
		a = (node *)a->list.next;
	} while (a != t->subsets);
	ret->data = ret2->data = attr;
	return ret;
}
/* check dup!!! */
static global *prepare_args(ohash_tab *ht, Const_ *arg)
{
	global g, *ret;
	curr_attr = arg->con.id;
	g.topic = curr_topic;
	g.feature = curr_func;
	g.attr = curr_attr;
	g.type = TP_ID;
	g.data = NULL;
	ret = oht_lookup(ht, &g, 1);
	return ret;
}

static global *prepare_func(ohash_tab *ht, Func_ *f)
{
	global g, *next, *arg, *ret;
	curr_func = f->name;
	g.topic = curr_topic;
	g.feature = curr_func;
	g.attr = NULL;
	g.type = TP_FUNC;
	//g.data = (void *)f->stat_l;
	ret = oht_lookup(ht, &g, 1);
	if (f->args == NULL)
		return ret;
	node *a = f->args;
	next = NULL;
	do {
		arg = prepare_args(ht, (Const_ *)a);
		arg->next = next;
		next = arg;
		a = (node *)a->list.next;
	} while (a != f->args);
	ret->data = arg;	//this is important, coz we finally get some
				//meaningful information
	return ret;
}
static global *prepare_proc(ohash_tab *ht, Proc_ *p)
{
	global g, *ret;
	curr_proc = p->name;
	g.topic = curr_topic;
	g.feature = curr_proc;
	g.attr = NULL;
	g.type = TP_PROC;
	g.data = NULL;
	return oht_lookup(ht, &g, 1);
}
/* type check topic is done by type checking all the sub features */
static int typing_topic(scope *s, type_env *penv, node *env)
{
	Topic_ *t = (Topic_ *)t;
	self_topic = t->name;
	scope_item i;
	i->name = t->name;
	i->type = TP_TOPIC;
	i->data = t->data;
	scope_add_item(s, penv, &i);

	int ret;
	type_env *self_env;
	scope_enter(s, env, &self_env);

	node *l = t->features;
	if (!l) {	//if no features in the topic, then I'm done
		scope_exit(s);
		return 0;
	}
	do {
		ret = ((node_struct *)l)->typing(s, self_env, l);
		l = l->next;
	} while (l != t->features);
	scope_exit(s);
}
/* typing kpt means two things:
 * *typing all the attrs
 * *adding kpt name in the toplogic link, to see if it connect a circle */
static int typing_kpt(scope *s, type_env *penv, node *env)
{
	Kpt_ *k = (Kpt_ *)env;
	scope_item i;
	i.name = k->name;
	i.type = TP_KPT;
	i.data = (void *) k->attr_l;
	scope_add_symbol(s, penv, &i);
	// checking nick name
	if (k->aka_name) {
		i.name = k->aka_name;
		i.data = (void *) k;
		scope_add_symbol(s, penv, &i);
	}

	int ret;
	type_env *self_env;
	scope_enter(s, &self_env);
	//TODO:do toplogic sort on subsets, implement it later
	
	node attrs = k->attr_l;
	if (!attrs) {	//we are done
		scope_exit(s);
		return 0;
	}
	do {
		((node_struct  *)attrs)->typing(s, self_env, attrs);
		attrs = attrs->next;
	} while (attrs != k->attr_l);
	scope_exit(s);
}
/* typing fucntion is done by checking all the statements
 * since we dont know type information about args
 * function check can be a bitch
 */
static int typing_func(scope *s, type_env *penv, node *env)
{
}

/* scope is totally static, there is no need to do type checkin */
static int typing_proc(scope *s, type_env *penv, node *env)
{
	return TP_PROC;
}
/* attr need typechecking just becasue we need to set the final type of attr
 * typing_attr do two things: a)do ASSIGN_A ASSIGN_E check b)check if expr is
 * valid.
 * NOTE: since the attr are extremely similar to label_stat, try to invoke the
 * function here when we get typing_label_state
 */
/*TODO: test it */
static int typing_attr(scope *s, type_env *penv, node *env)
{
	
	int ret = 0, del = 0;
	Attr_Stat_ *as = (Kpt_ *)env;
	scope_item i;
	i.name = as->attr_name;
	i.type = TP_ATTR;
	i.data = (void *)as->expr;
	scope_item *new_i = scope_op_symbol(penv, as->attr_name, SCOPE_NOR);

	if (as->append && new_i == NULL) { 		//append at non
		fprintf(stderr, 
			"fail to append data after non-created attr %s\n",
			as->symbol->str);
		ret = ERR_APPEND;
		new_i = scope_add_symbol(s, penv, &i);
		del = 1;

	} else if ((!as->append) && new_i != NULL) {	//duplicately define
		fprintf(stderr,
			"fail to duplicately define attr %s\n",
			as->symbol->str);
		ret = ERR_DUP;
	}
	if (as->expr == NULL)
		goto end;

	type_env *self_env;
	scope_enter(s, &self_env);
	i.type = ((node_struct *)as->expr)->typing(s, self_env, as->expr);
	scope_exit(s);

end:
	if (del) {
		scope_op_symbol(penv, as->attr_name, SCOPE_RM);
		new_i = NULL;	//restore state
	} 
	if (as->append && new_i != NULL) 
		new_i->type = lub(i.type, new_i->type);
	return ret;
}
static int typing_string(scope *s, type_env *penv, node *env)
{
	return TP_STRING;
}
static int typing_navig(scope *s, type_env *penv, node *env)
{
	Navig_ *n = (Navig_ *)env;
	symbol *topic 	= n->topic;
	symbol *kpt 	= n->kpt;
	symbol *attr 	= n->attr;
	global *g = oht_lookup(&global_tab, , 0);
	int ret = 0;
	if (infunction) 
		typing_fnavig(s, penv, env);
	else if (g == NULL)  {
		fprintf(stderr, "try to refer indetifier which is not declared");

	}
	else {
		if (attr->str)
			ret = TP_ATTR;
		else if (attr->str)
			ret = TP_KPT;
	}
}

/* Whatever, here we implement the lub (lesat upper bound) function */
/* you know what, I'm gonna play a trick, let's see if we can implement it as 
 *  least common multiple method :) */
/* becasue types are so well designed that that I can just find their least
 * common multiple by multiply them  */
static int lub(int type1, int type2) 
{
	return  type1 * type2;
};
