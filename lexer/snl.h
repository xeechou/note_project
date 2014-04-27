#ifndef SNL_H
#define SNL_H 1

/* keywords */
enum {
	STR_CONST = 150,
	OBJECTID = 151,
	TOPIC = 201,
	KPT = 202,
	AKA = 203,
	WHAT = 204,
	WHEN = 205,
	HOW = 206,
	WHY = 207,
	OF  = 208,
	COMPARES = 209,
	REF = 210,
	ERROR = 100
};

/* and I need some types to work */
/* some use YYSTYPE and 
 * extern YYSTYPE yylval to declare the yylval
 */
#include "stringtab.h"
typedef union snl_yylval
{
	Symbol symbol;
	char *err_msg;
}YYSTYPE;

YYSTYPE snl_yylval;

#endif /*SNL_H */
