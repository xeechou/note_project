%{
#include <stdio.h>
/* I don't know what's the point, since we use the curr_lineno provided by
 * yylex(), and set every node location by hand, we just omit it
 */
#define YYLTYPE int
#define yylloc curr_lineno
#define YYLLOC_DEFAULT(Current, Rhs, N)



#define YY_KEYWORDS
#include "snail.h"
#include "snl_const.h"
void gen_ast(SNAILState *s);
void prepare_lexer(SNAILState *s);
void yyerror(char *s);

/****************** extern datas *********************/
extern int   yylex();
extern int   curr_lineno;
static char *curr_filename;
//extern int   in_function;

extern symbol *what;
extern symbol *when;
extern symbol *where;
extern symbol *why;
extern symbol *who;
extern symbol *how;
extern symbol *self;	/* current kpt, function... */
extern symbol *local;	/* current topic, however we dont restrict user not to
			 * use local, local is statical allocated. */



/****************** static datas *********************/
static ast_node **ast_root;
static node_data *tmp_data;

static symbol_tab *id_table;
static symbol_tab *str_table;
static symbol_tab *str_psr_tab;

static int tmp_num;
static page_strt *nodes_data;
static page_strt *ast_nodes;

/* functions */
static inline ast_node *nil_node(void);
static inline ast_node *single_node(ast_node *n);
static inline ast_node *append_node(ast_node *to, ast_node *from);
static inline node_data *nil_data(void);
static inline node_data *single_data(node_data *n);
static inline node_data *append_data(node_data *to, node_data *from);

static inline ast_node *ast_syntax(char *start, char *end, int type, 
			node_data *n, unsigned int n_nodes)
{
	return syntax_line(start, end, type,
			   n, n_nodes, ast_nodes);
}
static inline node_data *anode(ast_node *n) 	{ return data_node(n, nodes_data); }
static inline node_data *inode(int i)	 	{ return data_int (i, nodes_data); }
static inline node_data *snode(symbol *s)	{ return data_sym (s, nodes_data); }

static inline ast_node 	*gen_topic(symbol *name, ast_node *features);
static inline ast_node 	*kpt_one_line(node_data *kpt_prefix, symbol *str);
static inline ast_node 	*gen_kpt(node_data *kpt_prefix, ast_node *attrs);
static inline node_data	*gen_kpt_simple(symbol *name, symbol *aka_name,
					ast_node *subsets);
static inline node_data *gen_args(node_data *args);
static inline ast_node 	*gen_func(symbol *name, node_data *args, 
				ast_node *stats);
static inline ast_node 	*gen_attr(symbol *name, ast_node *expr, int mode);
static inline node_data *gen_ids(node_data *ids);
static inline ast_node 	*gen_let(node_data *ids, ast_node *expr);
static inline ast_node 	*gen_case(symbol *name, ast_node *stats);
static inline ast_node 	*gen_case_stat(ast_node *id, ast_node *expr);
static inline ast_node 	*gen_label_stat(ast_node *navig, 
				ast_node *expr, int mode);
static inline ast_node 	*gen_conn(ast_node *navig,
				ast_node *expr, int mode);
static inline ast_node 	*gen_dispatch(symbol *topic, symbol *name, 
				ast_node *elist);
static inline ast_node 	*gen_oper(ast_node *ep1, ast_node *ep2, int mode);
static inline ast_node 	*gen_str(symbol *str);
static inline ast_node 	*gen_navig(symbol *topic, symbol *kpt, symbol *attr);
static inline ast_node 	*gen_proc(symbol *name, ast_node *procc);
static inline ast_node 	*gen_step(int mode, ast_node *step1,
				ast_node *step2, ast_node *step3);

/* yyerror messages */
char *topic_infunc = "topic is not allowed to access in function.";
/* yyerror messages */
%}
%name-prefix "snail_yy"

%union {
	ast_node *node_;
	node_data *data_;
	symbol* sym;
	char *err_msg;
}

%token TOPIC 201 KPT 202 AKA 203 EXTERN 204 FUNC 205 PROC 206
%token WHAT 251 WHEN 252 HOW 253 WHY 254 WHERE 255 WHO 256
%token CASE 301 OF 302 ESAC 303 ENUM 304 MUNE 305 LET 306 IN 307 TEL 308
%token IF 310 THEN 311 ELSE 312 FI 313 WHILE 314
%token RETURN 309
%token CONNECT 351 WITH 352 TO 353 FROM 354 ABOUT 355
%token <sym> STR_CONST 150
%token <sym> OBJECTID 151
%token ASSIGN_E 152 ASSIGN_A 153
%token ERROR 100

 /* Declare the types for grammar's non-terminals. */
