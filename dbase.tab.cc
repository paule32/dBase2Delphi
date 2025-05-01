/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "dbase.ypp"

// ---------------------------------------------------------------------------------------
// Copyright(c) 2025 Jens Kallup
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and /or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions :
// 
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// ---------------------------------------------------------------------------------------

#include <windows.h>
#include <objbase.h>    // CoCreateGuid, GUID

#include <stdio.h>      // classical functions
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>      // for atexit and exit
#include <cstdint>
#include <cctype>
#include <sstream>
#include <string>
#include <algorithm>
#include <memory>

#include <map>
#include <vector>

//-- PARSER INPUT / OUTPUT -------------------------------
extern int line_row;
extern int line_col;

extern void yyset_in(FILE*);
extern void yyset_out(FILE*);

extern FILE* yyin;
extern FILE* yyout;

int yylex(); 
int yyerror(const char *p) {
    printf("error at %d:%d: %s\n",
        line_row,
        line_col + 1,p);
    exit(1);
}

//-- FORWARD DECLARATIONS ---------------------------------
std::string repeat(const std::string& str, int n);
std::string* result = nullptr;

int scope_level = 0;
int indent = 1;
std::string get_indent() {
    return std::string(indent * 2, ' ');
}

// --------------------------------------------------------
// GENERATE GUID STRING FOR HASHING
// --------------------------------------------------------
std::string GenerateGUID(void)
{
    std::stringstream str;
    GUID guid;
    HRESULT hr = CoCreateGuid(&guid);
    
    if (SUCCEEDED(hr)) {
        str << std::setw(8) << std::setfill('0') << std::hex << static_cast< int >( guid.Data1 ) << "_"
            << std::setw(4) << std::setfill('0') << std::hex << static_cast< int >( guid.Data2 ) << "_"
            << std::setw(4) << std::setfill('0') << std::hex << static_cast< int >( guid.Data3 ) << "_"
            ;
            
        for (int idx = 0; idx < 8; idx++) {
            if (idx == 2)
            str << "_";
            str << std::setw(2)
                << std::setfill('0')
                << std::hex
                << static_cast<int>(guid.Data4[idx]);
        }
    }   else {
        str << "<empty>";
    }
    
    return str.str();
}

// --------------------------------------------------------
// PARSE with (this) ...
// --------------------------------------------------------
bool parseThis(const std::string& input, std::string& this_part) {
    std::string upper = input;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    if (upper == "THIS") {
        this_part = "self";
        return true;
    }   return false;
}

//-- EXPRESSION HANDLING ---------------------------------
std::vector<std::stringstream> term_stream(10);
int32_t term_count = 0;

static char buffer32_1[32];
static char buffer32_2[32];

void term_add(uint32_t num) {
    ltoa(num, buffer32_1, 10);
    term_stream[0] << buffer32_1;
}

//-- INTERNAL STRUCTURES ---------------------------------
enum class ParamType {
    PT_UNKNOWN = 0,
    PT_NUMERIC = 1,
    PT_STRING  = 2,
    PT_OBJECT  = 3
};
enum class ParamScope {
    PS_UNKNOWN = 0,
    PS_GLOBAL  = 1,
    PS_LOCAL   = 2,
    PS_PRIVATE = 3
};
struct DB_Parameter {
    ParamScope scope;  // global, local, private
    ParamType   type;
    //---------------
    int         size;
    std::string data;
};
std::map<std::string, std::vector< DB_Parameter >> parameters;

std::map<std::string, std::string> classes;
std::map<std::string, std::string> classes_guid;

std::vector<std::string> class_vector;

