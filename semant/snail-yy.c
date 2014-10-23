/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         snail_yyparse
#define yylex           snail_yylex
#define yyerror         snail_yyerror
#define yydebug         snail_yydebug
#define yynerrs         snail_yynerrs

#define yylval          snail_yylval
#define yychar          snail_yychar

/* Copy the first part of user declarations.  */
#line 1 "snail.y" /* yacc.c:339  */

#include <stdio.h>
/* I don't know what's the point, since we use the curr_lineno provided by
 * yylex(), and set every node location by hand, we just omit it
 */
#define YYLTYPE int
#define yylloc curr_lineno
#define YYLLOC_DEFAULT(Current, Rhs, N)



#define YY_KEYWORDS
#include "snail.h"
#include "snl_const.h"
void gen_ast(SNAILState *s);
void prepare_lexer(SNAILState *s);
void yyerror(char *s);

/****************** extern datas *********************/
extern int   yylex();
extern int   curr_lineno;
static char *curr_filename;
//extern int   in_function;

extern symbol *what;
extern symbol *when;
extern symbol *where;
extern symbol *why;
extern symbol *who;
extern symbol *how;
extern symbol *self;	/* current kpt, function... */
extern symbol *local;	/* current topic, however we dont restrict user not to
			 * use local, local is statical allocated. */



/****************** static datas *********************/
static ast_node **ast_root;
static node_data *tmp_data;

static symbol_tab *id_table;
static symbol_tab *str_table;
static symbol_tab *str_psr_tab;

static int tmp_num;
static page_strt *nodes_data;
static page_strt *ast_nodes;

/* functions */
static inline ast_node *nil_node(void);
static inline ast_node *single_node(ast_node *n);
static inline ast_node *append_node(ast_node *to, ast_node *from);
static inline node_data *nil_data(void);
static inline node_data *single_data(node_data *n);
static inline node_data *append_data(node_data *to, node_data *from);

static inline ast_node *ast_syntax(char *start, char *end, int type, 
			node_data *n, unsigned int n_nodes)
{
	return syntax_line(start, end, type,
			   n, n_nodes, ast_nodes);
}
static inline node_data *anode(ast_node *n) 	{ return data_node(n, nodes_data); }
static inline node_data *inode(int i)	 	{ return data_int (i, nodes_data); }
static inline node_data *snode(symbol *s)	{ return data_sym (s, nodes_data); }

static inline ast_node 	*gen_topic(symbol *name, ast_node *features);
static inline ast_node 	*kpt_one_line(node_data *kpt_prefix, symbol *str);
static inline ast_node 	*gen_kpt(node_data *kpt_prefix, ast_node *attrs);
static inline node_data	*gen_kpt_simple(symbol *name, symbol *aka_name,
					ast_node *subsets);
static inline node_data *gen_args(node_data *args);
static inline ast_node 	*gen_func(symbol *name, node_data *args, 
				ast_node *stats);
static inline ast_node 	*gen_attr(symbol *name, ast_node *expr, int mode);
static inline node_data *gen_ids(node_data *ids);
static inline ast_node 	*gen_let(node_data *ids, ast_node *expr);
static inline ast_node 	*gen_case(symbol *name, ast_node *stats);
static inline ast_node 	*gen_case_stat(ast_node *id, ast_node *expr);
static inline ast_node 	*gen_label_stat(ast_node *navig, 
				ast_node *expr, int mode);
static inline ast_node 	*gen_conn(ast_node *navig,
				ast_node *expr, int mode);
static inline ast_node 	*gen_dispatch(symbol *topic, symbol *name, 
				ast_node *elist);
static inline ast_node 	*gen_oper(ast_node *ep1, ast_node *ep2, int mode);
static inline ast_node 	*gen_str(symbol *str);
static inline ast_node 	*gen_navig(symbol *topic, symbol *kpt, symbol *attr);
static inline ast_node 	*gen_proc(symbol *name, ast_node *procc);
static inline ast_node 	*gen_step(int mode, ast_node *step1,
				ast_node *step2, ast_node *step3);

/* yyerror messages */
char *topic_infunc = "topic is not allowed to access in function.";
/* yyerror messages */

