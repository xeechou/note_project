%{
#include <stdio.h>
#include "ast_tree.h"
#include "snail_parser.h"
YYSTYPE snail_yylval;
#define yylex snail_yylex
#include "snail_tab.h"
extern int curr_lineno;
static int nested_cmtl = 0;

#define MAX_STR_LEN 1025
/* we alloc enough memory then 
 * we don't need to check buffer overflow */
extern char *str_buf;
char *str_buf_ptr;
int error_flag;
extern FILE *fin;
extern symbol_tab id_table;
extern symbol_tab str_table;
/* some assumptions */
/* define some error flag */

#define OVERFLOW	-1
#define NULL_IN_STR 	-2
#define EOF_IN_STR	-3
#define INVALID_CH	-4
/* TODO: 
 * >add id_str 
 * >change the str_buf, if we use the fsize to alloc str_buf, then we don't
 *  need to worry about memory overflow. 
 */
%}

%x			comment
%x			line_comment
%x			str
%x			obj_str
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
EXTERN_REG		extern
FUNC_REG		list
PROC_REG		proc

WHAT_REG		what
WHEN_REG		when
WHERE_REG		where
WHO_REG			who
HOW_REG			how
WHY_REG			why

CASE_REG		case
OF_REG			of
ESAC_REG		esac
ENUM_REG		enum
MUNE_REG		mune
LET_REG			let
IN_REG			in
TEL_REG			tel

CONNECT_REG		connect
WITH_REG		with
TO_REG			to
FROM_REG		from
ABOUT_REG		about
 /* end keywords */
ASSIGN			":"
ASSIGN_EQU		":="
ASSIGN_ADD		"+="
DOT			"."
TOPIC_QUOTE		"@"
SEMI_COLON		";"
LEFT_CURBRA		"{"
RIGHT_CURBRA		"}"

UNION			"|"
INTERSAC		"&"
DIFF			"-"
EQUAL			"=""="

LEFT_BRA		"("
COMMA			","
RIGHT_BRA		")"
%%
 /* 
  * Line_Comment
  */
"#"				BEGIN (line_comment);
<line_comment>[^\n]		/* no operation */
<line_comment>\n	{
				curr_lineno++;
				BEGIN (INITIAL);
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
<comment><<EOF>>	{	snail_yylval.err_msg = "EOF in comment!";
				BEGIN (INITIAL);
				return (ERROR);
			}
<comment>\n			curr_lineno++;
<comment>.		{}	/* do nothing */
 /* outside the comment */
"*)"			{	snail_yylval.err_msg = "Unmatched *)";
				BEGIN (INITIAL);
				return (ERROR);
			}
 /* end of comment */

 /* Operations */
{ASSIGN}		{	return (':');		}
{ASSIGN_EQU}		{	return (ASSIGN_E);	}
{ASSIGN_ADD}		{	return (ASSIGN_A);	}
{DOT}			{	return ('.');		}
{TOPIC_QUOTE}		{	return ('@');		}

{SEMI_COLON}		{	return (';');		}
{LEFT_CURBRA}		{	return ('{');		}
{RIGHT_CURBRA}		{	return ('}');		}
{EQUAL}			{	return ('=');		}

{LEFT_BRA}		{	return ('(');		}
{COMMA}			{	return (',');		}
{RIGHT_BRA}		{	return (')');		}

{UNION}			{	return ('|');		}
{INTERSAC}		{	return ('&');		}
{DIFF}			{	return ('-');		}

 /* end of Operations */

 /*
  * KEYWORDS are case-sensitive, actually just lower-case in SNL
  * except AKA
  */
{TOPIC_REG}		{ 	return (TOPIC);		}
{KPT_REG}		{	return (KPT);		}	
{AKA_REG}		{	return (AKA);		}
{EXTERN_REG}		{	return (EXTERN);	}
{FUNC_REG}		{	return (FUNC);		}
{PROC_REG}		{	return (PROC);		}

{WHAT_REG}		{	return (WHAT);		}
{WHEN_REG}		{	return (WHEN);		}
{WHERE_REG}		{	return (WHERE);		}
{WHO_REG}		{	return (WHO);		}
{HOW_REG}		{	return (HOW);		}
{WHY_REG}		{	return (WHY);		}

{CASE_REG}		{	return (CASE);		}
{OF_REG}		{	return (OF);		}
{ESAC_REG}		{	return (ESAC);		}
{LET_REG}		{	return (LET);		}
{IN_REG}		{	return (IN);		}
{TEL_REG}		{	return (TEL);		}

{CONNECT_REG}		{	return (CONNECT);	}
{WITH_REG}		{	return (WITH);		}
{TO_REG}		{	return (TO);		}
{FROM_REG}		{	return (FROM);		}
{ABOUT_REG}		{	return (ABOUT);		}

 /* Keywords ends */

{OBJID_REG}		{	snail_yylval.sym = st_add_string(&id_table, yytext);
				//snail_yylval.sym = st_add_string(&id_table, yytext,
				//					strlen(yytext));
				return (OBJECTID);
			}

{WHITESPACE}		{}
{NEWLINE_REG}		{	curr_lineno++;		}

\'			{	str_buf_ptr = str_buf;
				BEGIN(obj_str);
			}