#line 226 "dbase.tab.cc"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "dbase.tab.hh"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_CMD_NUMBER = 3,                 /* CMD_NUMBER  */
  YYSYMBOL_CMD_ID = 4,                     /* CMD_ID  */
  YYSYMBOL_CMD_ASSIGN = 5,                 /* CMD_ASSIGN  */
  YYSYMBOL_CMD_IF = 6,                     /* CMD_IF  */
  YYSYMBOL_CMD_ELSE = 7,                   /* CMD_ELSE  */
  YYSYMBOL_CMD_ENDIF = 8,                  /* CMD_ENDIF  */
  YYSYMBOL_CMD_NE = 9,                     /* CMD_NE  */
  YYSYMBOL_CMD_EQ = 10,                    /* CMD_EQ  */
  YYSYMBOL_CMD_GT = 11,                    /* CMD_GT  */
  YYSYMBOL_CMD_LT = 12,                    /* CMD_LT  */
  YYSYMBOL_CMD_EG = 13,                    /* CMD_EG  */
  YYSYMBOL_CMD_EL = 14,                    /* CMD_EL  */
  YYSYMBOL_CMD_GE = 15,                    /* CMD_GE  */
  YYSYMBOL_CMD_LE = 16,                    /* CMD_LE  */
  YYSYMBOL_CMD_PARAMETER = 17,             /* CMD_PARAMETER  */
  YYSYMBOL_CMD_LOCAL = 18,                 /* CMD_LOCAL  */
  YYSYMBOL_CMD_PUBLIC = 19,                /* CMD_PUBLIC  */
  YYSYMBOL_CMD_PRIVATE = 20,               /* CMD_PRIVATE  */
  YYSYMBOL_CMD_PROTECTED = 21,             /* CMD_PROTECTED  */
  YYSYMBOL_CMD_SQSTRING = 22,              /* CMD_SQSTRING  */
  YYSYMBOL_CMD_DQSTRING = 23,              /* CMD_DQSTRING  */
  YYSYMBOL_CMD_BLOCKSTRING = 24,           /* CMD_BLOCKSTRING  */
  YYSYMBOL_CMD_NEW = 25,                   /* CMD_NEW  */
  YYSYMBOL_CMD_OF = 26,                    /* CMD_OF  */
  YYSYMBOL_CMD_FORM = 27,                  /* CMD_FORM  */
  YYSYMBOL_CMD_WITH = 28,                  /* CMD_WITH  */
  YYSYMBOL_CMD_ENDWITH = 29,               /* CMD_ENDWITH  */
  YYSYMBOL_CMD_CLASS = 30,                 /* CMD_CLASS  */
  YYSYMBOL_CMD_ENDCLASS = 31,              /* CMD_ENDCLASS  */
  YYSYMBOL_FILENAME = 32,                  /* FILENAME  */
  YYSYMBOL_33_ = 33,                       /* '+'  */
  YYSYMBOL_34_ = 34,                       /* '-'  */
  YYSYMBOL_35_ = 35,                       /* '*'  */
  YYSYMBOL_36_ = 36,                       /* '/'  */
  YYSYMBOL_UMINUS = 37,                    /* UMINUS  */
  YYSYMBOL_UPLUS = 38,                     /* UPLUS  */
  YYSYMBOL_39_ = 39,                       /* '('  */
  YYSYMBOL_40_ = 40,                       /* ')'  */
  YYSYMBOL_41_ = 41,                       /* '.'  */
  YYSYMBOL_42_ = 42,                       /* '['  */
  YYSYMBOL_43_ = 43,                       /* ']'  */
  YYSYMBOL_44_ = 44,                       /* ','  */
  YYSYMBOL_YYACCEPT = 45,                  /* $accept  */
  YYSYMBOL_program = 46,                   /* program  */
  YYSYMBOL_stmt = 47,                      /* stmt  */
  YYSYMBOL_class_list = 48,                /* class_list  */
  YYSYMBOL_stmt_list = 49,                 /* stmt_list  */
  YYSYMBOL_expr_string = 50,               /* expr_string  */
  YYSYMBOL_if_stmt = 51,                   /* if_stmt  */
  YYSYMBOL_else_part = 52,                 /* else_part  */
  YYSYMBOL_stmt_block = 53,                /* stmt_block  */
  YYSYMBOL_expr = 54,                      /* expr  */
  YYSYMBOL_string_literal = 55,            /* string_literal  */
  YYSYMBOL_value = 56,                     /* value  */
  YYSYMBOL_class_element = 57,             /* class_element  */
  YYSYMBOL_class_def = 58,                 /* class_def  */
  YYSYMBOL_59_1 = 59,                      /* $@1  */
  YYSYMBOL_form_optional = 60,             /* form_optional  */
  YYSYMBOL_class_list_opt = 61,            /* class_list_opt  */
  YYSYMBOL_with_statement = 62,            /* with_statement  */
  YYSYMBOL_63_2 = 63,                      /* $@2  */
  YYSYMBOL_64_3 = 64,                      /* $@3  */
  YYSYMBOL_with_block = 65,                /* with_block  */
  YYSYMBOL_with_element = 66,              /* with_element  */
  YYSYMBOL_qualified_access = 67,          /* qualified_access  */
  YYSYMBOL_68_4 = 68,                      /* $@4  */
  YYSYMBOL_access_tail = 69,               /* access_tail  */
  YYSYMBOL_expr_list = 70,                 /* expr_list  */
  YYSYMBOL_assignment = 71,                /* assignment  */
  YYSYMBOL_72_5 = 72                       /* $@5  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  24
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   178

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  112

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   289


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      39,    40,    35,    33,    44,    34,    41,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    42,     2,    43,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    37,    38
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   244,   244,   250,   251,   255,   256,   260,   265,   271,
     274,   277,   283,   306,   309,   333,   336,   342,   343,   344,
     346,   350,   355,   360,   365,   370,   375,   380,   385,   390,
     395,   400,   405,   410,   411,   415,   416,   420,   421,   422,
     438,   439,   443,   443,   481,   485,   491,   492,   496,   510,
     496,   517,   518,   522,   523,   524,   539,   539,   548,   553,
     557,   562,   567,   573,   577,   585,   585,   590
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "CMD_NUMBER", "CMD_ID",
  "CMD_ASSIGN", "CMD_IF", "CMD_ELSE", "CMD_ENDIF", "CMD_NE", "CMD_EQ",
  "CMD_GT", "CMD_LT", "CMD_EG", "CMD_EL", "CMD_GE", "CMD_LE",
  "CMD_PARAMETER", "CMD_LOCAL", "CMD_PUBLIC", "CMD_PRIVATE",
  "CMD_PROTECTED", "CMD_SQSTRING", "CMD_DQSTRING", "CMD_BLOCKSTRING",
  "CMD_NEW", "CMD_OF", "CMD_FORM", "CMD_WITH", "CMD_ENDWITH", "CMD_CLASS",
  "CMD_ENDCLASS", "FILENAME", "'+'", "'-'", "'*'", "'/'", "UMINUS",
  "UPLUS", "'('", "')'", "'.'", "'['", "']'", "','", "$accept", "program",
  "stmt", "class_list", "stmt_list", "expr_string", "if_stmt", "else_part",
  "stmt_block", "expr", "string_literal", "value", "class_element",
  "class_def", "$@1", "form_optional", "class_list_opt", "with_statement",
  "$@2", "$@3", "with_block", "with_element", "qualified_access", "$@4",
  "access_tail", "expr_list", "assignment", "$@5", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-71)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      23,    24,   -31,    10,    16,   -71,   -17,    23,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,    24,    24,    24,   114,
     -71,   -71,    20,    26,   -71,   -71,   -71,   -32,     7,     7,
     103,    24,    24,    24,    24,    24,    24,    24,    24,    24,
      24,    24,    24,    23,    42,    -9,    32,    22,    69,    24,
     -71,   -71,    61,    61,    61,    61,   142,   142,    61,    61,
      73,    73,     7,     7,    23,    52,   -71,    37,   -32,    56,
      48,   -32,    68,   -71,   -71,     2,    94,   -71,   -71,    24,
     -32,   -71,   -32,   105,   -71,     2,   -71,   -71,   -71,    59,
     -17,   -71,   -71,   -71,    83,    92,   -71,   117,   -71,   -17,
     100,   -71,   -71,   -71,   -71,   -71,     0,   -71,   -71,   -71,
     -71,   -71
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     8,     4,     2,     3,     6,
      40,    41,    17,    56,    35,    36,     0,     0,     0,    16,
      19,    18,     0,     0,     1,     5,     7,    62,    34,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    15,    14,     0,     0,     0,     0,     0,
      57,    20,    26,    25,    31,    32,    29,    30,    27,    28,
      21,    22,    23,    24,    16,     0,    48,    45,    62,    63,
       0,    62,     0,    13,    12,    55,     0,    42,    59,     0,
      62,    58,    62,    56,    53,    49,    52,    65,    54,     0,
      47,    64,    60,    61,     0,     0,    51,     0,    44,    46,
       0,     9,    10,    11,    67,    50,     0,    43,    38,    37,
      66,    39
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -71,   -71,     5,    43,   132,   -71,   -71,   -71,    70,     1,
      29,   -71,    -6,   -71,   -71,   -71,   -71,   -70,   -71,   -71,
     -71,    55,   -21,   -71,    91,    62,   -71,   -71
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,    43,   104,     8,    65,    44,    69,
      20,   110,     9,    10,    90,    77,   100,    11,    75,    95,
      85,    86,    21,    27,    50,    70,    88,    97
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      25,    45,    19,   108,    13,    84,    83,    47,    22,    48,
      49,     2,    26,     3,    23,    84,    24,    28,    29,    30,
      35,    36,    14,    15,    13,    12,    13,    12,    13,     1,
       2,    66,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    14,    15,    14,    15,    26,    64,
      72,     2,    46,     3,    87,    16,    17,    16,    17,    67,
      74,    18,    68,    18,    87,    31,    32,    33,    34,    35,
      36,    37,    38,    71,    35,    36,    76,    31,    32,    33,
      34,    35,    36,    37,    38,   111,    35,    36,    80,    39,
      40,    41,    42,    25,    39,    40,    41,    42,    89,    98,
      79,    39,    40,    41,    42,   101,   102,   103,    41,    42,
      94,    82,    31,    32,    33,    34,    35,    36,    37,    38,
       1,   105,   106,    31,    32,    33,    34,    35,    36,    37,
      38,   107,     7,    99,    73,   109,    39,    40,    41,    42,
      96,    91,     2,    51,     3,     0,     0,    39,    40,    41,
      42,    31,    32,    33,    34,    35,    36,    37,    38,    78,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,    93,     0,    39,    40,    41,    42
};

static const yytype_int8 yycheck[] =
{
       6,    22,     1,     3,     4,    75,     4,    39,    39,    41,
      42,    28,     7,    30,     4,    85,     0,    16,    17,    18,
      13,    14,    22,    23,     4,     3,     4,     3,     4,     6,
      28,    40,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    22,    23,    22,    23,    43,     7,
      49,    28,    26,    30,    75,    33,    34,    33,    34,    27,
       8,    39,    40,    39,    85,     9,    10,    11,    12,    13,
      14,    15,    16,     4,    13,    14,    39,     9,    10,    11,
      12,    13,    14,    15,    16,   106,    13,    14,    40,    33,
      34,    35,    36,    99,    33,    34,    35,    36,     4,    40,
      44,    33,    34,    35,    36,    22,    23,    24,    35,    36,
       5,    43,     9,    10,    11,    12,    13,    14,    15,    16,
       6,    29,     5,     9,    10,    11,    12,    13,    14,    15,
      16,    31,     0,    90,    64,   106,    33,    34,    35,    36,
      85,    79,    28,    40,    30,    -1,    -1,    33,    34,    35,
      36,     9,    10,    11,    12,    13,    14,    15,    16,    68,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    82,    -1,    33,    34,    35,    36
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,    28,    30,    46,    47,    48,    49,    51,    57,
      58,    62,     3,     4,    22,    23,    33,    34,    39,    54,
      55,    67,    39,     4,     0,    57,    47,    68,    54,    54,
      54,     9,    10,    11,    12,    13,    14,    15,    16,    33,
      34,    35,    36,    49,    53,    67,    26,    39,    41,    42,
      69,    40,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,     7,    52,    40,    27,    40,    54,
      70,     4,    54,    53,     8,    63,    39,    60,    69,    44,
      40,    69,    43,     4,    62,    65,    66,    67,    71,     4,
      59,    70,    69,    69,     5,    64,    66,    72,    40,    48,
      61,    22,    23,    24,    50,    29,     5,    31,     3,    55,
      56,    67
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    47,    47,    48,    48,    49,    49,    50,
      50,    50,    51,    52,    52,    53,    53,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    55,    55,    56,    56,    56,
      57,    57,    59,    58,    60,    60,    61,    61,    63,    64,
      62,    65,    65,    66,    66,    66,    68,    67,    69,    69,
      69,    69,    69,    70,    70,    72,    71,    71
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     1,     2,     1,     1,
       1,     1,     5,     2,     0,     1,     0,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     0,     8,     3,     0,     1,     0,     0,     0,
       8,     2,     1,     1,     1,     0,     0,     3,     3,     3,
       4,     4,     0,     1,     3,     0,     4,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 2: /* program: stmt_list  */
