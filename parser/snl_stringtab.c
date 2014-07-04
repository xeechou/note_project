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
	//len += 1;		/* for '\0' */
	if (arr->array == NULL) {
		arr->array = 
			(char *)malloc(NINIT*STRLEN_MAX*sizeof(char));
		if (arr->array == NULL)
			return MEMOVERFLOW;
		arr->max = NINIT*STRLEN_MAX;
		arr->num = 0;
	} else if (
		tmp = lookup_table(src, arr)) {
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

/* append a string b after a, only invoke this when a is the last string in
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
	arr->num + lenb;
	return 0;

}
/* add a single string to a table and return a str_symbol pointer */
/* no check mechanism */
str_symbol single_string(Symbol a, str_array *arr)
{
	str_symbol s = malloc(sizeof(*s));
	s->len = a->len;
	int err = add_stringtable(arr, a->str, s->len+1, &(s->str));
	if (err != 0) 
		die(err);
	free(a);
	return s;
}

/* append a string to given table, it means that we need to del the '\0' at
 * ptr, add append there, okay, I need a deeper thought. */
/* add two strings in the str_array arr, sb is follow the sa */
str_symbol 
append_string(str_symbol b, Symbol a, str_array *arr)
{
	int err = concat_strtab(b->str,  b->len+1,
                               (a->str), a->len+1, arr);
	if (err != 0) {
		printf("err using concat\n");
		exit(1);
	}
	b->len += a->len;
	free(a);
	return b;
}
int main()
{
	char *tstr = "this is a test, you should be as verbose as possible."; 
	char *adc = "this is a test, you should be as verbose as possible.";

	int len = strlen(tstr);
	Symbol a = malloc(sizeof(*a) + len * sizeof(char));
	strcpy(a->str, tstr);
	a->len = len;
	Symbol b = malloc(sizeof(*b) + len * sizeof(char));
	strcpy(b->str, tstr);
	b->len = len;
	
	//char *c, *d;
	//add_stringtable(&str_arr, tstr, len+1, &c);
	//add_stringtable(&str_arr, adc, len+1, &d);
	//if (c == d)
	//	printf("checked\n");
	str_symbol c = single_string(a, &str_arr);
	str_symbol d = single_string(b, &str_arr);
	str_symbol c = append_string(c, b, &str_arr);
	//This is gonna be wrong!!!
	str_symbol f = single_string(a, &str_arr);
	str_symbol f = append_string(c, f, &str_arr);
	//printf("%s\n", d->str);
}