<obj_str>\'		{	BEGIN (INITIAL);
				if (error_flag)
					return (ERROR);
				else
					str_buf_ptr = '\0';
					snail_yylval.sym = 
					st_add_string(&id_table, str_buf);
					//str_add_string(&id_table, str_buf,
					//		str_buf_ptr - str_buf);
					return (OBJECTID);
			}

<obj_str><<EOF>>	{	BEGIN(INITIAL);
				snail_yylval.err_msg = "EOF in object name";
				return (ERROR);
			}

<obj_str>\0		{	snail_yylval.err_msg = "id costains null characters.";
				error_flag = NULL_IN_STR;
			}
<obj_str>\n		{	BEGIN(INITIAL);
				if (error_flag) ;
				else 
					snail_yylval.err_msg = "Unterminated id name.";
				curr_lineno++;
				return (ERROR);
			}
<obj_str>[a-zA-Z0-9\ ]	{
				*str_buf_ptr++ = *yytext;
			}
 /* all the other charactors is invalid */
<obj_str>.		{	error_flag = INVALID_CH; 	}

 /* TODO: finish raw string */
 /*************** raw string scanner start *********************/

 /*************** string scanner start *************************/
\"			{	str_buf_ptr = str_buf;
				BEGIN(str);
			}

<str>\"			{	BEGIN(INITIAL);
				if (error_flag)
					return (ERROR);

				else {
					*str_buf_ptr = '\0';
					snail_yylval.sym = 
					st_add_string(&str_table, str_buf);
					//str_add_string(&str_table, str_buf,
					//		str_buf_ptr - str_buf);
					return (STR_CONST);
				}

			}
<str><<EOF>>		{	BEGIN(INITIAL);
				snail_yylval.err_msg = "EOF in string constant";
				return (ERROR);
			}
<str>\0			{	snail_yylval.err_msg = "String costains null "
				"characters.";
				error_flag = NULL_IN_STR;
			}
<str>\n			{	BEGIN(INITIAL);
				snail_yylval.err_msg = "Unterminated string constant.";
				curr_lineno++;
				return (ERROR);
			}

<str>\\			{	BEGIN(str_bkslash);		}

<str_bkslash>\"		{	*str_buf_ptr++ = '\"';		
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
				snail_yylval.err_msg = "EOF in string "
				"constant";
				error_flag = EOF_IN_STR;
				BEGIN(INITIAL);
			}
<str_bkslash>\0		{
				snail_yylval.err_msg = "String constains "
				"escaped null character";
				error_flag = NULL_IN_STR;
				BEGIN(str);
			}
<str_bkslash>.		{
				*str_buf_ptr++ = *yytext;
				BEGIN(str);
			}
 /* normal characters */
<str>[^\0\\\n\"]+	{
				if (error_flag) 	/*privous error */
				;
				/*
				else if (str_buf_ptr - str_buf + yyleng >
					MAX_STR_LEN) {
					snail_yylval.err_msg = "string "
					"constant too long";
					error_flag = OVERFLOW;
					}
				*/
				else {			/* no error, copy the string */
					int i;
					for (i = 0; i < yyleng; i++) 
						*str_buf_ptr++ = yytext[i];
				}
			}

 /* String ends */
<<EOF>>			{	yyterminate();			}
.			{	snail_yylval.err_msg = yytext;
				return (ERROR);
			}
%%

int yywrap(void) { return 1;}
