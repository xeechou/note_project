#ifndef SNAIL_PARSER_H
#define SNAIL_PARSER_H

/* keywords */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
enum yytokentype {
	STR_CONST = 150,
	OBJECTID = 151,
	ASSIGN_E = 152,
	ASSIGN_A = 153,

	TOPIC  = 201,
	KPT    = 202,
	AKA    = 203,
	EXTERN = 204,
	FUNC   = 205,
	PROC   = 206,

	WHAT  = 251,
	WHEN  = 252,
	HOW   = 253,
	WHY   = 254,
	WHERE = 255,
	WHO   =	256,

	CASE  =	301,
	OF    = 302,
	ESAC  =	303,
	ENUM  =	304,
	MUNE  =	305,
	LET   = 306,
	IN    =	307,
	TEL   =	308,

	CONNECT = 351,
	WITH    = 352,
	TO      = 353,
	FROM    = 354,
	ABOUT   = 355,

	ERROR = 100
};
#endif
/* and I need some types to work */
/* some use YYSTYPE and 
 * extern YYSTYPE yylval to declare the yylval
 */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
#include "ast_tree.h"
#include "snail_tab.h"
typedef union
{
	Program_ program__;
	Kpt_ *kpt__;
	node *node_;
	Func_ *func__;
	symbol *sym;
	char *err_msg;
}YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif
#define yylval snail_yylval

#endif /* snail_parser.h */
