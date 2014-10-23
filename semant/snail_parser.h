/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_SNAIL_YY_SNAIL_PARSER_H_INCLUDED
# define YY_SNAIL_YY_SNAIL_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int snail_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOPIC = 201,
    KPT = 202,
    AKA = 203,
    EXTERN = 204,
    FUNC = 205,
    PROC = 206,
    WHAT = 251,
    WHEN = 252,
    HOW = 253,
    WHY = 254,
    WHERE = 255,
    WHO = 256,
    CASE = 301,
    OF = 302,
    ESAC = 303,
    ENUM = 304,
    MUNE = 305,
    LET = 306,
    IN = 307,
    TEL = 308,
    IF = 310,
    THEN = 311,
    ELSE = 312,
    FI = 313,
    WHILE = 314,
    RETURN = 309,
    CONNECT = 351,
    WITH = 352,
    TO = 353,
    FROM = 354,
    ABOUT = 355,
    STR_CONST = 150,
    OBJECTID = 151,
    ASSIGN_E = 152,
    ASSIGN_A = 153,
    ERROR = 100
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 99 "snail.y" /* yacc.c:1909  */

	ast_node *node_;
	node_data *data_;
	symbol* sym;
	char *err_msg;

#line 100 "snail_parser.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE snail_yylval;

int snail_yyparse (void);

#endif /* !YY_SNAIL_YY_SNAIL_PARSER_H_INCLUDED  */
