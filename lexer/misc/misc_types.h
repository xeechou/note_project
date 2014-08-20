#ifndef MISC_TYPES_H
#define MISC_TYPES_H

/***************************** stack begin ***********************************/
typedef struct {
	void *elems;
	size_t esize;
	size_t log_len; 
	size_t alloc_len;
	/* the user should know the size they compare */
	int (*cmp) (const void *, const void *);
	void (*func) (void *);
} stack;
void stack_init(stack *s, size_t esize,  size_t init_alloc, 
		int (*cmp) (const void *, const void *),
		void (*func)(void *));

void stack_dispose(stack *s);

int stack_find(stack *s, const void *elemaddr);

int stack_isempty(stack *s);

void stack_pop(stack *s, void *ret_addr);

void stack_push(stack *s, void *elem_addr);
/****************************** stack end *************************************/

/***************************** slots begin ************************************/
typedef int (*slots_com_f) (const void *elemAddr1,
			const void *elemAddr2);
typedef void (*slots_free_f) (void *elemAddr);
typedef void (*slots_map_f) (void *elemAddr, void *auxData);

typedef struct {
	void *elems;
	size_t esize;
	size_t log_len;
	size_t alloc_len;
	void (*func) (void *);
	stack frags;
} slots;

void  slots_init(slots *s, size_t esize,
		 size_t init_alloc, 
		 //int (*cmp)(const void *, const void *),
		 void (*func)(void *));

void    slots_dispose(slots *s);
size_t  slots_length(const slots *s);
void   *slots_nth(slots *s, size_t pos);
void   *slots_insert(slots *s, const void *elemAddr);
void    slots_replace(slots *s, const void *elemAddr, size_t pos);
size_t  slots_getpos(slots *s, void *addr);
void    slots_delete(slots *s, size_t pos, void **ret_addr);
void    slots_pop(slots *s, void *ret_addr);

void  slotsSearch(const slots *s, const void *key,
		   slots_com_f searchfn, size_t start_index,
		   size_t is_sorted);

void slotsSort(slots *s, slots_com_f cmpfn);

void slotsMap(slots *s, slots_map_f mapfn, void *auxData);

/****************************** slots end *************************************/


/**************************** smmblk begin ************************************/

typedef struct {
	void *elems;
	size_t esize;
	size_t log_len;
	size_t alloc_len;
	size_t sec_size;		//secured size
	void (*func) (void *);
} smmblk;

void smm_init(smmblk *s, size_t esize, size_t init_alloc, size_t sec_size,
		void (*func) (void *));

void smm_dispose(smmblk *s);
void smm_append(smmblk *s, void *elem_addr, void **ret_addr, size_t num);
void smm_delete(smmblk *s, void **ret_addr, size_t num);
void smm_pop(smmblk *s, void *ret_addr, size_t num);
/***************************** smmblk end *************************************/

/***************************** hash begin *************************************/
typedef volatile struct lhash_elem {
	volatile struct lhash_elem *next;
	char data[];
} lhash_elem;


typedef struct {
	lhash_elem *(*table);	/* lhash_elem *table[] */
	size_t esize;		/* the true data size */
	size_t hesize;		/* hash elem's size */
	size_t log_len;
	size_t alloc_len;
	slots loc;	/* where I stores true elems */
	int (* cmp) (const void *, const void *);
	unsigned long (* hash) (void *, size_t);
} lhash_tab;

void lht_init(lhash_tab *lht, size_t esize, size_t init_alloc, 
		size_t (*hash) (void *, size_t), 
		int (*cmp) (const void *, const void *),
		void (*func) (void *));

void lht_dispose(lhash_tab *lht);

void *lht_lookup(lhash_tab *lht, void *elem_addr, int create);

void lht_delete(lhash_tab *lht, void *elem_addr);

/****************************** hash end **************************************/


#endif /*misc_types.h*/
