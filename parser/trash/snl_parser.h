#ifndef SNL_PARSER_H
#define SNL_PARSER_H

/* keywords */
enum {
	STR_CONST = 150,
	OBJECTID = 151,

	TOPIC  = 201,
	KPT    = 202,
	AKA    = 203,
	EXTERN = 204,
	LIST   = 205,
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

/* and I need some types to work */
/* some use YYSTYPE and 
 * extern YYSTYPE yylval to declare the yylval
 */
#include "ast_tree.h"
#include "stringtab.h"
typedef union snl_yylval
{
	Program_ program__;
	Kpt_ *kpt__;
	List_ *list__;
	node *node_;
	Symbol  symbol;
	str_symbol strsymbol;
	char *err_msg;
}YYSTYPE;

YYSTYPE snl_yylval;

#endif /*SNL_PARSER_H */
