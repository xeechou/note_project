%{
#include <stdio.h>
#include "snl.h"
#include "stringtab.h"
extern int curr_lineno;
static int nested_cmtl = 0;

#define MAX_STR_LEN 1025
char str_buf[MAX_STR_LEN];
char *str_buf_ptr;
int error_flag;
extern FILE *fin;
/* some assumptions */
#define yylval snl_yylval
/* define some error flag */
#define OVERFLOW	-1
#define NULL_IN_STR 	-2
#define EOF_IN_STR	-3
%}

%option noyywrap
%x			comment
%x			line_comment
%x			str
%x			str_bkslash
%x			raw_str

 /* basic identifiers */
WHITESPACE		[\ \t\f\r\v]+
NEWLINE_REG		\n

OBJID_REG		[A-Za-z_][A-Za-z_0-9]*

 /* position lower tokens have higher priority */
 /* keywords */
TOPIC_REG		topic
KPT_REG			kpt
AKA_REG			AKA
WHAT_REG		what
WHEN_REG		when
HOW_REG			how
WHY_REG			why
OF_REG			of
COMPARES_REG		compares
REF_REG			ref
 /* end keywords */

ASSIGN_REG		":"
COMMA			","
DOT			"."
SEMI_COLON		";"
LEFT_CURBRA		"{"
RIGHT_CURBRA		"}"
%%
 /* 
  * Line_Comment
  */
"#"				BEGIN(line_comment);
<line_comment>[^\n]		/* no operation */
<line_comment>\n	{
				curr_lineno++;
			}

 /*
  * Nested comment
  */
"(*"				BEGIN(comment);
<comment>"*)"		{	/* end of the comment */
				if (nested_cmtl > 0) {
					nested_cmtl--;
				} else {
					BEGIN(INITIAL);
				}
			}
<comment>"(*"		{	nested_cmtl++;		}
<comment><<EOF>>	{	snl_yylval.err_msg = "EOF in comment!";
				BEGIN (INITIAL);
				return (ERROR);
			}
<comment>\n			curr_lineno++;
<comment>.		{}	/* do nothing */
 /* outside the comment */
"*)"			{	snl_yylval.err_msg = "Unmatched *)";
				BEGIN (INITIAL);
				return (ERROR);
			}
 /* end of comment */

 /* Operations */
{ASSIGN_REG}		{	return (':');		}
{COMMA}			{	return (',');		}
{DOT}			{	return ('.');		}
{SEMI_COLON}		{	return (';');		}
{LEFT_CURBRA}		{	return ('{');		}
{RIGHT_CURBRA}		{	return ('}');		}

 /* end of Operations */

 /*
  * KEYWORDS are case-sensitive, actually just lower-case in SNL
  * except AKA
  */
{TOPIC_REG}		{ 	return (TOPIC);		}
{KPT_REG}		{	return (KPT);		}	
{AKA_REG}		{	return (AKA);		}
{WHAT_REG}		{	return (WHAT);		}
{WHEN_REG}		{	return (WHEN);		}
{HOW_REG}		{	return (HOW);		}
{WHY_REG}		{	return (WHY);		}
{OF_REG}		{	return (OF);		}
{COMPARES_REG}		{	return (COMPARES);	}
{REF_REG}		{	return (REF);		}

 /* Keywords ends */

{OBJID_REG}		{	snl_yylval.symbol = add_string(idtable, yytext);
				return (OBJECTID);
			}

{WHITESPACE}		{}
{NEWLINE_REG}		{	curr_lineno++;		}

 /* 
  * String Constant
  * There are two kinds of string in SNL, regular and raw
  */
\"			{	str_buf_ptr = str_buf;
				BEGIN(str);
			}

<str>\"			{	BEGIN(INITIAL);
				if (error_flag)
					return (ERROR);
				if ((str_buf_ptr - str_buf) > 1024) {
					snl_yylval.err_msg = "String \
constant too long";
					str_buf[1024] = '\0';
					return (ERROR);
				}
				else {
					*str_buf_ptr = '\0';
					snl_yylval.symbol = 
					add_string(strtable, str_buf);
					return (STR_CONST);
				}

			}
<str><<EOF>>		{	BEGIN(INITIAL);
				snl_yylval.err_msg = "EOF in string constant";
				return (ERROR);
			}
<str>\0			{	snl_yylval.err_msg = "String costains null "
				"characters.";
				error_flag = NULL_IN_STR;
			}
<str>\n			{	BEGIN(INITIAL);
				if (error_flag) ;
				else {
					snl_yylval.err_msg = "Unterminated "
					"string constant.";
				}
				curr_lineno++;
				return (ERROR);
			}
<str>\\			{	BEGIN(str_bkslash);		}
<str_bkslash>\"	{	*str_buf_ptr++ = '\"';		
				BEGIN(str);
			}

<str_bkslash>\\		{	*str_buf_ptr++ = '\\';		
				BEGIN(str);
			}
<str_bkslash>\n		{	*str_buf_ptr++ = '\n';
				curr_lineno++;
				BEGIN(str);
			}
 /* there are four kinds of white space */
<str_bkslash>n		{	*str_buf_ptr++ = '\n';
				BEGIN(str);
			}
<str_bkslash>t		{
				*str_buf_ptr++ = '\t';
				BEGIN(str);
			}
<str_bkslash>b		{
				*str_buf_ptr++ = '\b';
				BEGIN(str);
			}
<str_bkslash>f		{
				*str_buf_ptr++ = '\f';
				BEGIN(str);
			}
<str_bkslash><<EOF>>	{
				snl_yylval.err_msg = "EOF in string "
				"constant";
				error_flag = EOF_IN_STR;
				BEGIN(INITIAL);
			}
<str_bkslash>\0		{
				snl_yylval.err_msg = "String constains "
				"escaped null character";
				error_flag = NULL_IN_STR;
				BEGIN(str);
			}
<str_bkslash>.		{
				*str_buf_ptr++ = *yytext;
				BEGIN(str);
			}
 /* normal characters */
<str>[^\0\\\n\"]	{
				if (error_flag) 	/*privous error */
				;			/* if overfow? */
				else if (str_buf_ptr - str_buf + yyleng >
					MAX_STR_LEN) {
					snl_yylval.err_msg = "string "
					"constant too long";
					error_flag = OVERFLOW;
					}
				else {			/* no error, copy the string */
					int i;
					for (i = 0; i < yyleng; i++) 
						*str_buf_ptr++ = yytext[i];
				}
			}

 /* String ends */
<<EOF>>			{	yyterminate();			}
.			{	snl_yylval.err_msg = yytext;
				return (ERROR);
			}
%%

