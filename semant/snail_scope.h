#ifndef SNAIL_SCOPE_H
#define SNAIL_SCOPE_H
#include "snail_tab.h"

/* okay I decide to use the typical stack policy to implement scope */

/* scope items is what to search in the type-checking */
struct scope_item {
	symbol *name;
	int type;
	void *data;
	struct scope_item *next;
} __attribute__(( aligned (8)));
typedef struct scope_item scope_item;

typedef struct global {
	symbol *topic;
	symbol *feature;	/* can be kpt, func, attr */
	symbol *attr;		/* can be attr or arg */
	int type;
	void *data;		/* its right value */
	struct global *next;	/* until it is empty */
}global __attribute__(( aligned (8)));

/* maybe I should go with the stack, either the data structure should provide
 * the problem is I need scan it at list twice, so I cannot use stack, cause
 * they will be */

struct type_env {
	scope_item *head;
	size_t len;
	struct type_env *parent;
} __attribute__(( aligned (8)));
typedef struct type_env type_env;

#include "misc/misc_types.h"
/* used to track current enviornment */
#define SCOPE_OPT	0x0f
#define SCOPE_NOR	0x00
#define SCOPE_REC	0x01

#define OPER_OPT	0xf0
#define SCOPE_RM	0x10
#define SCOPE_FIND	0x20
#define SCOPE_ADD	0x30
typedef struct {
	stack env_stack;
	page_strt items;
} scope __attribute__((aligned(8)));

/* enter scope, lookup table, leave scope, insert, delete symbol */
/* symbol table should be a tree */
/* the type system?  */

void scope_init(scope *s);

void scope_enter(scope *s, type_env *newenv_ptr);
void scope_exit(scope *s);

scope_item *scope_add_item(scope *s, type_env *cur_env, scope_item *item);
scope_item *scope_op_item(type_env *env, symbol *a, int local);

/*ALERT: the return vaule of typing function group is ethier 
 * 0:		just return;
 * positive:	TYPE;
 * negative:	error number;
 */

#endif /* scope.h */
