#ifndef MISC_TYPES_H
#define MISC_TYPES_H
#include <stddef.h>
/*TODO: add a copy function, memcpy is not efficient enough */

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

void stack_push(stack *s, const void *elem_addr);
/****************************** stack end *************************************/

/***************************** slots begin ************************************/
typedef int (*slots_com_f) (const void *elemAddr1,
			const void *elemAddr2);
typedef void (*slots_free_f) (void *elemAddr);
typedef void (*slots_map_f) (void *elemAddr, void *auxData);
/* export outside, this represent how long the index can be */
typedef size_t slot_ind_t;

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
void    slots_delete(slots *s, size_t pos, void **ret_addr);
void    slots_pop(slots *s, void *ret_addr);
size_t  slots_getpos(slots *s, void *pos);

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
typedef size_t smm_t;
void smm_init(smmblk *s, size_t esize, size_t init_alloc, size_t sec_size,
		void (*func) (void *));
#define smm_nth(s, index) \
	(void *)((char *)(s->elems) + s->esize * index)
#define smm_getpos(s, addr) \
	(size_t)( ((char *)addr - (char *)(s->elems))  / s->esize)
void smm_dispose(smmblk *s);
void smm_append(smmblk *s, void *elem_addr, void **ret_addr, size_t num);
void smm_delete(smmblk *s, void **ret_addr, size_t num);
void smm_pop(smmblk *s, void *ret_addr, size_t num);
/***************************** smmblk end *************************************/

/***************************** hash begin *************************************/
#define HT_CRE		1
#define HT_NOC		0

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

#define OHT_NIL		0
#define OHT_DELETED	1
#define OHT_CHK		2
#define OHT_INIT	3
#define OHT_DEL		4
#define OHT_OHTER	5

typedef struct {
	void *table;
	size_t esize;
	size_t log_len;
	size_t alloc_len;
	int (*cmp) (const void *, const void *);
	unsigned long (* hash) (void *, size_t);
	int (*check) (void *, int, size_t);
	void (*cp) (void *, void *);
	/*stack log;*/
} ohash_tab;

void 
oht_init(ohash_tab *oht, size_t esize, size_t init_alloc, 
		size_t (*hash) (void *, size_t), 
		int (*cmp) (const void *, const void *),
		int (*check) (void *, int, size_t),
		void (*cp) (void *, void *));

void oht_dispose(ohash_tab *oht);
void *oht_lookup(ohash_tab *oht, void *elem_addr, int create);
void oht_delete(ohash_tab *oht, void *elem_addr);

/****************************** hash end **************************************/

/****************************** page start **************************************/
/* this struct provides:
 * a) page sized storage, which means I can use address in "user space",
 * forget about that slot design 
 *
 * b) slots struct ability to record free slots;
 */

#define ERR_ALLOC	-1
#define ERR_ARG		-2
typedef struct {
	size_t log_len;

	unsigned long elems_per_page;
	size_t esize;

	size_t page_size;
	size_t n_pages;
	void *curr_page;

	stack pages;	//array that for store page addresses;
	stack frags;
	void (* cp)(void *, const void *);
} page_strt;

int err_pstrt_init(page_strt *p, size_t esize, size_t page_size,
		void (* cp) (void *, const void *));


void pstrt_dispose(page_strt *p);
int pstrt_insert(page_strt *p,  const void *elemAddr, void **ret_addr);
void pstrt_delete(page_strt *p, void *ret_addr);
/******************************* page end **************************************/
#endif /*misc_types.h*/
