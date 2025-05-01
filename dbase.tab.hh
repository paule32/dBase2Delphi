/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_DBASE_TAB_HH_INCLUDED
# define YY_YY_DBASE_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    CMD_NUMBER = 258,              /* CMD_NUMBER  */
    CMD_ID = 259,                  /* CMD_ID  */
    CMD_ASSIGN = 260,              /* CMD_ASSIGN  */
    CMD_IF = 261,                  /* CMD_IF  */
    CMD_ELSE = 262,                /* CMD_ELSE  */
    CMD_ENDIF = 263,               /* CMD_ENDIF  */
    CMD_NE = 264,                  /* CMD_NE  */
    CMD_EQ = 265,                  /* CMD_EQ  */
    CMD_GT = 266,                  /* CMD_GT  */
    CMD_LT = 267,                  /* CMD_LT  */
    CMD_EG = 268,                  /* CMD_EG  */
    CMD_EL = 269,                  /* CMD_EL  */
    CMD_GE = 270,                  /* CMD_GE  */
    CMD_LE = 271,                  /* CMD_LE  */
    CMD_PARAMETER = 272,           /* CMD_PARAMETER  */
    CMD_LOCAL = 273,               /* CMD_LOCAL  */
    CMD_PUBLIC = 274,              /* CMD_PUBLIC  */
    CMD_PRIVATE = 275,             /* CMD_PRIVATE  */
    CMD_PROTECTED = 276,           /* CMD_PROTECTED  */
    CMD_SQSTRING = 277,            /* CMD_SQSTRING  */
    CMD_DQSTRING = 278,            /* CMD_DQSTRING  */
    CMD_BLOCKSTRING = 279,         /* CMD_BLOCKSTRING  */
    CMD_NEW = 280,                 /* CMD_NEW  */
    CMD_OF = 281,                  /* CMD_OF  */
    CMD_FORM = 282,                /* CMD_FORM  */
    CMD_WITH = 283,                /* CMD_WITH  */
    CMD_ENDWITH = 284,             /* CMD_ENDWITH  */
    CMD_CLASS = 285,               /* CMD_CLASS  */
    CMD_ENDCLASS = 286,            /* CMD_ENDCLASS  */
    FILENAME = 287,                /* FILENAME  */
    UMINUS = 288,                  /* UMINUS  */
    UPLUS = 289                    /* UPLUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 157 "dbase.ypp"

  uint32_t      val_int32   ;
  char*         val_charp   ;
  char*         val_keyword ;
  std::string * str         ;

#line 105 "dbase.tab.hh"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_DBASE_TAB_HH_INCLUDED  */
