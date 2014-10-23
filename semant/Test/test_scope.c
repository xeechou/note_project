#include "../snail_scope.c"

/* test scope entering functions */
#include <stdio.h>
int curr_lineno = 1;
FILE *stream;
static void type_env_test(scope *s);
int main()
{
	scope s;
	scope_init(&s);
	type_env_test(&s);
}

static type_env root;
static void type_env_test(scope *s)
{
	scope_enter(s, &root);
	scope_exit(s);
	printf("first step!\n");
}