#ifndef SNL_CONST_H
#define SNL_CONST_H

/* const used in ast_tree.h */

/* expr_identity */
#define CASE_EXPR 1

/* error flag */
#define MEMOVERFLOW 	-1
#define NO_ARR_IN_TABLE	-2
#define NOT_LAST_STR	-3
#define STR_TOOLONG	-4


/* type define */
#define KPT_TYPE	1
#define PROC_TYPE	2
#define LIST_TYPE	3

#define ID		1
#define STRING		2

/* connection */
#define TO_CONN		1
#define WITH_CONN	2
#define FROM_CONN	3
#endif /* SNL_CONST_H */
