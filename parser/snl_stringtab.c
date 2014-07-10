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
int add_stringtable(str_array *arr, char *src, int len, char **dest)
{
	char *tmp;
	if (arr->array == NULL) {
		arr->array = 
			(char *)malloc(NINIT*STRLEN_MAX*sizeof(char));
		if (arr->array == NULL)
			return MEMOVERFLOW;
		arr->max = NINIT*STRLEN_MAX;
		arr->num = 0;
	} else if ( 	/* if we tried to insert a OBJECTID in arr that already exists */
			/* notice we don't check if array is str_arr */
		arr == &id_arr && (tmp = lookup_table(src, arr)) != 0 ) {
		*dest = tmp;
		return 0;
	} else if (arr->num + len >= arr->max) {
		tmp = (char *)realloc(arr->array, 
				arr->max*NGROW); /* if not null, the len of array must be bigger than 
									STRLEN_MAX */
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

/* append string b after a, only invoke this when a is the last string in
 * table */
/* length included in '\0' */
int concat_strtab(char *a, int lena,
		char *b, int lenb,
		str_array *arr)
{
	char *ptr = arr->array + arr->num;

	if (arr->array == NULL)		/* if arr not initialized */
		return NO_ARR_IN_TABLE;
	if (a + lena != ptr)		/* if a is not last string in table */
		return NOT_LAST_STR;

	ptr--; arr->num--;
	if (arr->num + lenb >= arr->max) {
		char *tmp = (char *)realloc(arr->array,
				arr->max * NGROW);
		if (tmp == NULL)
			return MEMOVERFLOW;
		arr->max *= NGROW;
		arr->array = tmp;
	}
	strncpy(ptr, b, lenb);
	arr->num += lenb;
	return 0;

}
/* add a single string to a table and return a str_symbol pointer
assuming string length = strlen(str) + 1*/
str_symbol single_string(Symbol a, str_array *arr)
{
	str_symbol s = malloc(sizeof(*s));
	s->len = a->len;
	int err = add_stringtable(arr, a->str, s->len, &(s->str));
	if (err != 0) 
		die(err);
	/* release the node in list */
	list_rm(&(a->list), a->list.prev, a->list.next);
	free(a);
	return s;
}

/* append a string to given table, it means that we need to del the '\0' at
 * ptr, add append there, okay, I need a deeper thought. */
/* add two strings in the str_array arr, sb is follow the sa
assuming string length = strlen(str) + 1*/
str_symbol 
append_string(str_symbol b, Symbol a, str_array *arr)
{
	int err = concat_strtab(b->str,  b->len,
                               (a->str), a->len, arr);
	if (err != 0) {
		printf("error using concat\n");
		exit(1);
	}
	/* mistake: len(b) - 1 for the removed '\0' */
	b->len += a->len - 1;
	list_rm(&(a->list), a->list.prev, a->list.next);
	free(a);
	return b;
}

int main()
{
	strtable_init();
	char *tstr = "This is a test!";
	int i;
	str_symbol test = single_string(
		       add_string(strtable, tstr),
		       &str_arr);

	for (i = 0; i < 100; i++) {
		test = append_string(
				test,
				add_string(strtable, tstr),
				&str_arr);
	}
	printf("%s\n", test->str);
}
