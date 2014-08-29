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

void yyerror(char *s);
extern int yylex();

#define YY_KEYWORDS
#include "ast_tree.h"
#include "snl_const.h"
extern Program_ *ast_root;

%}

%union {
	Program_ program__;
	Kpt_ *kpt__;
	List_ *list__;
	node *node_;
	Symbol  symbol;
	str_symbol strsymbol;
	char *err_msg;
}

%token TOPIC 201 KPT 202 AKA 203 EXTERN 204 LIST 205 PROC 206
%token WHAT 251 WHEN 252 HOW 253 WHY 254 WHERE 255 WHO 256
%token CASE 301 OF 302 ESAC 303 ENUM 304 MUNE 305 LET 306 IN 307 TEL 308
%token CONNECT 351 WITH 352 TO 353 FROM 354 ABOUT 355
%token <symbol> STR_CONST 150
%token <symbol> OBJECTID 151
%token ERROR 100

 /* Declare the types for grammar's non-terminals. */
%type <program__> program
/* this is main type in parsing */
%type <node_> topics topic feature_list feature
%type <node_> basic_expr basic_exprs
%type <node_> const const_list block_const
%type <node_> navig_list navig
%type <node_> assign
%type <node_> formal formal_list

%type <kpt__> kpt kpt_simple
 /* %type <proc__> proc */
%type <list__> list
%type <strsymbol> string attr
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
	|	list			{$$ = list_feature($1);		}
	;

	kpt
	:	kpt_simple ';'
		{$$ = $1;}
	|	kpt_simple ':' string ';'
		{$$ = kpt_const($1, single_node( 
				     basic_expr(what,
				      single_node(
					string_node($3)))));		}
	|	kpt_simple ':' '{' basic_exprs '}' ';'
		{$$ = kpt_const($1, $4);				}
	;

	kpt_simple
	:	KPT OBJECTID
		{$$ = kpt_simple(single_string($2, &id_arr), 
				NULL, NULL, NULL);			}
	|	KPT OBJECTID AKA OBJECTID
		{$$ = kpt_simple(single_string($2, &id_arr), 
				 single_string($4, &id_arr), 
					NULL, NULL);			}
	|	KPT OBJECTID EXTERN OBJECTID
		{$$ = kpt_simple(single_string($2, &id_arr), 
				 NULL, 
				 single_string($4, &id_arr), 
				 NULL);					}
	|	KPT OBJECTID AKA OBJECTID EXTERN OBJECTID
		{$$ = kpt_simple(single_string($2, &id_arr), 
				 single_string($4, &id_arr), 
				 single_string($6, &id_arr),
				 NULL);					}
	;

	list :	LIST '{' navig_list '}' ';'
		{$$ = list_simple($3);	}
	;

	
	/* skip list and proc now */
	basic_exprs
	:	basic_expr ';'			{$$ = single_node($1);	}
	|	basic_exprs basic_expr ';'	{append_node($1, 
							single_node($2));	}
	/*return type: node */
	basic_expr
	:	attr 				{$$ = basic_expr($1, NULL);	}
	/******* TODO: attr ":=" const ********/
	|	attr ':' const			{$$ = basic_expr($1, $3);	}
	;
	
	/* return type: str_symbol */
	attr
	:	WHAT				{$$ = what;}
	|	WHEN				{$$ = when;}
	|	HOW				{$$ = how;}
	|	WHY				{$$ = why;}
	|	WHERE				{$$ = where;}
	|	WHO				{$$ = who;}
	|	string				{$$ = $1;}
	;
	
	/*return type: node */
	formal 
	: 	OBJECTID 			{$$ = navig(NULL, $1, NULL);	}
	|	OBJECTID '@' OBJECTID		{$$ = navig($1, $3, NULL);	}
	|	OBJECTID '.' attr		{$$ = navig(NULL, $1, $3);	}
	|	attr				{$$ = navig(NULL, NULL, $1);	}
	;

	formal_list
	:	formal				{$$ = single_node($1);		}
	|	formal_list ',' formal		{$$ = append_node($1,
							single_node($3));	}

	navig_list
	: 	navig ';'			{$$ = single_node($1);		}
	|	navig_list navig ';'		{$$ = append_node($1,
							single_node($2));	}
	;

	navig
	
	/* ENUM is totally unnecessary
	 * :	ENUM single_navig_list MUNE	{$$ = enum_navig($2);		}
	 */
	/******* TODO: 	formal ":=" assign
			formal "+=" assign */
	:	formal ':' assign		{$$ = formal_navig($1, $3); 	}

	|	LET formal_list IN assign TEL	{$$ = let_navig($2, $4);	}

	/* CASE is not useful now, move it to future desgin 
	|	CASE attr OF formal_list ':' ESAC	
	{$$ = case_navig($2, $4);	}
	*/
	;


	/* FIX ME: there is a design problem, we cannot let user to add multiple edges
 	 * to  a node, fix it in the future version
	 */
	/* return type: node */
	assign
	: 	const				{$$ = single_node($1);	}
	|	block_const			{$$ = $1;}

	/* TODO: move CONNECT to const, the OBJECTID here makes connection not
 	 * usable...*/
	|	CONNECT WITH formal ABOUT OBJECTID ':' const
	{	$$ = single_node(
			connection($3, WITH_CONN, $5, single_node($7))
			);							}	
	|	CONNECT TO formal ABOUT OBJECTID ':' const
	{	$$ = single_node(
			connection($3, TO_CONN, $5, single_node($7))
			);							}

	|	CONNECT FROM formal ABOUT OBJECTID ':' const
	{	$$ = single_node(
			connection($3, FROM_CONN, $5, single_node($7))
			);							}
	;
	

	block_const : '{' const_list '}'	{$$ = $2;			}
	;

	const_list
	:	const ';'			{$$ = single_node($1);	}
	|	const_list const ';'		{$$ = append_node($1,
							single_node($2));	}
	;

	const 
	: 	string				{$$ = string_node($1);		}
	|	OBJECTID			{$$ = id_node($1); 		}
	;

	string
	: 	STR_CONST			
		{$$ = single_string($1,&str_arr); 				}
	|	string STR_CONST		
		{$$ = append_string($1, $2, &str_arr);				}
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
