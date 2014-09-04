#ifndef DUMPTYPE_H
#define DUMPTYPE_H
//#include <stdio.h>
//incomplete struct
typedef struct List node;
typedef struct Program Program_;
//static int (*dump_type) (int n, node *pos) = (void *)0;

int dump_program(Program_ *p);
int dump_topic(int n, node *pos);
int dump_kpt(int n, node *pos);
int dump_func(int n, node *pos);
int dump_list(int n, node *pos);
int dump_proc(int n, node *pos);	/* implement next time */
int dump_attr(int n, node *pos);
int dump_const(int n, node *pos);
int dump_navig(int n, node *pos);
int dump_label_stat(int n, node *pos);
int dump_let_stat(int n, node *pos);
int dump_case_stat(int n, node *pos);
int dump_conn(int n, node *pos);
int dump_dispatch(int n, node *pos);
int dump_operation(int n, node *pos);
int dump_step(int n, node *pos);
int dump_proc(int n, node *pos);
#endif /* dumptype.h */