%type <node_> program
/* this is main type in parsing */
%type <node_> topics topic feature_list feature
%type <node_> attrs attr
%type <node_> const const_list block_const
%type <node_> statements statement
%type <node_> labeled_stat
%type <node_> expr expr_list
%type <node_> case_stat case_stats
%type <node_> navig
%type <data_> id_list
%type <node_> proc_components proc_component

%type <node_> kpt
%type <data_> kpt_simple
%type <node_> func
%type <node_> proc
 /* %type <proc__> proc */
%type <sym> string attr_name

/* precedence declaration */
%expect 0
%precedence ABOUT
%left '-'
%left '|' '&'

%%

 /* program */
 	/*#define OD_PROG_TOP	0
 	 *#define N_PROG_ELEM	1 */
	program  : topics 		{*ast_root = ast_syntax(
						start_prog, end_prog, 
						0, anode($1), N_PROG_ELEM);}
	;
	
 /* topics */
	topics
	:	topic 			{$$ = single_node($1);	}
	|	topics topic		{$$ = append_node($1, $2);	}
	;

	/*#define N_TOPIC_ELEM	2
	 *#define OD_TOPIC_NAME	0
	 *#define OD_TOPIC_FEAT	1 */
	topic
	:	TOPIC OBJECTID '{' feature_list '}' ';'

	{	$$ = gen_topic($2, $4); }
	;

	feature_list
	:	feature			{$$ = single_node($1);		}
	|	feature_list feature	{$$ = append_node($1, $2);	}
	;

	feature
	:	kpt			{$$ = $1;		} 

	|	proc			{$$ = $1;		} 

	|	func			{$$ = $1;		} 
	;

	
	/* #define N_KPT_ELEM	4 
	 * #define OD_KPT_NAME	0
	 * #define OD_KPT_AKA   1
	 * #define OD_KPT_SUBS	2
	 * #define OD_KPT_ATTRS 3 */
	kpt
	:	kpt_simple ASSIGN_E string ';'
		{	$$ = kpt_one_line($1, $3);	}

	|	kpt_simple ':' '{' attrs '}' ';'
		{	$$ = gen_kpt($1, $4);		}
	;

	/* #define N_KPT_SIMPLE_E 3 */
	kpt_simple
	:	KPT OBJECTID
		{$$ = gen_kpt_simple($2, NULL, NULL);	}
	|	KPT OBJECTID AKA OBJECTID
		{$$ = gen_kpt_simple($2, $4, NULL);	}
	|	KPT OBJECTID EXTERN expr
		{$$ = gen_kpt_simple($2, NULL, $4);	}
	|	KPT OBJECTID AKA OBJECTID EXTERN expr
		{$$ = gen_kpt_simple($2, $4, $6);	}
	;

	id_list :				{$$ = NULL;		}
	|	OBJECTID			{$$ = single_data(snode($1));
						tmp_num = 1;}
	|	id_list ',' OBJECTID		{$$ = append_data($1, snode($3));
						tmp_num++;	}
	;
	/* #define N_FUNC_ELEM	3 
	 * #define OD_FUNC_NAME 0
	 * #define OD_FUNC_ARG	1
	 * #define OD_FUNC_STAT 2 */
	func :  FUNC OBJECTID '(' id_list ')' '{' statements '}'
	{	$$ = gen_func($2, $4, $7);	}
	;
	attrs
	:	attr ';'			{$$ = single_node($1);		}
	|	attrs attr ';'			{$$ = append_node($1, $2);	}
	;

	/* #define N_ATTR_ELEM	3
	 * #define OD_ATTR_NAME	0
	 * #define OD_ATTE_EXPR	1
	 * #define OB_ATTR_MODE 2 */
	attr
	:	attr_name 			{$$ = gen_attr($1, NULL, 0);	}

	|	attr_name ASSIGN_E expr		{$$ = gen_attr($1, $3, '=');	}

	|	attr_name ASSIGN_A expr		{$$ = gen_attr($1, $3, '+');	}
	;

	/* return type: symbol */
	attr_name
	:	WHAT				{$$ = what;	}
	|	WHEN				{$$ = when;	}
	|	HOW				{$$ = how;	}
	|	WHY				{$$ = why;	}
	|	WHERE				{$$ = where;	}
	|	WHO				{$$ = who;	}
	|	OBJECTID			{$$ = $1;	}
	;

	/* look, it would be pointless to put a nil statment here */
	statements
	: 	statement ';'			{$$ = single_node($1);		}
	|	statements statement ';'	{$$ = append_node($1, $2);	}
	;

	statement 
	: 	labeled_stat			{$$ = $1;	}

	/* #define N_LET_ELEM	2
	 * #define OD_LET_ID	0
	 * #define OD_LET_EXPR	1 */
	|	LET id_list IN expr TEL		{$$ = gen_let($2, $4);	}
	/* #define N_CASE_ELEM	2
	 * #define OD_CASE_NAME	0
	 * #define OD_CASE_STAT	1 */
	|	CASE attr_name OF case_stats ESAC
						{$$ = gen_case($2, $4);}
	|	RETURN expr			{$$ = $2;	}
	;

	case_stats 
	:	case_stat ';'			{$$ = single_node($1);		}
	|	case_stats case_stat ';'	{$$ = append_node($1, $2);	}
	;
	
	case_stat :	navig ':' expr		{$$ = gen_case_stat($1, $3);	}
	;
	/* #define N_LAB_ELEM	3
	 * #define OD_LAB_NV	0	
	 * #define OD_LAB_EXPR	1
	 * #define OD_LAB_MODE	2 */
	labeled_stat 
	:	navig ASSIGN_E expr		{$$ = gen_label_stat($1, $3, '=');}
	|	navig ASSIGN_A expr		{$$ = gen_label_stat($1, $3, '+');}
	;

	/* ast_node */
	expr_list :				{$$ = nil_node();		}
	|	expr				{$$ = single_node($1);		}
	|	expr_list ',' expr		{$$ = append_node($1, $3);	}
	;

	/* return type: node */
	expr
	: 	const				{$$ = single_node($1);	}
	|	block_const			{$$ = $1;}

	/* #define N_CON_ELEM	3
	 * #define OD_CON_NV	0
	 * #define OD_CON_EXPR	1
	 * #define OD_CON_MODE  2 */
	|	CONNECT WITH navig ABOUT expr
	{	$$ = single_node(
			gen_conn($3, $5, WITH_CONN));				}	

	|	CONNECT TO navig ABOUT expr
	{	$$ = single_node(
			gen_conn($3, $5, TO_CONN));				}

	|	CONNECT FROM navig ABOUT expr
	{	$$ = single_node(
			gen_conn($3, $5, FROM_CONN));				}

	/* #define N_DIS_ELEM	3
	 * #define OD_DIS_TOP	0
	 * #define OD_DIS_NAME	1
	 * #define OD_DIS_EL	2 */
	/* I guess make function global maybe better */
	|	OBJECTID '(' expr_list ')'	
		{$$ = gen_dispatch (local, $1, $3);	}
	|	OBJECTID '@' OBJECTID '(' expr_list ')'	
		{$$ = gen_dispatch ($1, $3, $5);	}

	/* #define N_OPER_ELEM	3
	 * #define OD_OPER_TOP	0
	 * #define OD_OPER_NAME	1
	 * #define OD_OPER_EL	2 */
	|	expr '|' expr			{$$ = gen_oper($1, $3, '|');	}
	|	expr '&' expr			{$$ = gen_oper($1, $3, '&');	}
	|	expr '-' expr			{$$ = gen_oper($1, $3, '-');	}
	;
	
	block_const : '{' const_list '}'	{$$ = $2;			}
	;

	const_list
	:	const ';'			{$$ = single_node($1);		}
	|	const_list const ';'		{$$ = append_node($1, $2);	}
	;

	const
	: 	string				{$$ = gen_str($1);	}
	|	navig				{$$ = $1;		}
	;

	/* #define N_NAVIG_ELEM		3
	 * #define OD_NAVIG_TOP		0
	 * #define OD_NAVIG_NAME	1
	 * #define OD_NAVIG_EL		2 */
	navig 
	: 	OBJECTID /*kpt */		{ $$ = gen_navig(local, $1, NULL);}
	|	OBJECTID '.' attr_name		{ $$ = gen_navig(local, $1, $3);}
	|	OBJECTID '@' OBJECTID		{ $$ = gen_navig($1, $3, NULL);	}
	|	OBJECTID '@' OBJECTID '.' attr_name		
		{ $$ = gen_navig($1, $3, $5);	}
	;

	proc : PROC OBJECTID  '{' proc_components '}'
		{	$$ = gen_proc($2, $4);		}
	;
	proc_components 
	:	proc_component ';'			{single_node($1); 	}
						
	|	proc_components proc_component ';'	{append_node($1, $2);	}
	;
	/* with the fi close parent, I don't need to worry about
	 * shift/reduce problem */
	/* #define N_STEP_ELEM		3
	 * #define OD_STEP_TOP		0
	 * #define OD_STEP_NAME		1
	 * #define OD_STEP_EL		2 */
	proc_component 
	:	string		
		{$$ = gen_step(PC_STR, gen_str($1), NULL, NULL); 	}
	|	IF proc_component THEN proc_component FI
		{$$ = gen_step(PC_IF, $2, $4, NULL);			}
	|	IF proc_component THEN proc_component ELSE proc_component FI
		{$$ = gen_step(PC_IF_ELSE, $2, $4, $6);			}
	|	WHILE	proc_component THEN proc_component 
		{$$ = gen_step(PC_LOOP, $2, $4, NULL);			}
	;

	string
	: 	STR_CONST			
		{$$ = single_string(str_psr_tab, $1); 				}
	|	string STR_CONST
		{$$ = append_string(str_psr_tab, $1, $2);			}
	;