#line 244 "dbase.ypp"
                  {
        result = (yyvsp[0].str);
    }
#line 1376 "dbase.tab.cc"
    break;

  case 7: /* stmt_list: stmt_list stmt  */
#line 260 "dbase.ypp"
                       {
        *(yyvsp[-1].str) += *(yyvsp[0].str);
        (yyval.str) = (yyvsp[-1].str);
        delete (yyvsp[0].str);
    }
#line 1386 "dbase.tab.cc"
    break;

  case 8: /* stmt_list: stmt  */
#line 265 "dbase.ypp"
             {
        (yyval.str) = (yyvsp[0].str);
    }
#line 1394 "dbase.tab.cc"
    break;

  case 9: /* expr_string: CMD_SQSTRING  */
#line 271 "dbase.ypp"
                     {
        (yyval.str) = new std::string(*(yyvsp[0].str));
    }
#line 1402 "dbase.tab.cc"
    break;

  case 10: /* expr_string: CMD_DQSTRING  */
#line 274 "dbase.ypp"
                     {
        (yyval.str) = new std::string(*(yyvsp[0].str));
    }
#line 1410 "dbase.tab.cc"
    break;

  case 11: /* expr_string: CMD_BLOCKSTRING  */
#line 277 "dbase.ypp"
                        {
        (yyval.str) = new std::string(*(yyvsp[0].str));
    }
