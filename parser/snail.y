%{
#include <stdio.h>
/* I don't know what's the point, since we use the curr_lineno provided by
 * yylex(), and set every node location by hand, we just omit it
 */
#define YYLTYPE int
#define yylloc curr_lineno
#define YYLLOC_DEFAULT(Current, Rhs, N)

extern int curr_lineno;
extern char *curr_filename;
extern int in_function;

void yyerror(char *s);
extern int yylex();

#define YY_KEYWORDS
#include "ast_tree.h"
#include "snl_const.h"
extern Program_ *ast_root;
extern symbol_tab id_table;
extern symbol_tab str_table;
extern symbol_tab str_psr_tab;
extern symbol *what;
extern symbol *when;
extern symbol *where;
extern symbol *why;
extern symbol *who;
extern symbol *how;

/* yyerror messages */
char *topic_infunc = "topic is not allowed to access in function.";
/* yyerror messages */
%}
%name-prefix "snail_yy"
%union {
	Program_ program__;
	Kpt_ *kpt__;
	node *node_;
	symbol* sym;
	Func_ *func__;
	char *err_msg;
}

%token TOPIC 201 KPT 202 AKA 203 EXTERN 204 FUNC 205 PROC 206
%token WHAT 251 WHEN 252 HOW 253 WHY 254 WHERE 255 WHO 256
%token CASE 301 OF 302 ESAC 303 ENUM 304 MUNE 305 LET 306 IN 307 TEL 308
%token RETURN 309
%token CONNECT 351 WITH 352 TO 353 FROM 354 ABOUT 355
%token <sym> STR_CONST 150
%token <sym> OBJECTID 151
%token ASSIGN_E 152 ASSIGN_A 153
%token ERROR 100

 /* Declare the types for grammar's non-terminals. */
%type <program__> program
/* this is main type in parsing */
%type <node_> topics topic feature_list feature
%type <node_> attrs attr
%type <node_> const const_list block_const
%type <node_> statements statement
%type <node_> labeled_stats labeled_stat
%type <node_> expr expr_list
%type <node_> navig
%type <node_> id_list

%type <kpt__> kpt kpt_simple
%type <func__> func
 /* %type <proc__> proc */
%type <sym> string attr_name

/* precedence declaration */
%expect 0
%precedence ABOUT
%left '-'
%left '|' '&'