#line 171 "snail-yy.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "snail_parser.h".  */
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
#line 99 "snail.y" /* yacc.c:355  */

	ast_node *node_;
	node_data *data_;
	symbol* sym;
	char *err_msg;

#line 257 "snail-yy.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE snail_yylval;

int snail_yyparse (void);

#endif /* !YY_SNAIL_YY_SNAIL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 272 "snail-yy.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   190

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  163

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   357

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    41,     2,
      47,    48,     2,     2,    46,    39,    50,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    45,    44,
       2,     2,     2,     2,    49,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      38,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,    40,    43,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    35,    36,    37,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     4,     5,     6,     7,     8,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     9,    10,    11,    12,    13,    14,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    15,    16,    17,    18,    19,    20,    21,    22,    28,
      23,    24,    25,    26,    27,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    29,    30,    31,    32,    33,     1,     2
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   149,   149,   156,   157,   164,   170,   171,   175,   177,
     179,   189,   192,   198,   200,   202,   204,   208,   209,   211,
     218,   222,   223,   231,   233,   235,   240,   241,   242,   243,
     244,   245,   246,   251,   252,   256,   261,   265,   267,   271,
     272,   275,   282,   283,   287,   288,   289,   294,   295,   301,
     305,   309,   318,   320,   327,   328,   329,   332,   336,   337,
     341,   342,   350,   351,   352,   353,   357,   361,   363,   372,
     374,   376,   378,   383,   385
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOPIC", "KPT", "AKA", "EXTERN", "FUNC",
  "PROC", "WHAT", "WHEN", "HOW", "WHY", "WHERE", "WHO", "CASE", "OF",
  "ESAC", "ENUM", "MUNE", "LET", "IN", "TEL", "IF", "THEN", "ELSE", "FI",
  "WHILE", "RETURN", "CONNECT", "WITH", "TO", "FROM", "ABOUT", "STR_CONST",
  "OBJECTID", "ASSIGN_E", "ASSIGN_A", "ERROR", "'-'", "'|'", "'&'", "'{'",
  "'}'", "';'", "':'", "','", "'('", "')'", "'@'", "'.'", "$accept",
  "program", "topics", "topic", "feature_list", "feature", "kpt",
  "kpt_simple", "id_list", "func", "attrs", "attr", "attr_name",
  "statements", "statement", "case_stats", "case_stat", "labeled_stat",
  "expr_list", "expr", "block_const", "const_list", "const", "navig",
  "proc", "proc_components", "proc_component", "string", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   356,   357,   201,   202,   203,   204,   205,   206,   251,
     252,   253,   254,   255,   256,   301,   302,   303,   304,   305,
     306,   307,   308,   310,   311,   312,   313,   314,   309,   351,
     352,   353,   354,   355,   150,   151,   152,   153,   100,    45,
     124,    38,   123,   125,    59,    58,    44,    40,    41,    64,
      46
};
# endif

