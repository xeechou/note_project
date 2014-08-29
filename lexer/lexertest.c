#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "snail.h"
#include "snail_tab.h"
void show_snl_token(FILE *file, int lineno, int token, YYSTYPE yylval);
char *token_to_string(int token);

extern int yylex();
extern int yy_flex_debug;


FILE *xfopen(const char *filename, const char *how)	/* stop from here */
{
	FILE *fp;
	if (strcmp(filename, "stdin") == 0)
		return stdin;
	if (strcmp(filename, "stdout") == 0)
		return stdout;
	/* other condition, just pass the paramters */
	fp = fopen(filename, how);
	return fp;
}
void xfclose(FILE *fp)
{
	int fd = fileno(fp);
	if (fd < 2)
		return;
	else
		fclose(fp);
}

int curr_lineno = 1;
extern FILE *yyin;

/* escape the argv[0], all other arguments is input file */

symbol_tab id_table;
symbol_tab str_table;
char *str_buf;
int main(int argc, char **argv)
{
	int nfiles;
	int i = nfiles = (argc > 1) ? argc-1 : 1;
	/* trick here is let files[0] to be stdin first, then see if it will
	 * get replaced by 'real' file, so we avoid branch */
	char *files[nfiles];
	size_t fsize;
	struct stat st;
	files[0] = "stdin";
	while (i > 0 )  {
		files[i-1] = argv[i];
		i--;
	}
	int token;
	for (i = 0; i < nfiles; i++) {
		// open file and get state
		yyin = xfopen(files[i], "r");
		if (yyin == NULL) {
			fprintf(stderr, "cannot open input file %s\n", files[i]);
			exit(1);
		}
		fstat(fileno(yyin), &st);
		fsize = st.st_size;
		////////////////////////////
		st_init(&id_table, fsize, 
				&symbol_hash);
		st_init(&str_table, fsize,
				&string_hash);
		str_buf = malloc((fsize + 1) * sizeof(char) );
		curr_lineno = 1;
		// scan the input file 
		printf("#filename \"%s\"\n", files[i]);
		while ((token = yylex()) != 0) {
			show_snl_token(stdout, curr_lineno, token, snail_yylval);
		}
		st_dispose(&id_table);
		st_dispose(&str_table);
		free(str_buf);
		xfclose(yyin);
	}
	return 0;
}

void show_snl_token(FILE *file, int lineno, int token, YYSTYPE yylval)
{
	char *msg = NULL;
	switch (token) {
		case STR_CONST :
			msg = (snail_yylval.sym)->str;
		break;
		case OBJECTID :
			msg = (snail_yylval.sym)->str;
		break;
		case ERROR :
			msg = snail_yylval.err_msg;
		break;
	}
	fprintf(file, "#lineno %4d %s ", lineno, token_to_string(token));
	if (msg == NULL)
		fprintf(file, "\n");
	else 
		fprintf(file, "%s\n", msg);
}

char *token_to_string(int token)
{
	switch (token) {
		case 0 :	return "EOF";		break;
		case STR_CONST: return "STR_CONST";	break;
		case OBJECTID:	return "OBJECTID";	break;
		case TOPIC:	return "TOPIC";		break;
		case KPT:	return "KPT";		break;
		case AKA:	return "AKA";		break;
		case WHAT:	return "WHAT";		break;
		case WHEN:	return "WHEN";		break;
		case HOW:	return "HOW";		break;
		case WHY:	return "WHY";		break;
		case OF:	return "OF";		break;
<<<<<<< HEAD
		//case COMPARES:	return "COMPARES";	break;
		//case REF:	return "REF";		break;
=======
>>>>>>> parser
		case ERROR:	return "ERROR";		break;
		case ASSIGN_E:	return ":=";		break;
		case ASSIGN_A:  return "+=";		break;
		case ':':	return "':'";		break;
		case ',':	return "','";		break;
		case '.':	return "'.'";		break;
		case ';':	return "';'";		break;
		case '{':	return "'{'";		break;
		case '}':	return "'}'";		break;
		default:	return "<Invalid Token>";
	}
}