%%

 /* program */
	program  : topics 		{ast_root = program($1);}
	;
	
 /* topics */
	topics
	:	topic 			{$$ = single_node($1);	}
	|	topics topic		{$$ = append_node($1,
					 	 single_node($2));	}
	;

	topic
	:	TOPIC OBJECTID '{' feature_list '}' ';'
	{ $$ = topic_simple($2, $4);	}
	;

	feature_list
	:	feature			{$$ = single_node($1);	}
	|	feature_list feature	{$$ = append_node($1,
						single_node($2));	}
	;

	feature
	:	kpt			{$$ = kpt_feature($1);		} 
	/*
	|	proc			{$$ = proc_feature($1);		} 
	*/
	|	func			{$$ = func_feature($1);		}
	;
	kpt
	:	kpt_simple ';'
		{$$ = $1;}
	|	kpt_simple ':' string ';'
		{$$ = kpt_const($1, single_node( 
				     attr_stat(what,
				      single_node(
					string_node($3)))));		}
	|	kpt_simple ':' '{' attrs '}' ';'
		{$$ = kpt_const($1, $4);				}
	;
	/* TODO: allow extern to have more complex expression */
	kpt_simple
	:	KPT OBJECTID
		{$$ = kpt_simple($2, NULL, NULL, NULL);			}
	|	KPT OBJECTID AKA OBJECTID
		{$$ = kpt_simple($2, $4,   NULL, NULL);			}
	|	KPT OBJECTID EXTERN OBJECTID
		{$$ = kpt_simple($2, NULL, $4, NULL);			}
	|	KPT OBJECTID AKA OBJECTID EXTERN OBJECTID
		{$$ = kpt_simple($2, $4, $6, NULL);			}
	;

	id_list :
		OBJECTID			{$$ = single_node(id_node($1));}
	|	id_list ',' OBJECTID		{$$ = append_node($1,
							single_node(id_node($3)));}
	;

	func :  FUNC OBJECTID '(' id_list ')' '{' statements '}'
		{ in_function = 1;	/* when in function, the navig behave
					 * differently */
		$$ = function($2, $4, $7);
		  in_function = 0; }
	;
	attrs
	:	attr ';'			{$$ = single_node($1);	}
	|	attrs attr ';'			{append_node($1, 
							single_node($2));	}
	;
	/*return type: node */
	attr
	:	attr_name 			{$$ = attr_stat($1, NULL);	}
	/******* TODO: attr ":=" const ********/
	/* should we distinguish ASSIGN_E and ASSIGN_A */
	|	attr_name ASSIGN_E expr		{$$ = attr_stat($1, $3);	}

	|	attr_name ASSIGN_A expr		{$$ = attr_stat($1, $3);	}
	;

	/* return type: symbol */
	attr_name
	:	WHAT				{$$ = what;}
	|	WHEN				{$$ = when;}
	|	HOW				{$$ = how;}
	|	WHY				{$$ = why;}
	|	WHERE				{$$ = where;}
	|	WHO				{$$ = who;}
	|	OBJECTID			{$$ = $1;}
	;

	statements
	: 	statement ';'			{$$ = single_node($1);		}
	|	statements statement ';'		{$$ = append_node($1,
							single_node($2));	}
	;

	statement 
	: 	labeled_stat			{$$ = $1;}

	|	LET id_list IN expr TEL		{$$ = let_stat($2, $4);	}

	|	CASE attr_name OF labeled_stats ESAC	
						{$$ = case_stat($2, $4);}
	|	RETURN expr			{$$ = $2;}
	;

	labeled_stats 
	:	labeled_stat ';'		{$$ = single_node($1);		}
	|	labeled_stats labeled_stat ';'	{$$ = append_node($1,
							single_node($2));	}
	;

	labeled_stat 
	:	navig ASSIGN_E expr		{$$ = label_stat($1, $3);	}
	|	navig ASSIGN_A expr		{$$ = label_stat($1, $3);	}
	;


	expr_list :
		expr				{$$ = single_node($1);	}
	|	expr_list ',' expr		{$$ = append_node($1,
							single_node($3));	}
	;

	/* return type: node */
	expr
	: 	const				{$$ = single_node($1);	}
	|	block_const			{$$ = $1;}

	/* do type checking on this */
	/**************************************/
	|	CONNECT WITH navig ABOUT expr
	{	$$ = single_node(
			connection($3, WITH_CONN, $5));				}	

	|	CONNECT TO navig ABOUT expr
	{	$$ = single_node(
			connection($3, TO_CONN, $5));				}

	|	CONNECT FROM navig ABOUT expr
	{	$$ = single_node(
			connection($3, FROM_CONN, $5));				}

	/* this means function are global */
	|	OBJECTID '(' expr_list ')'	{$$ = dispatch ($1, $3);	}
	|	expr '|' expr			{$$ = operation($1, $3, '|');	}
	|	expr '&' expr			{$$ = operation($1, $3, '&');	}
	|	expr '-' expr			{$$ = operation($1, $3, '-');	}
	;
	
	/* checked */
	block_const : '{' const_list '}'	{$$ = $2;			}
	;

	const_list
	:	const ';'			{$$ = single_node($1);		}
	|	const_list const ';'		{$$ = append_node($1,
							single_node($2));	}
	;
	/* checked */
	const
	: 	string				{$$ = single_node(string_node($1));}
	|	navig				{$$ = single_node($1);}
	;

	/*return type: node */
	navig 
	: 	OBJECTID /*kpt */		{$$ = navig(NULL, $1, NULL);	}
	|	OBJECTID '.' attr_name		{$$ = navig(NULL, $1, $3);	}
	|	OBJECTID '@' OBJECTID		{if (in_function == 1) {
							yyerror(topic_infunc);
							$$ = navig(NULL, NULL, NULL);
						 } else
						 	$$ = navig($1, $3, NULL);}
	|	OBJECTID '@' OBJECTID '.' attr_name		
		{if (in_function == 1) {
			yyerror(topic_infunc);
			$$ = navig(NULL, NULL, NULL);
		 } else
		 	$$ = navig($1, $3, $5);				}
	;

	string
	: 	STR_CONST			
		{$$ = single_string(&str_psr_tab, $1); 				}
	|	string STR_CONST
		{$$ = append_string(&str_psr_tab, $1, $2);			}
	;
%%

void yyerror(char *s)
{
	fprintf(stderr, "\"%s\", %d : %s at or near %c", curr_filename, 
							curr_lineno,
							s,
							yychar);

/* yychar contains the lookahead token or YYEOF, YYEMPTY*/
}