#define YYPACT_NINF -67

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-67)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      51,   -15,    81,    51,   -67,    27,   -67,   -67,   115,    55,
      58,    83,     8,   -67,   -67,    -2,   -67,   -67,    12,    53,
      96,    97,   -67,   105,    98,   107,    69,   108,    10,   -67,
     -67,   -23,    61,   138,    35,    63,    42,    66,   -67,   -67,
     -67,   112,   -67,    -6,    10,    10,    65,   101,   112,   -67,
     -67,   -67,   -67,   -67,   -67,   -67,   -67,   -67,    17,   103,
      91,    69,   113,   113,   113,    69,   114,    61,    80,    82,
     106,    69,    69,    69,   116,   110,   129,   130,   -67,   117,
     -67,   118,   119,   -67,    69,    69,    66,   122,   123,   124,
       9,    66,   -28,   -67,   125,   -67,   120,   -67,    94,   -67,
     -67,   -67,    74,    10,    10,   -67,   -67,   -67,    66,    66,
      69,    69,    69,    69,   -67,    69,    61,   109,   -67,    61,
     108,    69,    21,   121,   -67,    95,   111,   -67,    66,    66,
      66,    66,    78,   -67,   142,    -8,    66,   -67,   126,   -67,
      69,    69,    10,   -67,   -67,   113,    69,   -67,    66,    66,
     140,    -3,   127,   128,    39,   -67,   -67,   131,   -67,    69,
     -67,   -67,    66
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     3,     0,     1,     4,     0,     0,
       0,     0,     0,     6,     8,     0,    10,     9,    13,     0,
       0,     0,     7,     0,     0,     0,     0,    17,     0,     5,
      73,     0,     0,    14,     0,    62,     0,    15,    48,    47,
      61,    60,    18,     0,     0,     0,     0,     0,    69,    74,
      11,    26,    27,    28,    29,    30,    31,    32,     0,     0,
      23,     0,     0,     0,     0,    44,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,     0,    21,     0,     0,    16,     0,     0,     0,
       0,    45,    64,    63,     0,    57,     0,    58,    56,    54,
      55,    19,     0,     0,     0,    68,    12,    22,    24,    25,
       0,     0,     0,     0,    52,    44,     0,    64,    59,     0,
      17,     0,     0,     0,    35,     0,     0,    72,    49,    50,
      51,    46,     0,    65,     0,     0,    38,    20,     0,    33,
       0,     0,     0,    70,    53,     0,     0,    34,    42,    43,
       0,     0,     0,     0,     0,    71,    37,     0,    39,     0,
      36,    40,    41
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -67,   -67,   -67,   164,   -67,   156,   -67,   -67,    49,   -67,
     -67,   132,   -66,   -67,    50,   -67,    23,   -67,    62,   -26,
     -67,   -67,   -30,   -54,   -67,   -67,   -41,   -21
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,    12,    13,    14,    15,    43,    16,
      58,    59,    60,   122,   123,   151,   152,   124,    90,    91,
      38,    69,    39,    40,    17,    46,    47,    41
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      37,    93,    31,    76,    77,    79,    70,    48,    87,    88,
      89,    49,     9,   146,   156,    10,    11,    25,    26,   115,
       5,    50,   116,    48,    48,    48,    51,    52,    53,    54,
      55,    56,    68,    44,    23,    86,   119,    45,    74,    96,
      74,   120,    75,    24,    30,    98,    99,   100,   125,   121,
     133,    21,    57,   134,     1,   113,    68,   114,   108,   109,
      81,   160,   126,   127,   137,    62,    63,    64,   125,     8,
      51,    52,    53,    54,    55,    56,    30,    68,    71,    72,
      73,     6,    48,    48,   128,   129,   130,   131,    44,   119,
      18,   153,    45,    19,   120,   136,    57,   153,    34,    30,
      27,   150,   121,    30,    35,    71,    72,    73,    78,    68,
      65,    36,    66,    67,   148,   149,    30,    68,    20,     9,
     154,    48,    10,    11,   113,    95,   144,    84,    85,    94,
      67,   140,   141,   162,    72,    73,   142,   143,    28,    30,
      32,    29,    33,    42,    61,    80,    49,    83,    68,    92,
      97,   101,   102,   103,   104,   110,   111,   112,   145,   116,
     117,   105,   106,   107,   118,   139,   155,     7,    22,   135,
     147,   158,   138,   159,   157,   161,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82
};

