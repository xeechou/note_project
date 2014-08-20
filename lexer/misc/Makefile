CC = gcc
CFLAGS = -Wall -g
AR  = ar

CSRC = stack.c slots.c smalloc.c hash_array.c
OBJS = ${CSRC:.c=.o}

DEPEND = ${CC} -MM
LIBMISC = libmisctypes.a


${LIBMISC} : ${OBJS}
	${AR} rcs $@ $^

%.d: %.c
	${DEPEND} $< | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@

-include ${CSRC:.c=.d}

.PHONY: clean
clean :
	-rm -f *.o *~ 
