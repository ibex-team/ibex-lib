
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_CONSTANT = 258,
     TK_NEW_SYMBOL = 259,
     TK_FUNC_SYMBOL = 260,
     TK_FUNC_RET_SYMBOL = 261,
     TK_FUNC_INP_SYMBOL = 262,
     TK_FUNC_TMP_SYMBOL = 263,
     TK_ENTITY = 264,
     TK_ITERATOR = 265,
     TK_STRING = 266,
     TK_INTEGER = 267,
     TK_FLOAT = 268,
     TK_BOOL = 269,
     TK_PARAM = 270,
     TK_CONST = 271,
     TK_VARS = 272,
     TK_FUNCTION = 273,
     TK_MIN = 274,
     TK_MAX = 275,
     TK_INF = 276,
     TK_MID = 277,
     TK_SUP = 278,
     TK_SIGN = 279,
     TK_ABS = 280,
     TK_SQRT = 281,
     TK_EXPO = 282,
     TK_LOG = 283,
     TK_COS = 284,
     TK_SIN = 285,
     TK_TAN = 286,
     TK_ACOS = 287,
     TK_ASIN = 288,
     TK_ATAN = 289,
     TK_ATAN2 = 290,
     TK_COSH = 291,
     TK_SINH = 292,
     TK_TANH = 293,
     TK_ACOSH = 294,
     TK_ASINH = 295,
     TK_ATANH = 296,
     TK_LEQ = 297,
     TK_GEQ = 298,
     TK_EQU = 299,
     TK_ASSIGN = 300,
     TK_BEGIN = 301,
     TK_END = 302,
     TK_FOR = 303,
     TK_FROM = 304,
     TK_TO = 305,
     TK_CTRS = 306,
     TK_MINIMIZE = 307,
     TK_IN = 308,
     TK_UNION = 309,
     TK_INTERSEC = 310
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 121 "parser.yacc"

  char*     str;
  int       itg;
  double    real;
  
  ibex::Interval* itv;
  
  ibex::Dim*      dim;
  
  const ibex::ExprSymbol*                 func_input_symbol;
  std::vector<const ibex::ExprSymbol*>*   func_input_symbols;

  ibex::parser::Entity*                   entity_symbol;

  ibex::parser::P_NumConstraint*               constraint;
  std::vector<ibex::parser::P_NumConstraint*>* constraints;

  const ibex::ExprNode*                expression;
  std::vector<const ibex::ExprNode*>*  expressions;




/* Line 1676 of yacc.c  */
#line 131 "parser.tab.hacc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE ibexlval;