static const yytype_int16 yycheck[] =
{
      26,    67,    23,    44,    45,    46,    36,    28,    62,    63,
      64,    34,     4,    21,    17,     7,     8,     5,     6,    47,
      35,    44,    50,    44,    45,    46,     9,    10,    11,    12,
      13,    14,    35,    23,    36,    61,    15,    27,    46,    69,
      46,    20,    48,    45,    34,    71,    72,    73,   102,    28,
     116,    43,    35,   119,     3,    46,    35,    48,    84,    85,
      43,    22,   103,   104,    43,    30,    31,    32,   122,    42,
       9,    10,    11,    12,    13,    14,    34,    35,    39,    40,
      41,     0,   103,   104,   110,   111,   112,   113,    23,    15,
      35,   145,    27,    35,    20,   121,    35,   151,    29,    34,
      47,   142,    28,    34,    35,    39,    40,    41,    43,    35,
      47,    42,    49,    50,   140,   141,    34,    35,    35,     4,
     146,   142,     7,     8,    46,    43,    48,    36,    37,    49,
      50,    36,    37,   159,    40,    41,    25,    26,    42,    34,
      42,    44,    35,    35,     6,    44,    34,    44,    35,    35,
      44,    35,    42,    24,    24,    33,    33,    33,    16,    50,
      35,    44,    44,    44,    44,    44,    26,     3,    12,   120,
      44,    44,   122,    45,   151,    44,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    52,    53,    54,    35,     0,    54,    42,     4,
       7,     8,    55,    56,    57,    58,    60,    75,    35,    35,
      35,    43,    56,    36,    45,     5,     6,    47,    42,    44,
      34,    78,    42,    35,    29,    35,    42,    70,    71,    73,
      74,    78,    35,    59,    23,    27,    76,    77,    78,    34,
      44,     9,    10,    11,    12,    13,    14,    35,    61,    62,
      63,     6,    30,    31,    32,    47,    49,    50,    35,    72,
      73,    39,    40,    41,    46,    48,    77,    77,    43,    77,
      44,    43,    62,    44,    36,    37,    70,    74,    74,    74,
      69,    70,    35,    63,    49,    43,    73,    44,    70,    70,
      70,    35,    42,    24,    24,    44,    44,    44,    70,    70,
      33,    33,    33,    46,    48,    47,    50,    35,    44,    15,
      20,    28,    64,    65,    68,    74,    77,    77,    70,    70,
      70,    70,    69,    63,    63,    59,    70,    43,    65,    44,
      36,    37,    25,    26,    48,    16,    21,    44,    70,    70,
      77,    66,    67,    74,    70,    26,    17,    67,    44,    45,
      22,    44,    70
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    53,    53,    54,    55,    55,    56,    56,
      56,    57,    57,    58,    58,    58,    58,    59,    59,    59,
      60,    61,    61,    62,    62,    62,    63,    63,    63,    63,
      63,    63,    63,    64,    64,    65,    65,    65,    65,    66,
      66,    67,    68,    68,    69,    69,    69,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    71,    72,    72,
      73,    73,    74,    74,    74,    74,    75,    76,    76,    77,
      77,    77,    77,    78,    78
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     6,     1,     2,     1,     1,
       1,     4,     6,     2,     4,     4,     6,     0,     1,     3,
       8,     2,     3,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     2,     3,     1,     5,     5,     2,     2,
       3,     3,     3,     3,     0,     1,     3,     1,     1,     5,
       5,     5,     4,     6,     3,     3,     3,     3,     2,     3,
       1,     1,     1,     3,     3,     5,     5,     2,     3,     1,
       5,     7,     4,     1,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 149 "snail.y" /* yacc.c:1646  */
    {*ast_root = ast_syntax(
						start_prog, end_prog, 
						0, anode((yyvsp[0].node_)), N_PROG_ELEM);}
#line 1474 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 3:
#line 156 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = single_node((yyvsp[0].node_));	}
#line 1480 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 4:
#line 157 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = append_node((yyvsp[-1].node_), (yyvsp[0].node_));	}
#line 1486 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 5:
#line 166 "snail.y" /* yacc.c:1646  */
    {	(yyval.node_) = gen_topic((yyvsp[-4].sym), (yyvsp[-2].node_)); }
#line 1492 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 6:
#line 170 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = single_node((yyvsp[0].node_));		}
#line 1498 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 7:
#line 171 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = append_node((yyvsp[-1].node_), (yyvsp[0].node_));	}
#line 1504 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 8:
#line 175 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = (yyvsp[0].node_);		}
#line 1510 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 9:
#line 177 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = (yyvsp[0].node_);		}
#line 1516 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 10:
#line 179 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = (yyvsp[0].node_);		}
#line 1522 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 11:
#line 190 "snail.y" /* yacc.c:1646  */
    {	(yyval.node_) = kpt_one_line((yyvsp[-3].data_), (yyvsp[-1].sym));	}
#line 1528 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 12:
#line 193 "snail.y" /* yacc.c:1646  */
    {	(yyval.node_) = gen_kpt((yyvsp[-5].data_), (yyvsp[-2].node_));		}
#line 1534 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 13:
#line 199 "snail.y" /* yacc.c:1646  */
    {(yyval.data_) = gen_kpt_simple((yyvsp[0].sym), NULL, NULL);	}
#line 1540 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 14:
#line 201 "snail.y" /* yacc.c:1646  */
    {(yyval.data_) = gen_kpt_simple((yyvsp[-2].sym), (yyvsp[0].sym), NULL);	}
#line 1546 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 15:
#line 203 "snail.y" /* yacc.c:1646  */
    {(yyval.data_) = gen_kpt_simple((yyvsp[-2].sym), NULL, (yyvsp[0].node_));	}
#line 1552 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 16:
#line 205 "snail.y" /* yacc.c:1646  */
    {(yyval.data_) = gen_kpt_simple((yyvsp[-4].sym), (yyvsp[-2].sym), (yyvsp[0].node_));	}
#line 1558 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 17:
#line 208 "snail.y" /* yacc.c:1646  */
    {(yyval.data_) = NULL;		}
#line 1564 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 18:
#line 209 "snail.y" /* yacc.c:1646  */
    {(yyval.data_) = single_data(snode((yyvsp[0].sym)));
						tmp_num = 1;}
#line 1571 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 19:
#line 211 "snail.y" /* yacc.c:1646  */
    {(yyval.data_) = append_data((yyvsp[-2].data_), snode((yyvsp[0].sym)));
						tmp_num++;	}
#line 1578 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 20:
#line 219 "snail.y" /* yacc.c:1646  */
    {	(yyval.node_) = gen_func((yyvsp[-6].sym), (yyvsp[-4].data_), (yyvsp[-1].node_));	}
#line 1584 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 21:
#line 222 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = single_node((yyvsp[-1].node_));		}
#line 1590 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 22:
#line 223 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = append_node((yyvsp[-2].node_), (yyvsp[-1].node_));	}
#line 1596 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 23:
#line 231 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_attr((yyvsp[0].sym), NULL, 0);	}
#line 1602 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 24:
#line 233 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_attr((yyvsp[-2].sym), (yyvsp[0].node_), '=');	}
#line 1608 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 25:
#line 235 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_attr((yyvsp[-2].sym), (yyvsp[0].node_), '+');	}
#line 1614 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 26:
#line 240 "snail.y" /* yacc.c:1646  */
    {(yyval.sym) = what;	}
#line 1620 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 27:
#line 241 "snail.y" /* yacc.c:1646  */
    {(yyval.sym) = when;	}
#line 1626 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 28:
#line 242 "snail.y" /* yacc.c:1646  */
    {(yyval.sym) = how;	}
#line 1632 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 29:
#line 243 "snail.y" /* yacc.c:1646  */
    {(yyval.sym) = why;	}
#line 1638 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 30:
#line 244 "snail.y" /* yacc.c:1646  */
    {(yyval.sym) = where;	}
#line 1644 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 31:
#line 245 "snail.y" /* yacc.c:1646  */
    {(yyval.sym) = who;	}
#line 1650 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 32:
#line 246 "snail.y" /* yacc.c:1646  */
    {(yyval.sym) = (yyvsp[0].sym);	}
#line 1656 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 33:
#line 251 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = single_node((yyvsp[-1].node_));		}
#line 1662 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 34:
#line 252 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = append_node((yyvsp[-2].node_), (yyvsp[-1].node_));	}
#line 1668 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 35:
#line 256 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = (yyvsp[0].node_);	}
#line 1674 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 36:
#line 261 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_let((yyvsp[-3].data_), (yyvsp[-1].node_));	}
#line 1680 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 37:
#line 266 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_case((yyvsp[-3].sym), (yyvsp[-1].node_));}
#line 1686 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 38:
#line 267 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = (yyvsp[0].node_);	}
#line 1692 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 39:
#line 271 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = single_node((yyvsp[-1].node_));		}
#line 1698 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 40:
#line 272 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = append_node((yyvsp[-2].node_), (yyvsp[-1].node_));	}
#line 1704 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 41:
#line 275 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_case_stat((yyvsp[-2].node_), (yyvsp[0].node_));	}
#line 1710 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 42:
#line 282 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_label_stat((yyvsp[-2].node_), (yyvsp[0].node_), '=');}
#line 1716 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 43:
#line 283 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_label_stat((yyvsp[-2].node_), (yyvsp[0].node_), '+');}
#line 1722 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 44:
#line 287 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = nil_node();		}
#line 1728 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 45:
#line 288 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = single_node((yyvsp[0].node_));		}
#line 1734 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 46:
#line 289 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = append_node((yyvsp[-2].node_), (yyvsp[0].node_));	}
#line 1740 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 47:
#line 294 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = single_node((yyvsp[0].node_));	}
#line 1746 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 48:
#line 295 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = (yyvsp[0].node_);}
#line 1752 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 49:
#line 302 "snail.y" /* yacc.c:1646  */
    {	(yyval.node_) = single_node(
			gen_conn((yyvsp[-2].node_), (yyvsp[0].node_), WITH_CONN));				}
#line 1759 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 50:
#line 306 "snail.y" /* yacc.c:1646  */
    {	(yyval.node_) = single_node(
			gen_conn((yyvsp[-2].node_), (yyvsp[0].node_), TO_CONN));				}
#line 1766 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 51:
#line 310 "snail.y" /* yacc.c:1646  */
    {	(yyval.node_) = single_node(
			gen_conn((yyvsp[-2].node_), (yyvsp[0].node_), FROM_CONN));				}
#line 1773 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 52:
#line 319 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_dispatch (local, (yyvsp[-3].sym), (yyvsp[-1].node_));	}
#line 1779 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 53:
#line 321 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_dispatch ((yyvsp[-5].sym), (yyvsp[-3].sym), (yyvsp[-1].node_));	}
#line 1785 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 54:
#line 327 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_oper((yyvsp[-2].node_), (yyvsp[0].node_), '|');	}
#line 1791 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 55:
#line 328 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_oper((yyvsp[-2].node_), (yyvsp[0].node_), '&');	}
#line 1797 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 56:
#line 329 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_oper((yyvsp[-2].node_), (yyvsp[0].node_), '-');	}
#line 1803 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 57:
#line 332 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = (yyvsp[-1].node_);			}
#line 1809 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 58:
#line 336 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = single_node((yyvsp[-1].node_));		}
#line 1815 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 59:
#line 337 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = append_node((yyvsp[-2].node_), (yyvsp[-1].node_));	}
#line 1821 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 60:
#line 341 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_str((yyvsp[0].sym));	}
#line 1827 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 61:
#line 342 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = (yyvsp[0].node_);		}
#line 1833 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 62:
#line 350 "snail.y" /* yacc.c:1646  */
    { (yyval.node_) = gen_navig(local, (yyvsp[0].sym), NULL);}
#line 1839 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 63:
#line 351 "snail.y" /* yacc.c:1646  */
    { (yyval.node_) = gen_navig(local, (yyvsp[-2].sym), (yyvsp[0].sym));}
#line 1845 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 64:
#line 352 "snail.y" /* yacc.c:1646  */
    { (yyval.node_) = gen_navig((yyvsp[-2].sym), (yyvsp[0].sym), NULL);	}
#line 1851 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 65:
#line 354 "snail.y" /* yacc.c:1646  */
    { (yyval.node_) = gen_navig((yyvsp[-4].sym), (yyvsp[-2].sym), (yyvsp[0].sym));	}
#line 1857 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 66:
#line 358 "snail.y" /* yacc.c:1646  */
    {	(yyval.node_) = gen_proc((yyvsp[-3].sym), (yyvsp[-1].node_));		}
#line 1863 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 67:
#line 361 "snail.y" /* yacc.c:1646  */
    {single_node((yyvsp[-1].node_)); 	}
#line 1869 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 68:
#line 363 "snail.y" /* yacc.c:1646  */
    {append_node((yyvsp[-2].node_), (yyvsp[-1].node_));	}
#line 1875 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 69:
#line 373 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_step(PC_STR, gen_str((yyvsp[0].sym)), NULL, NULL); 	}
#line 1881 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 70:
#line 375 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_step(PC_IF, (yyvsp[-3].node_), (yyvsp[-1].node_), NULL);			}
#line 1887 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 71:
#line 377 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_step(PC_IF_ELSE, (yyvsp[-5].node_), (yyvsp[-3].node_), (yyvsp[-1].node_));			}
#line 1893 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 72:
#line 379 "snail.y" /* yacc.c:1646  */
    {(yyval.node_) = gen_step(PC_LOOP, (yyvsp[-2].node_), (yyvsp[0].node_), NULL);			}
#line 1899 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 73:
#line 384 "snail.y" /* yacc.c:1646  */
    {(yyval.sym) = single_string(str_psr_tab, (yyvsp[0].sym)); 				}
#line 1905 "snail-yy.c" /* yacc.c:1646  */
    break;

  case 74:
#line 386 "snail.y" /* yacc.c:1646  */
    {(yyval.sym) = append_string(str_psr_tab, (yyvsp[-1].sym), (yyvsp[0].sym));			}
#line 1911 "snail-yy.c" /* yacc.c:1646  */
    break;


#line 1915 "snail-yy.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 388 "snail.y" /* yacc.c:1906  */


static inline ast_node *nil_node(void)
{
	return NULL;
}
static inline ast_node *single_node(ast_node *n)
{
	n->prev = n->next = n;
	return n;
}
static inline ast_node *append_node(ast_node *to, ast_node *from)
{
	ast_node *prev = to->prev;
	from->prev = prev;
	from->next = to;
	prev->next = from;
	to->prev = from;

	return to;
}
static inline node_data *nil_data(void)
{	return NULL;	}
static inline node_data *single_data(node_data *d)
{
	d->prev = d->next = d;
	return d;
}
static inline node_data *append_data(node_data *to, node_data *from)
{
	node_data *prev = to->prev;
	from->prev = prev;
	from->next = to;
	prev->next = from;
	to->prev = from;

	return to;
}
/*********************** wrap functions ***********************/
/* to keep the simplicity of bison code itself, the wrap function is avoidable
 * At least we kept a clean ast_node structure :) */
static inline ast_node *gen_topic(symbol *name, ast_node *features)
{ 
	tmp_data = compose_node_data(N_TOPIC_ELEM, 
			snode(name), anode(features));

	return ast_syntax(start_topic, end_topic, TP_TOPIC,
	   		 tmp_data, N_TOPIC_ELEM); 
}
static inline ast_node *kpt_one_line(node_data *kpt_prefix, symbol *str)
{
	//build attr first
	ast_node *a = gen_str(str);
	ast_node *b = gen_attr(what, a, '=');

	tmp_data = append_node_data(1, kpt_prefix, anode(b));

	return  ast_syntax(start_kpt, end_kpt, TP_KPT,
			tmp_data, N_KPT_ELEM);
}

static inline ast_node *gen_kpt(node_data *kpt_prefix, ast_node *attrs)
{
	node_data *d = append_node_data(1, kpt_prefix, anode(attrs));	
	ast_node *a = ast_syntax(start_kpt, end_kpt, TP_KPT,
                        	d, N_KPT_ELEM);
	return a;
}
static inline node_data	*gen_kpt_simple(symbol *name, symbol *aka_name,
					ast_node *subsets) 
{
	return compose_node_data(3, snode(name), snode(aka_name),
				anode(subsets));
}

/* adding args is exceptional, coz it is unkown in the first place */
static inline node_data *gen_args(node_data *args) 
{
	ast_node *a = ast_syntax(start_arg, end_arg, TP_ID,
				args, tmp_num);
	tmp_num = 0;
	return anode(a);
}

static inline ast_node *gen_func(symbol *name, node_data *args, ast_node *stats)
{
	node_data *d = compose_node_data(N_FUNC_ELEM, snode(name), 
					gen_args(args), anode(stats));
	return ast_syntax(start_func, end_func, TP_FUNC, 
				d, N_FUNC_ELEM);
}

static inline ast_node *gen_attr(symbol *name, ast_node *expr, int mode)
{
	node_data *d = compose_node_data(N_ATTR_ELEM, snode(name),
			anode(expr), inode(mode));
	return ast_syntax(start_attr, end_attr, TP_ATTR,
				d, N_ATTR_ELEM); 
}

static inline node_data *gen_ids(node_data *ids)
{
	ast_node *a = ast_syntax(start_id, end_id, TP_ID,
				ids, tmp_num);
	tmp_num = 0;
	return anode(a);
}

static inline ast_node *gen_let(node_data *ids, ast_node *expr)
{
	node_data *d = compose_node_data(N_LET_ELEM, gen_ids(ids),
					anode(expr));

	return ast_syntax(start_let, end_let, TP_NONE, d, N_LET_ELEM);
}

static inline ast_node *gen_case(symbol *name, ast_node *stats)
{
	node_data *d = compose_node_data(N_CASE_ELEM, snode(name),
				anode(stats));

	return ast_syntax(start_case, end_case, TP_NONE, d, N_CASE_ELEM);
}

static inline ast_node *gen_case_stat(ast_node *id, ast_node *expr)
{
	node_data *d = compose_node_data(2, anode(id), anode(expr));

	return ast_syntax(start_case_stat, end_case_stat, TP_NONE, 
			d, 2);
}
static inline ast_node *gen_label_stat(ast_node *navig, 
				ast_node *expr, int mode)
{
	node_data *d = compose_node_data(N_LAB_ELEM, anode(navig),
			anode(expr), inode(mode));
	return ast_syntax(start_label_stat, end_label_stat, TP_NONE,
				d, N_LAB_ELEM); 
}

static inline ast_node *gen_conn(ast_node *navig,
				ast_node *expr, int mode)
{
	node_data *d = compose_node_data(N_CON_ELEM, anode(navig),
			anode(expr), inode(mode));
	return ast_syntax(start_conn, end_conn, TP_CONN,
				d, N_CON_ELEM); 
}
static inline ast_node *gen_dispatch(symbol *topic, symbol *name, 
				ast_node *elist)
{
	node_data *d = compose_node_data(N_DIS_ELEM, snode(topic), 
			snode(name), anode(elist));
	return ast_syntax(start_dispatch, end_dispatch, TP_NONE,
				d, N_DIS_ELEM); 
}

static inline ast_node *gen_oper(ast_node *ep1, ast_node *ep2, int mode)
{
	node_data *d = compose_node_data(N_OPER_ELEM, anode(ep1),
			anode(ep2), inode(mode));
	return ast_syntax(start_oper, end_oper, TP_NONE,
				d, N_OPER_ELEM);
}

static inline ast_node *gen_str(symbol *str)
{
	return ast_syntax(start_str, end_str, TP_STRING, snode(str), 1);
}
/* checking should be done in typing steps */
static inline ast_node *gen_navig(symbol *topic, symbol *kpt, symbol *attr)
{
	node_data *d = compose_node_data(N_NAVIG_ELEM, snode(topic), 
					snode(kpt), snode(attr));

	return ast_syntax(start_navig, end_navig, TP_ID, 
				d, N_NAVIG_ELEM);
}

static inline ast_node *gen_proc(symbol *name, ast_node *procc)
{
	node_data *d = compose_node_data(N_PROC_ELEM, snode(name), procc);
	return ast_syntax(start_proc, end_proc, TP_PROC, d, 2);
}

static inline ast_node *gen_step(int mode, ast_node *step1,
				ast_node *step2, ast_node *step3)
{
	node_data *d = compose_node_data(N_STEP_ELEM, anode(step1), 
					anode(step2), anode(step3));

	return ast_syntax(start_procc, end_procc, TP_PROC, 
				d, N_STEP_ELEM);
}

void yyerror(char *s)
{
	fprintf(stderr, "\"%s\", %d : %s at or near %c", curr_filename, 
							curr_lineno,
							s,
							yychar);
// yychar contains the lookahead token or YYEOF, YYEMPTY
}

void gen_ast(SNAILState *s)
{
	prepare_lexer(s);
	curr_filename = s->curr_filename;
	curr_lineno = 0;

	id_table = &s->id_table;
	str_table = &s->str_table;
	str_psr_tab = &s->str_psr_tab;

	nodes_data = &s->nodes_data;
	ast_nodes  = &s->ast_nodes;
	ast_root = &s->ast_root;
	snail_yyparse();
}
