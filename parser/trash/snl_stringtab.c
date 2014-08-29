#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snl_stringtab.h"
#include "snl_const.h"
#include "stringtab.h"

str_array id_arr;
str_array str_arr;

void die(int errorno)
{
	fprintf(stderr, "died\n");
	exit(1);
}
/* add src to arr which is almost 1024 size long
 * change the str address */ 
/* this function assumes that arr, src, len, *dest is valid,
 * and len is less than STRLEN_MAX */ 
char *lookup_table(char *s, str_array *arr)
{
	char *ptr = arr->array;
	char *final = arr->array + arr->num;
	int len;
	do {
		len = strlen(ptr);
		if (strcmp(s, ptr) == 0)
			return ptr;
		ptr += len + 1;
	} while(final - ptr > 0);
	return NULL;
}
/* add single string to the dynamic array, with check mechanism */
static int add_stringtable(str_array *arr, char *src, int len, char **dest)
{
	char *tmp;
	if (arr->array == NULL) {
		if ((arr->array = 
			(char *)malloc(NINIT*STRLEN_MAX*sizeof(char)))
		 == NULL)
			return MEMOVERFLOW;
		arr->max = NINIT*STRLEN_MAX;
		arr->num = 0;
	} else if (
		arr == &id_arr && (tmp = lookup_table(src, arr)) != 0 ) {
		*dest = tmp;
		return 0;
	} else if (arr->num + len >= arr->max) {
		tmp = (char *) realloc(arr->array, arr->max*NGROW); 
		if (tmp == NULL)
			return MEMOVERFLOW;
		arr->max *= NGROW;
		arr->array = tmp;
	}
	*dest = arr->array + arr->num;
	strncpy(*dest, src, len);
	arr->num += len;
	return 0;
}

str_symbol single_string(Symbol a, str_array *arr)
{
	str_symbol s = malloc(sizeof(*s));
	int err = add_stringtable(arr, a->str, a->len, &(s->str));
	if (err != 0) 
		die(err);
	s->len = a->len;
	/* release the node in list */
	list_rm(&(a->list), a->list.prev, a->list.next);
	free(a);
	return s;
}
/* append string a to b, for now it only works when b is the last string in arr */
str_symbol 
append_string(str_symbol b, Symbol a, str_array *arr)
{
	/* check if b is the last one in the arr */
	if (arr->array == NULL)
		die(NO_ARR_IN_TABLE);
	else if (b->str + b->len != arr->array + arr->num)
		die(NOT_LAST_STR);

	else {
		int err;
		char *trash;	/* trash here is for the sake of b->str */
		arr->num -= 1;
		if ((err = add_stringtable(arr, a->str, a->len, &trash)))
			die(err);
	}
	b->len += a->len - 1;
	list_rm(&(a->list), a->list.prev, a->list.next);
	free(a);
	return b;
}

/* here we need to install basic attrs
 * what, when, how, why, where
 */
void initialize_constants(void)
{
	/* fix the exit(1) later */
	int err;
	what  = malloc(sizeof(* what));
	what->len = 4;
	if ((err = 
	   add_stringtable(&id_arr, "what", 5, &(what->str))) ) {
		exit(1);
	};

	when  = malloc(sizeof(* when));
	when->len = 4;
	if ((err = 
	   add_stringtable(&id_arr, "when", 5, &(when->str))) ) {
		exit(1);
	};

	where = malloc(sizeof(*where));
	where->len = 5;
	if ((err = 
	   add_stringtable(&id_arr, "where", 6, &(where->str))) ) {
		exit(1);
	}

	who   = malloc(sizeof(*  who));
	who->len = 3;
	if ((err = 
	add_stringtable(&id_arr, "who", 4, &(who->str))) ) {
		exit(1);
	}

	why   = malloc(sizeof(*  why));
	why->len = 3;
	if ((err = 
	add_stringtable(&id_arr, "why", 4, &(why->str))) ) {
		exit(1);
	}

	how   = malloc(sizeof(*  how));
	how->len = 3;
	if ((err = 
	add_stringtable(&id_arr, "how", 4, &(how->str))) ) {
		exit(1);
	}
}
