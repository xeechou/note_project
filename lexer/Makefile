CC = gcc
LSRC = snl.lex
LEXC = snl-lex.c
FFLAGS = -o ${LEXC}
LEX = flex  ${FFLAGS}

OBJS = lexertest.c  snl-lex.c stringtab.c stringtab.h snl.h snl_list.h
INCLUDE =  -I.
CFLAGS = -Wall -ggdb3 -finline-functions ${INCLUDE}


lexer : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o lexer

${LEXC} : ${LSRC}
	${LEX} ${LSRC}

.PHONY : clean
clean : 
	-rm -f *.o *~ lexer
