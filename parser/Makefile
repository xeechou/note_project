
CC = gcc
CFLAGS = -ggdb3 -Wall 
YYAC = bison
LEX = flex

LEXS = snail.lex
LEXC = snail-lex.c
LEXH = snail_parser.h

YYSRC = snail.y
YYC   = snail-yy.c
CSRC = parsertest.c ast_tree.c dumptype.c snail_tab.c ${LEXC} ${YYC}
OBJS = ${CSRC:.c=.o}
DEPEND = ${CC} -MM

#subsystem
SLIBS = misctypes
SLIBOPT = -L./misc -l${SLIBS}
MAKE = make

parser : ${OBJS}
	${CC} ${OBJS} ${SLIBOPT} -o $@

${SLIBS}:
	cd misc/ && ${MAKE}

${LEXC} : ${LEXS}
	${LEX} -o $@ ${LEXS}

${YYC} ${LEXH}: ${YYSRC}
	${YYAC} -v --defines=${LEXH} --output=${YYC} ${YYSRC}

%.d: %.c
	${CC} ${CFLAGS} -c $<
	${DEPEND} $< | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@

-include ${CSRC:.c=.d}

.PHONY : clean
clean : 
	-rm -f *.o *.d *~ ${LEXC} ${YYC} ${LEXH} parser