#line 1418 "dbase.tab.cc"
    break;

  case 12: /* if_stmt: CMD_IF expr stmt_block else_part CMD_ENDIF  */
#line 283 "dbase.ypp"
                                                   {
        indent--;
        term_stream[3] << get_indent() << "IF ("  << *(yyvsp[-3].str) << ") THEN" <<
        std::endl      << get_indent() << "BEGIN" <<
        std::endl      ;
        term_stream[3] << get_indent() << *(yyvsp[-2].str);
        if (!(yyvsp[-1].str)->empty()) {
            term_stream[3] << get_indent() << "ELSE"  <<
            std::endl      << get_indent() << "BEGIN" <<
            std::endl      ;
            term_stream[3] << get_indent() << *(yyvsp[-1].str)     <<
            std::endl;
        }
        term_stream[3] << "end;"     <<
        std::endl;
        
        delete (yyvsp[-3].str);
        delete (yyvsp[-2].str);
        delete (yyvsp[-1].str);
    }
#line 1443 "dbase.tab.cc"
    break;

  case 13: /* else_part: CMD_ELSE stmt_block  */
#line 306 "dbase.ypp"
                            {
        (yyval.str) = (yyvsp[0].str);
    }
#line 1451 "dbase.tab.cc"
    break;

  case 14: /* else_part: %empty  */
