CC = gcc
CFLAGS = -Wall -ggdb3
LEX = flex

<<<<<<< HEAD
OBJS = lexertest.c  snl-lex.c stringtab.c stringtab.h snl.h snl_list.h
INCLUDE =  -I.
CFLAGS = -Wall -ggdb3 -finline-functions ${INCLUDE}
=======
LSRC = snail.lex
LEXC = snail_lex.c
>>>>>>> parser

CSRC = lexertest.c  snail_tab.c ${LEXC}
OBJS = ${CSRC:.c=.o}

DEPEND = ${CC} -MM

#subsystem
SLIBS = misctypes
SLIBOPT = -L./misc -l${SLIBS}
MAKE = make

lexer : ${OBJS} ${SLIBS}
	${CC} ${CFLAGS} ${OBJS} ${SLIBOPT} -o $@


${SLIBS}:
	cd misc/ && ${MAKE}

${LEXC} : ${LSRC}
	${LEX} -o $@ ${LSRC}

%.d: %.c
	${CC} ${CFLAGS} -c $<
	${DEPEND} $< | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@

-include ${CSRC:.c=.d}

.PHONY : clean
clean : 
	-rm -f *.o *.d *~ lexer
