
CC = gcc
CFLAGS = -g -Wall 
YYAC = bison
LEX = flex

LEXS = snl.lex
LEXC = snl-lex.c

YYSRC = snl.y
YYC   = snl-yy.c
CSRC = parsertest.c ast_tree.c dumptype.c snl_stringtab.c stringtab.c ${LEXC} ${YYC}
OBJS = ${CSRC:.c=.o}
DEPEND = ${CC} -MM


parser : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@

${LEXC} : ${LEXS}
	${LEX} -o $@ ${LEXS}

${YYC} : ${YYSRC}
	${YYAC} -o $@ ${YYSRC}

%.d: %.c
	${DEPEND} $< | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@

-include ${CSRC:.c=.d}

.PHONY : clean
clean : 
	-rm -f *.o *~ parser