%%

static inline ast_node *nil_node(void)
{
	return NULL;
}
static inline ast_node *single_node(ast_node *n)
{
	n->prev = n->next = n;
	return n;
}
static inline ast_node *append_node(ast_node *to, ast_node *from)
{
	ast_node *prev = to->prev;
	from->prev = prev;
	from->next = to;
	prev->next = from;
	to->prev = from;

	return to;
}
static inline node_data *nil_data(void)
{	return NULL;	}
static inline node_data *single_data(node_data *d)
{
	d->prev = d->next = d;
	return d;
}
static inline node_data *append_data(node_data *to, node_data *from)
{
	node_data *prev = to->prev;
	from->prev = prev;
	from->next = to;
	prev->next = from;
	to->prev = from;

	return to;
}
/*********************** wrap functions ***********************/
/* to keep the simplicity of bison code itself, the wrap function is avoidable
 * At least we kept a clean ast_node structure :) */
static inline ast_node *gen_topic(symbol *name, ast_node *features)
{ 
	tmp_data = compose_node_data(N_TOPIC_ELEM, 
			snode(name), anode(features));

	return ast_syntax(start_topic, end_topic, TP_TOPIC,
	   		 tmp_data, N_TOPIC_ELEM); 
}
static inline ast_node *kpt_one_line(node_data *kpt_prefix, symbol *str)
{
	//build attr first
	ast_node *a = gen_str(str);
	ast_node *b = gen_attr(what, a, '=');

	tmp_data = append_node_data(1, kpt_prefix, anode(b));

	return  ast_syntax(start_kpt, end_kpt, TP_KPT,
			tmp_data, N_KPT_ELEM);
}