#line 309 "dbase.ypp"
                    {
        (yyval.str) = new std::string("");
    }
#line 1459 "dbase.tab.cc"
    break;

  case 15: /* stmt_block: stmt_list  */
#line 333 "dbase.ypp"
                  {
        (yyval.str) = (yyvsp[0].str);
    }
#line 1467 "dbase.tab.cc"
    break;

  case 16: /* stmt_block: %empty  */
#line 336 "dbase.ypp"
                    {
        (yyval.str) = new std::string("");
    }
#line 1475 "dbase.tab.cc"
    break;

  case 17: /* expr: CMD_NUMBER  */
#line 342 "dbase.ypp"
                         { (yyval.str) = new std::string(*(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 1481 "dbase.tab.cc"
    break;

  case 18: /* expr: qualified_access  */
#line 343 "dbase.ypp"
                         { (yyval.str) = new std::string(*(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 1487 "dbase.tab.cc"
    break;

  case 19: /* expr: string_literal  */
#line 344 "dbase.ypp"
                         { (yyval.str) = new std::string(*(yyvsp[0].str)); delete (yyvsp[0].str);
    }
#line 1494 "dbase.tab.cc"
    break;

  case 20: /* expr: '(' expr ')'  */
#line 346 "dbase.ypp"
                              {
        (yyval.str) = new std::string("(" + *(yyvsp[-1].str) + ")");
        delete (yyvsp[-1].str);
    }
#line 1503 "dbase.tab.cc"
    break;

  case 21: /* expr: expr '+' expr  */
#line 350 "dbase.ypp"
                      {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " + " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1513 "dbase.tab.cc"
    break;

  case 22: /* expr: expr '-' expr  */
#line 355 "dbase.ypp"
                      {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " - " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1523 "dbase.tab.cc"
    break;

  case 23: /* expr: expr '*' expr  */
#line 360 "dbase.ypp"
                      {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " * " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1533 "dbase.tab.cc"
    break;

  case 24: /* expr: expr '/' expr  */
#line 365 "dbase.ypp"
                      {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " / " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1543 "dbase.tab.cc"
    break;

  case 25: /* expr: expr CMD_EQ expr  */
#line 370 "dbase.ypp"
                         {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " == " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1553 "dbase.tab.cc"
    break;

  case 26: /* expr: expr CMD_NE expr  */
#line 375 "dbase.ypp"
                         {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " != " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1563 "dbase.tab.cc"
    break;

  case 27: /* expr: expr CMD_GE expr  */
#line 380 "dbase.ypp"
                         {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " >= " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1573 "dbase.tab.cc"
    break;

  case 28: /* expr: expr CMD_LE expr  */
#line 385 "dbase.ypp"
                         {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " <= " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1583 "dbase.tab.cc"
    break;

  case 29: /* expr: expr CMD_EG expr  */
#line 390 "dbase.ypp"
                         {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " => " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1593 "dbase.tab.cc"
    break;

  case 30: /* expr: expr CMD_EL expr  */
#line 395 "dbase.ypp"
                         {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " =< " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1603 "dbase.tab.cc"
    break;

  case 31: /* expr: expr CMD_GT expr  */
#line 400 "dbase.ypp"
                         {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " > " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1613 "dbase.tab.cc"
    break;

  case 32: /* expr: expr CMD_LT expr  */
#line 405 "dbase.ypp"
                         {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + " < " + *(yyvsp[0].str) + ")");
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1623 "dbase.tab.cc"
    break;

  case 33: /* expr: '-' expr  */
#line 410 "dbase.ypp"
                              { (yyval.str) = new std::string("-" + *(yyvsp[0].str)); }
#line 1629 "dbase.tab.cc"
    break;

  case 34: /* expr: '+' expr  */
#line 411 "dbase.ypp"
                              { (yyval.str) = new std::string("+" + *(yyvsp[0].str)); }
#line 1635 "dbase.tab.cc"
    break;

  case 42: /* $@1: %empty  */
#line 443 "dbase.ypp"
                                                       {
        //indent--;
        //if (indent < 1)
        //term_stream[0] << get_indent() << "end;" <<
        //std::endl ;  else
        //term_stream[1] << get_indent() << "type" <<
        //std::endl ;
        //indent++;
        term_stream[1] << "type" << std::endl;
        term_stream[1] << get_indent() << *(yyvsp[-3].str) << " = class(TForm)" <<
        std::endl << get_indent() << "public" <<
        std::endl ;
        indent++;
        term_stream[1] << get_indent() << "constructor Create;" <<
        std::endl      << get_indent() << "destructor Destroy;" <<
        std::endl      ;
        indent--;
        term_stream[1] << get_indent() << "end;" << std::endl;
        
        term_stream[2] <<
        std::endl      << "destructor " << *(yyvsp[-3].str) << ".Destroy;" <<
        std::endl      << "begin" <<
        std::endl      << "  inherited Destroy;" <<
        std::endl      << "end;"  <<
        std::endl      <<
        std::endl      << "constructor " << *(yyvsp[-3].str) << ".Create;" <<
        std::endl      << "begin" <<
        std::endl      << get_indent() << "inherited Create;" <<
        std::endl      ;
        delete (yyvsp[-3].str);
    }
#line 1671 "dbase.tab.cc"
    break;

  case 43: /* class_def: CMD_CLASS CMD_ID CMD_OF CMD_FORM form_optional $@1 class_list_opt CMD_ENDCLASS  */
#line 473 "dbase.ypp"
                                    {
        indent--;
        term_stream[2] << get_indent() << "end;" <<
        std::endl ;
    }
#line 1681 "dbase.tab.cc"
    break;

  case 44: /* form_optional: '(' CMD_ID ')'  */
#line 481 "dbase.ypp"
                       {
        (yyval.str) = new std::string(*(yyvsp[-1].str));
        delete (yyvsp[-1].str);
    }
#line 1690 "dbase.tab.cc"
    break;

  case 45: /* form_optional: %empty  */
#line 485 "dbase.ypp"
                    {
        (yyval.str) = nullptr;
    }
#line 1698 "dbase.tab.cc"
    break;

  case 48: /* $@2: %empty  */
#line 496 "dbase.ypp"
                                          {
        std::string* input = new std::string(*(yyvsp[-1].str));
        std::string  extracted;
        if (parseThis(*input, extracted)) {
            term_stream[2] << get_indent() << "with (" << extracted << ") do" <<
            std::endl      << get_indent() << "begin"                         <<
            std::endl;
            delete (yyvsp[-1].str);
            delete input;
            indent++;
        }   else {
            yyerror("unknow command.");
        }
        std::cout << "qualo" << std::endl;
    }
#line 1718 "dbase.tab.cc"
    break;

  case 49: /* $@3: %empty  */
#line 510 "dbase.ypp"
                   { std::cout << "uuuuu" << std::endl; }
#line 1724 "dbase.tab.cc"
    break;

  case 50: /* with_statement: CMD_WITH '(' qualified_access ')' $@2 with_block $@3 CMD_ENDWITH  */
#line 510 "dbase.ypp"
                                                                      {
        indent--;
        term_stream[2] << get_indent() << "end;" << std::endl;
    }
#line 1733 "dbase.tab.cc"
    break;

  case 56: /* $@4: %empty  */
#line 539 "dbase.ypp"
               { std::cout << "----->> " << *(yyvsp[0].str) << std::endl; }
#line 1739 "dbase.tab.cc"
    break;

  case 57: /* qualified_access: CMD_ID $@4 access_tail  */
#line 539 "dbase.ypp"
                                                                            {
    std::cout << "=>>> " << *(yyvsp[0].str) << " <<<-- " << std::endl;
        *(yyvsp[-2].str) += *(yyvsp[0].str);
        (yyval.str) = (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1750 "dbase.tab.cc"
    break;

  case 58: /* access_tail: '.' CMD_ID access_tail  */
#line 548 "dbase.ypp"
                               {
        (yyval.str) = new std::string("." + *(yyvsp[-1].str) + *(yyvsp[0].str));
        delete (yyvsp[-1].str);
        delete (yyvsp[0].str);
    }
#line 1760 "dbase.tab.cc"
    break;

  case 59: /* access_tail: '(' ')' access_tail  */
#line 553 "dbase.ypp"
                            {
        (yyval.str) = new std::string("()" + *(yyvsp[0].str));
        delete (yyvsp[0].str);
    }
#line 1769 "dbase.tab.cc"
    break;

  case 60: /* access_tail: '(' expr_list ')' access_tail  */
#line 557 "dbase.ypp"
                                      {
        (yyval.str) = new std::string("(" + *(yyvsp[-2].str) + ")" + *(yyvsp[0].str));
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1779 "dbase.tab.cc"
    break;

  case 61: /* access_tail: '[' expr ']' access_tail  */
#line 562 "dbase.ypp"
                                 {
        (yyval.str) = new std::string("[" + *(yyvsp[-2].str) + "]" + *(yyvsp[0].str));
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1789 "dbase.tab.cc"
    break;

  case 62: /* access_tail: %empty  */
#line 567 "dbase.ypp"
                    {
        (yyval.str) = new std::string("");
    }
#line 1797 "dbase.tab.cc"
    break;

  case 63: /* expr_list: expr  */
#line 573 "dbase.ypp"
             {
        (yyval.str) = new std::string(*(yyvsp[0].str));
        delete (yyvsp[0].str);
    }
#line 1806 "dbase.tab.cc"
    break;

  case 64: /* expr_list: expr ',' expr_list  */
#line 577 "dbase.ypp"
                           {
        (yyval.str) = new std::string(*(yyvsp[-2].str) + ", " + *(yyvsp[0].str));
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1816 "dbase.tab.cc"
    break;

  case 65: /* $@5: %empty  */
#line 585 "dbase.ypp"
                         { std::cout << "oooooooo" << std::endl; }
#line 1822 "dbase.tab.cc"
    break;

  case 66: /* assignment: qualified_access $@5 CMD_ASSIGN value  */
#line 585 "dbase.ypp"
                                                                                    {
        term_stream[2] << get_indent() << *(yyvsp[-3].str) << " = " << *(yyvsp[0].str) << std::endl;
        delete (yyvsp[-3].str);
        delete (yyvsp[0].str);
    }
#line 1832 "dbase.tab.cc"
    break;

  case 67: /* assignment: CMD_ID CMD_ASSIGN expr_string  */
#line 590 "dbase.ypp"
                                      {
        term_stream[0] << get_indent() << *(yyvsp[-2].str) << " = \"" << *(yyvsp[0].str) << "\"" << std::endl;
        delete (yyvsp[-2].str);
        delete (yyvsp[0].str);
    }
#line 1842 "dbase.tab.cc"
    break;


#line 1846 "dbase.tab.cc"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 639 "dbase.ypp"

//-- FUNCTION DEFINITIONS ---------------------------------
// --------------------------------------------------------
// CLEAN UP FUNCTION WHEN APPLICATION "EXIT" ...
// --------------------------------------------------------
void finish()
{
    fclose(yyin);
    fclose(yyout);
}

// --------------------------------------------------------
// FUNCTION TO CONVERT LOWERCASE CHARS TO UPPER CASE
// --------------------------------------------------------
std::string UpperCase(const std::string& input) {
    std::string res = input;
    std::transform(res.begin(), res.end(),
        res.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    return res;
}

// --------------------------------------------------------
// REPEAT std::string N-TIMES
// --------------------------------------------------------
std::string repeat(const std::string& str, int n) {
    std::string res;
    res.reserve(str.size() * n);
    for (int i = 0; i < n; ++i)
        res += str;
    return res;
}

// --------------------------------------------------------
// ENTRY POINT OF OUR COMPILER
// --------------------------------------------------------
int main(int argc, char **argv)
{
    //int8_t result  = 0;
    int8_t is_open = 0;

    std::cout << "dbase2delphi (c) 2025 Jens Kallup" <<
    std::endl << "all rights reserved." <<
    std::endl <<
    std::endl ;

    if (atexit(finish) != 0) {
        std::cerr << "Error: could not set exit routine." <<
        std::endl ;
        return 1;
    }
    
    if (argc > 0) {
        if (!(yyin = fopen(argv[1], "r"))) {
            std::cerr << "Error: can't find input file, use stdin." <<
            std::endl  ;
            is_open = 1;
            yyset_in  ( stdin  );
            yyset_out ( stdout );
        }   else {
            yyset_in  ( yyin );
        }

        if (!is_open) {
            std::stringstream ss;
            ss << argv[1] << ".pas";
    
            if (!(yyout = fopen(ss.str().c_str(), "w"))) {
                std::cerr << "Error: can't open output file, use stdout." <<
                std::endl ;
                yyset_out ( stdout );
            }
        }
    }   else {
        yyset_in (stdin);
        yyset_out(stdout);
    }

    std::string extension;
    std::string offext;
    
    if (argc > 0) {
        std::string filename = strdup(argv[1]);
        size_t pos = filename.rfind('.');
    
        if (pos != std::string::npos)
        {
            extension = filename.substr(pos + 1);
            offext = filename.substr(0, pos);
        
            if (extension != "prg") {
                std::cerr << "Error: file extension not .pas" <<
                std::endl ;
                exit(1);
            }
        }   else {
            std::cerr << "Error: could not found extension." <<
            std::endl ;
            exit(1);
        }
    }   else {
        std::cerr << "TODO: stdout" <<
        std::endl ;
        exit(1);
    }

    std::stringstream str;
    str << "// " << std::string(77, '-') << std::endl
        << "// Copyright(c) 2025 Jens Kallup"                   << std::endl
        << "// all rights reserved."                            << std::endl
        << "//"                                                 << std::endl
        << "// ATTENTION:"                                      << std::endl
        << "// This file was automaticaly created from: "       << std::endl
        << "// All changes will be lost on next convert run !"  << std::endl
        << "// " << std::string(77, '-') << std::endl
        
        << "{$mode delphi}"       << std::endl
        << "UNIT "   << offext    << ";"
        << std::endl << std::endl << "INTERFACE"
        << std::endl << std::endl << "USES"
        << std::endl << "  "      << "Interfaces, SysUtils, Classes, Forms, Dialogs;"
        << std::endl << std::endl;
    /*
    str << "TYPE"
        << std::endl << "  TParamScope = ("
        << std::endl << std::string(4, ' ') << "PS_UNKNOWN   = 0,"
        << std::endl << std::string(4, ' ') << "PS_LOCAL     = 1,"
        << std::endl << std::string(4, ' ') << "PS_GLOBAL    = 2,"
        << std::endl << std::string(4, ' ') << "PS_PRIVATE   = 3,"
        << std::endl << std::string(4, ' ') << "PS_PROTECTED = 4"
        << std::endl << "  );"
        << std::endl
        << "  TParamType = ("
        << std::endl << std::string(4, ' ') << "PT_UNKNOWN = 0,"
        << std::endl << std::string(4, ' ') << "PT_NUMERIC = 1,"
        << std::endl << std::string(4, ' ') << "PT_STRING  = 2,"
        << std::endl << std::string(4, ' ') << "PT_OBJECT  = 3"
        "  );"
        << std::endl
        << std::endl
        ;
        
    str << "TYPE"    << std::endl
        << "  TDBParameterClass = CLASS(TObject)" << std::endl
        << "  public"
        << std::endl
        << std::string(4, ' ') << "pName  : String      ;" << std::endl
        << std::string(4, ' ') << "pScope : TParamScope ;" << std::endl
        << std::string(4, ' ') << "pType  : TParamType  ;" << std::endl
        << std::string(4, ' ') << "pSize  : Integer     ;" << std::endl
        << std::string(4, ' ') << "pData  : PChar       ;" << std::endl
        << "  END;"
        << std::endl << std::endl;*/

    term_stream[4] << "procedure Main(Args: array of String);" << std::endl;
    term_stream[3] << "procedure Main(Args: array of String);" <<
    std::endl      << "begin" <<
    std::endl      ;
    
    indent++;
    
    scope_level = 0;
    yyparse();
    
    std::cout << str.str();
    std::cout << term_stream[1].str() << std::endl;
    std::cout << term_stream[0].str() << std::endl;
    std::cout << term_stream[4].str() << std::endl;
    
    std::cout << "implementation"     << std::endl;
    
    std::cout << term_stream[2].str() << std::endl;
    std::cout << term_stream[3].str() ;
    
    std::cout << "end;" <<
    std::endl <<
    std::endl << "end." <<
    std::endl <<
    std::endl ;
        
    if (indent < 1)
        term_stream[0] << get_indent() << "end;" <<
        std::endl;
    
    if (result) {
        std::cout << "SUCCESS" <<
        std::endl ;
        
        fprintf(yyout, "%s", str.str().c_str());
    } else {
        std::cout << "ERROR" <<
        std::endl ;
        exit(1);
    }

    return 0;
}
