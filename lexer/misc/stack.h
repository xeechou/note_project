#ifndef MYSTACK_H
#define MYSRACK_H
typedef struct {
	void *elems;
	int esize;
	int log_len; 
	int alloc_len;
	void (*func) (void *);
} stack;
void stack_init(stack *s, int esize, void (*func)(void *), int init_alloc);

void stack_dispose(stack *s);

int stack_find(stack *s, const void *elemaddr);

int stack_isempty(stack *s);

void stack_pop(stack *s, void *ret_addr);

void stack_push(stack *s, void *elem_addr);



#endif /*stack.h*/