static inline ast_node *gen_kpt(node_data *kpt_prefix, ast_node *attrs)
{
	node_data *d = append_node_data(1, kpt_prefix, anode(attrs));	
	ast_node *a = ast_syntax(start_kpt, end_kpt, TP_KPT,
                        	d, N_KPT_ELEM);
	return a;
}
static inline node_data	*gen_kpt_simple(symbol *name, symbol *aka_name,
					ast_node *subsets) 
{
	return compose_node_data(3, snode(name), snode(aka_name),
				anode(subsets));
}

/* adding args is exceptional, coz it is unkown in the first place */
static inline node_data *gen_args(node_data *args) 
{
	ast_node *a = ast_syntax(start_arg, end_arg, TP_ID,
				args, tmp_num);
	tmp_num = 0;
	return anode(a);
}

static inline ast_node *gen_func(symbol *name, node_data *args, ast_node *stats)
{
	node_data *d = compose_node_data(N_FUNC_ELEM, snode(name), 
					gen_args(args), anode(stats));
	return ast_syntax(start_func, end_func, TP_FUNC, 
				d, N_FUNC_ELEM);
}

static inline ast_node *gen_attr(symbol *name, ast_node *expr, int mode)
{
	node_data *d = compose_node_data(N_ATTR_ELEM, snode(name),
			anode(expr), inode(mode));
	return ast_syntax(start_attr, end_attr, TP_ATTR,
				d, N_ATTR_ELEM); 
}

