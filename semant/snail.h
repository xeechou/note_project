#ifndef SNAIL_H
#define SNAIL_H
#include <stdio.h>
#include "misc/misc_types.h"
#include "snail_tab.h"
#include "snl_const.h"
extern char *start_prog;
extern char *start_topic;
extern char *start_kpt;
extern char *start_attr;
extern char *start_func;
extern char *start_arg;
extern char *start_id;
extern char *start_proc;
extern char *start_procc;
extern char *start_label_stat;
extern char *start_let;
extern char *start_case;
extern char *start_case_stat;
extern char *start_navig;
extern char *start_conn;
extern char *start_dispatch;
extern char *start_str;
extern char *start_oper;

extern char *end_prog;
extern char *end_topic;
extern char *end_kpt;
extern char *end_attr;
extern char *end_func;
extern char *end_arg;
extern char *end_id;
extern char *end_proc;
extern char *end_procc;
extern char *end_label_stat;
extern char *end_let;
extern char *end_case;
extern char *end_case_stat;
extern char *end_navig;
extern char *end_conn;
extern char *end_dispatch;
extern char *end_str;
extern char *end_oper;

/* define types */
#define TP_NONE		0
#define TP_TOPIC	1	//fairly speaking, topic is not a type in SNAIL
#define TP_FUNC		5	//func is not a topic, either

#define TP_ATTR		2
#define TP_KPT		3
#define TP_ID		6	//2 * 3
#define TP_STRING 	7
#define TP_LC		42	//6 * 7
#define TP_PROC		41
#define TP_CONST	1722	//41 * 42
#define TP_CONN		1721
#define TP_OBJ		2963562	//1722 * 1721


#include "ast_tree.h"


/* essential data, num ast node elems and index */
#define OD_PROG_TOP	0
#define N_PROG_ELEM	1

#define N_TOPIC_ELEM	2
#define OD_TOPIC_NAME	0
#define OD_TOPIC_FEAT	1

#define N_KPT_ELEM	4
#define OD_KPT_NAME	0
#define OD_KPT_AKA   	1
#define OD_KPT_SUBS	2
#define OD_KPT_ATTRS 	3

#define N_KPT_SIMPLE_E 	3

#define N_FUNC_ELEM	3
#define OD_FUNC_NAME 	0
#define OD_FUNC_ARG	1
#define OD_FUNC_STAT 	2

#define N_ATTR_ELEM	3
#define OD_ATTR_NAME	0
#define OD_ATTE_EXPR	1
#define OB_ATTR_MODE 	2

#define N_LET_ELEM	2
#define OD_LET_ID	0
#define OD_LET_EXPR	1

#define N_CASE_ELEM	2
#define OD_CASE_NAME	0
#define OD_CASE_STAT	1

#define N_LAB_ELEM	3
#define OD_LAB_NV	0
#define OD_LAB_EXPR	1
#define OD_LAB_MODE	2

#define N_CON_ELEM	3
#define OD_CON_NV	0
#define OD_CON_EXPR	1
#define OD_CON_MODE  	2

#define N_DIS_ELEM	3
#define OD_DIS_TOP	0
#define OD_DIS_NAME	1
#define OD_DIS_EL	2

#define N_OPER_ELEM	3
#define OD_OPER_TOP	0
#define OD_OPER_NAME	1
#define OD_OPER_EL	2

#define N_NAVIG_ELEM	3
#define OD_NAVIG_TOP	0
#define OD_NAVIG_NAME	1
#define OD_NAVIG_EL	2

#define N_PROC_ELEM	2
#define OD_PROC_NAME	0
#define OD_PROC_COMP	1

#define N_STEP_ELEM	3
#define OD_STEP_TOP	0
#define OD_STEP_NAME	1
#define OD_STEP_EL	2
/* essential data, num ast node elems and index */

typedef struct SNAILState {
	FILE **files;
	unsigned int n_files;
	
	/* current file stuff */
	char curr_filename[1024];
	FILE *curr_file;
	size_t curr_file_size;
	char *str_buf;
	
	symbol_tab id_table;
	symbol_tab str_table;
	symbol_tab str_psr_tab;
	page_strt nodes_data;
	page_strt ast_nodes;


	ast_node *ast_root;
} SNAILState;

#endif
