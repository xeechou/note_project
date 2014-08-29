#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "snl_const.h"
#include "dumptype.h"
#include "snail_tab.h"
#define yyparse snail_yyparse
Program_ *ast_root;
FILE *yyin;
extern int yylex();
extern int yyparse();
char curr_filename[256];

char *str_buf;
symbol_tab id_table;
symbol_tab str_table;
symbol_tab str_psr_tab;
symbol *what;
symbol *when;
symbol *where;
symbol *why;
symbol *who;
symbol *how;
void init(size_t);
int main(int argc, char **argv)
{
	strcpy(curr_filename, "../Test/test.note");
	/* open file, and read. */
	yyin = fopen(curr_filename, "r");
	struct stat st;
	if (yyin == NULL) {
		fprintf(stderr, "Cannot open file!\n");
		exit(1);
	}
	fstat(fileno(yyin), &st);
	str_buf = malloc(st.st_size + 1 * sizeof(char));

	init(st.st_size);
	yyparse();
	dump_program(ast_root);
	fclose(yyin);
	return 0;
}
void init(size_t fsize)
{
	st_init(&id_table, fsize,
			&symbol_hash);
	st_init(&str_table, fsize,
			&string_hash);
	st_init(&str_psr_tab, fsize,
			&string_hash);
	what  = st_add_string(&id_table, "what");
	when  = st_add_string(&id_table, "when");
	who   = st_add_string(&id_table, "who");
	where = st_add_string(&id_table, "where");
	why   = st_add_string(&id_table, "why");
	how   = st_add_string(&id_table, "how");
}
