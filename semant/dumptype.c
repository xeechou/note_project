#include <stdio.h>
#include "ast_tree.h"

extern FILE *stream;
static char *padding = "                                                                                ";      // 80 spaces for padding

/* pad function will point to the exact position of padding,
 * then return the position */
static inline char *pad(int n)
{
	if (n > 80) 
		return padding;
	if (n <= 0)
		return "";
	else
		return padding + (80 - n);
}

static void dump_line(int n, int lineno)
{
	char *p = pad(n);
	fprintf(stream, "%s #%d\n", p, lineno);
}

static inline char *sym_gstr(symbol *s)
{
	return (char *)(((symbol *)s)->base->elems) + 
		((symbol *)s)->index;
}

static void dump_symbol(int n, symbol *sym)
{
	if (sym == NULL)
		return;
	char *p = pad(n);
	fprintf(stream, "%s #%s\n", p, sym_gstr(sym));		
}
static void dump_int(int n, int a)
{
	char *p = pad(n);
	fprintf(stream, "%s Data is #%d\n", p, a);
}
static void dump_str(int n, char *s)
{
	char *p = pad(n);
	fprintf(stream, "%s #%s\n", p, s);
}
int dump_type(int n, ast_node *an)
{
	dump_line(n, an->info.curr_lineno);
	dump_str(n, an->start.name);
	n = n + 2;
	// dump content
	int i;
	node_data *d = an->items;
	for (i = 0; i < an->n_nodes; i++) {
		switch(d->type) {
			case NT_SYM:
				dump_symbol(n, d->sym);
				break;
			case NT_NODE:
				if (d->n == NULL)
					break;
				else
					(d->n->info).dump(n, d->n);
				break;
			case NT_INT:
				dump_int(n, d->i);
				break;
		}
		d = d->next;
	}
	dump_str(n, an->end.name);

	return 0;
}
