#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snl_const.h"
#include "dumptype.h"
#include "snl_stringtab.h"
Program_ *ast_root;
extern FILE *yyin;
extern int yylex();
extern int yyparse();
char curr_filename[256];

extern void initialize_constants(void);

int main(int argc, char **argv)
{
	strcpy(curr_filename, "Test/test.note");
	initialize_constants();
	/* open file, and read. */
	FILE *f = fopen(curr_filename, "r");
	if (f == NULL) {
		fprintf(stderr, "Cannot open file!\n");
		exit(1);
	}
	yyin = f;
	yyparse();
	dump_program(ast_root);
	fclose(f);
	return 0;
}