static inline node_data *gen_ids(node_data *ids)
{
	ast_node *a = ast_syntax(start_id, end_id, TP_ID,
				ids, tmp_num);
	tmp_num = 0;
	return anode(a);
}

static inline ast_node *gen_let(node_data *ids, ast_node *expr)
{
	node_data *d = compose_node_data(N_LET_ELEM, gen_ids(ids),
					anode(expr));

	return ast_syntax(start_let, end_let, TP_NONE, d, N_LET_ELEM);
}

static inline ast_node *gen_case(symbol *name, ast_node *stats)
{
	node_data *d = compose_node_data(N_CASE_ELEM, snode(name),
				anode(stats));

	return ast_syntax(start_case, end_case, TP_NONE, d, N_CASE_ELEM);
}

static inline ast_node *gen_case_stat(ast_node *id, ast_node *expr)
{
	node_data *d = compose_node_data(2, anode(id), anode(expr));

	return ast_syntax(start_case_stat, end_case_stat, TP_NONE, 
			d, 2);
}
static inline ast_node *gen_label_stat(ast_node *navig, 
				ast_node *expr, int mode)
{
	node_data *d = compose_node_data(N_LAB_ELEM, anode(navig),
			anode(expr), inode(mode));
	return ast_syntax(start_label_stat, end_label_stat, TP_NONE,
				d, N_LAB_ELEM); 
}

static inline ast_node *gen_conn(ast_node *navig,
				ast_node *expr, int mode)
{
	node_data *d = compose_node_data(N_CON_ELEM, anode(navig),
			anode(expr), inode(mode));
	return ast_syntax(start_conn, end_conn, TP_CONN,
				d, N_CON_ELEM); 
}
static inline ast_node *gen_dispatch(symbol *topic, symbol *name, 
				ast_node *elist)
{
	node_data *d = compose_node_data(N_DIS_ELEM, snode(topic), 
			snode(name), anode(elist));
	return ast_syntax(start_dispatch, end_dispatch, TP_NONE,
				d, N_DIS_ELEM); 
}

static inline ast_node *gen_oper(ast_node *ep1, ast_node *ep2, int mode)
{
	node_data *d = compose_node_data(N_OPER_ELEM, anode(ep1),
			anode(ep2), inode(mode));
	return ast_syntax(start_oper, end_oper, TP_NONE,
				d, N_OPER_ELEM);
}

static inline ast_node *gen_str(symbol *str)
{
	return ast_syntax(start_str, end_str, TP_STRING, snode(str), 1);
}
/* checking should be done in typing steps */
static inline ast_node *gen_navig(symbol *topic, symbol *kpt, symbol *attr)
{
	node_data *d = compose_node_data(N_NAVIG_ELEM, snode(topic), 
					snode(kpt), snode(attr));

	return ast_syntax(start_navig, end_navig, TP_ID, 
				d, N_NAVIG_ELEM);
}

static inline ast_node *gen_proc(symbol *name, ast_node *procc)
{
	node_data *d = compose_node_data(N_PROC_ELEM, snode(name), procc);
	return ast_syntax(start_proc, end_proc, TP_PROC, d, 2);
}

static inline ast_node *gen_step(int mode, ast_node *step1,
				ast_node *step2, ast_node *step3)
{
	node_data *d = compose_node_data(N_STEP_ELEM, anode(step1), 
					anode(step2), anode(step3));

	return ast_syntax(start_procc, end_procc, TP_PROC, 
				d, N_STEP_ELEM);
}

void yyerror(char *s)
{
	fprintf(stderr, "\"%s\", %d : %s at or near %c", curr_filename, 
							curr_lineno,
							s,
							yychar);
// yychar contains the lookahead token or YYEOF, YYEMPTY
}

void gen_ast(SNAILState *s)
{
	prepare_lexer(s);
	curr_filename = s->curr_filename;
	curr_lineno = 0;

	id_table = &s->id_table;
	str_table = &s->str_table;
	str_psr_tab = &s->str_psr_tab;

	nodes_data = &s->nodes_data;
	ast_nodes  = &s->ast_nodes;
	ast_root = &s->ast_root;
	snail_yyparse();
}
