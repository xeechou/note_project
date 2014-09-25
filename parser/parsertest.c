#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "snail.h"
#define yyparse snail_yyparse

static char *SNAIL_LOCAL_TOKEN  = "local";
FILE *yyin;
int curr_lineno;		//notorious global
extern void gen_ast(SNAILState *s);
FILE *stream;
symbol_tab id_table;
symbol_tab str_table;
symbol_tab str_psr_tab;
symbol *what;
symbol *when;
symbol *where;
symbol *how;
symbol *why;
symbol *who;
symbol *local;
symbol *self;

char *start_prog;
char *start_topic;
char *start_kpt;
char *start_attr;
char *start_func;
char *start_arg;
char *start_id;
char *start_proc;
char *start_procc;
char *start_label_stat;
char *start_let;
char *start_case;
char *start_case_stat;
char *start_navig;
char *start_conn;
char *start_dispatch;
char *start_str;
char *start_oper;

char *end_prog;
char *end_topic;
char *end_kpt;
char *end_attr;
char *end_func;
char *end_arg;
char *end_id;
char *end_proc;
char *end_procc;
char *end_label_stat;
char *end_let;
char *end_case;
char *end_case_stat;
char *end_navig;
char *end_conn;
char *end_dispatch;
char *end_str;
char *end_oper;

static SNAILState *new_SNAILState(void)
{
	SNAILState *s = malloc(sizeof(SNAILState));

	err_pstrt_init(&s->nodes_data, sizeof(node_data), 8192, NULL);
	err_pstrt_init(&s->ast_nodes,  sizeof(ast_node) , 8192, NULL);


	local = malloc(sizeof(symbol));
	local->str = SNAIL_LOCAL_TOKEN;
	local->len = 5;

	//******************** set globals ********************//
	start_prog 	= "<program>";
	start_topic 	= "<topic>";
	start_kpt	= "<kpt>";
	start_attr	= "<attribute>";
	start_func	= "<function>";
	start_arg	= "<argument>";
	start_id	= "<id>";
	start_proc	= "<procedure>";
	start_procc	= "<step>";
	start_label_stat= "<label_statement>";
	start_let	= "<let_statement>";
	start_case	= "<case_statement>";
	start_case_stat = "<case_branch>";
	start_navig	= "<navigation>";
	start_conn	= "<connection>";
	start_dispatch	= "<dispatch>";
	start_str	= "<string>";
	start_oper	= "<operation>";
	
	end_prog	= "</program>";
	end_topic	= "</topic>"; 
	end_kpt         = "</kpt>";
	end_attr	= "</attribute>";
	end_func	= "</function>";
	end_arg		= "</argument>";
	end_id		= "</id>";
	end_proc	= "</procedure>";
	end_procc	= "</step>";
	end_label_stat	= "</label_statement>";
	end_let		= "</let_statement>";
	end_case	= "</case_statement>";
	end_case_stat	= "</case_branch>";
	end_navig	= "</navigation>";
	end_conn	= "</connection>";
	end_dispatch	= "</dispatch>";
	end_str		= "</string>";
	end_oper	= "</operation>";
	return s;
}

int main(int argc, char **argv)
{
	SNAILState *s = new_SNAILState();
	stream = stdout;
	// for (i = n_files; i > 0; i--)
	strcpy(s->curr_filename, "Test/test.note");
	yyin = fopen(s->curr_filename, "r");
	s->curr_file = yyin;
	struct stat st;
	if (yyin == NULL) {
		fprintf(stderr, "Cannot open file!\n");
		exit(1);
	}
	fstat(fileno(yyin), &st);
	s->curr_file_size = st.st_size;
	s->str_buf = malloc(st.st_size + 1 * sizeof(char));
	if (s->str_buf == NULL) {
		fprintf(stderr, "Cannot allocate memory!\n");
		exit(1);
	}
	st_init(&s->id_table, st.st_size,
			&symbol_hash);
	st_init(&s->str_table, st.st_size,
			&string_hash);
	st_init(&s->str_psr_tab, st.st_size,
			&string_hash);

	what  = st_add_string(&s->id_table, "what");
	when  = st_add_string(&s->id_table, "when");
	who   = st_add_string(&s->id_table, "who");
	where = st_add_string(&s->id_table, "where");
	why   = st_add_string(&s->id_table, "why");
	how   = st_add_string(&s->id_table, "how");
	self  = st_add_string(&s->id_table, "self");

	curr_lineno = 0;
	gen_ast(s);
	dump_type(0, s->ast_root);
	fclose(yyin);
	return 0;
}
