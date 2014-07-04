#ifndef DUMPTYPE_H
#define DUMPTYPE_H
//#include <stdio.h>
//incomplete struct
typedef struct List node;
static int (*dump_type) (int n, node *pos) = (void *)0;

int dump_topic(int n, node *pos);
int dump_kpt(int n, node *pos);
int dump_list(int n, node *pos);
int dump_proc(int n, node *pos);	/* implement next time */
int dump_expr(int n, node *pos);
int dump_const(int n, node *pos);
int dump_formal_navig(int n, node *pos);
int dump_enum_navig(int n, node *pos);
int dump_let_navig(int n, node *pos);
int dump_case_navig(int n, node *pos);
int dump_conn(int n, node *pos);

#endif /* dumptype.h */
