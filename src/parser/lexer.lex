	
%{
/*---------------------------------------------------------------------------------
 * 
 * Flex input for Ibex lexer
 * -------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stack>

#include "Interval.h"


/*#include "IbexEnv.h"
#include "IbexExpr.h"
#include "IbexSystemFactory.h"
#include "IbexIdTable.h"
#include "IbexFunction.h"
#include "IbexException.h" */

#include "ParserExpr.h"
#include "ParserFunction.h"
#include "ParserBlock.h"
#include "ParserConstraint.h"
#include "ParserContractor.h"
#include "ParserPrinter.h"

#include "parser.h"

#include "Scope.h"

namespace ibex {
namespace parser {

extern stack<Scope> scopes;

} // end namespace
} // end namespace

int ibex_lineno = 1;

using namespace ibex;
using namespace ibex::parser;

%}

%%

"parameters"|"Parameters"|"PARAMETERS"	  { return TK_PARAM; }
"constants"|"Constants"|"CONSTANTS"	  { return TK_CONST; }
"variables"|"Variables"|"VARIABLES"       { return TK_VARS; }
"function"|"Function"|"FUNCTION"   { return TK_FUNCTION; }
"max"                              { return TK_MAX; }
"min"                              { return TK_MIN; }
"inf"                              { return TK_INF; }
"mid"                              { return TK_MID; }
"sup"                              { return TK_SUP; }
"sign"                             { return TK_SIGN; }
"abs"                              { return TK_ABS; }
"in"			           { return TK_IN; }
"not-in"			   { return TK_NOT_IN; }
"exp"                              { return TK_EXPO; }
"ln"                               { return TK_LOG; }
"sqrt"                             { return TK_SQRT; }
"cos"                              { return TK_COS; }
"sin"                              { return TK_SIN; }
"tan"                              { return TK_TAN; }
"arccos"                           { return TK_ARCCOS; }
"arcsin"                           { return TK_ARCSIN; }
"arctan"                           { return TK_ARCTAN; }
"arctan2"                          { return TK_ARCTAN2; }
"cosh"                             { return TK_COSH; }
"sinh"                             { return TK_SINH; }
"tanh"                             { return TK_TANH; }
"arccosh"                          { return TK_ARCCOSH; }
"arcsinh"                          { return TK_ARCSINH; }
"arctanh"                          { return TK_ARCTANH; }
"begin"|"Begin"|"BEGIN"            { return TK_BEGIN; }
"end"|"End"|"END"                  { return TK_END;  }
"for"|"For"|"FOR"                  { return TK_FOR;  }
"column"|"Column"|"COLUMN"                { return TK_COLUMN; }
"transpose"|"Transpose"|"TRANSPOSE"       { return TK_TRANSPOSE; }
"atmost-nvector"                          { return TK_ATMOST_NVECTOR; }
"contractor-list"|"Contractor-List"       { return TK_CONTRACTOR_LIST; }
"constraint-list"|"Constraint-List"       { return TK_CONSTRAINT_LIST; }
"contractor"|"Contractor"|"CONTRACTOR"    { return TK_CONTRACTOR; }
"constraints"|"Constraints"|"CONSTRAINTS" { return TK_CTRS; }
"constraint"|"Constraint"|"CONSTRAINT"    { return TK_CONSTRAINT; }
"inter"                                   { return TK_INTERSEC; }
"union"                                   { return TK_UNION; }
"proj-inter"                              { return TK_PROJ_INTERSEC; }
"proj-union"                              { return TK_PROJ_UNION; }
"propag"|"Propag"|"PROPAG"                { return TK_PROPAG; }
"shave"|"Shave"|"SHAVE"                   { return TK_SHAVE; }
"newton"|"Newton"|"NEWTON"                { return TK_NEWTON; }
"maxdiamGT"|"MaxDiamGT"|"MAXDIAMGT"       { return TK_PRECISION; }
"q-inter"                                 { return TK_Q_INTER; }
"load"                                    { return TK_LOAD; }
"box-narrow"|"Box-narrow"|"Box-Narrow"|"BOX-NARROW"                 { return TK_BOX_NARROW; }
"true"|"false"                     { ibexlval._bool = ibextext[0]=='t' ? true : false; return TK_BOOL; }
"oo"                               { ibexlval.real = BiasPosInf; return TK_FLOAT; } 
"\""[^\n]*"\""                     { ibexlval.str = (char*) malloc(strlen(ibextext)-1);
                                     strncpy(ibexlval.str,&ibextext[1],strlen(ibextext)-2);   // copy while removing quotes
				     ibexlval.str[strlen(ibextext)-2]='\0';
                                     return TK_STRING; }
[_a-zA-Z][_a-zA-Z0-9]*	           { ibexlval.str = (char*) malloc(strlen(ibextext)+1);
				     strcpy(ibexlval.str,ibextext);
				     return scopes.top().token(ibextext);				       				                               }
([0-9]{6,10}[0-9]*|([0-9][0-9]*\.[0-9]*)|(\.[0-9]+))(e(\-|\+)?[0-9]+)?|([0-9]{1,5}e(\-|\+)?[0-9]+)  { 
                                     ibexlval.real = atof(ibextext); return TK_FLOAT; 
                                   }
[0-9]+                             { ibexlval.itg = atoi(ibextext); return TK_INTEGER; }

"//".*                             { /* C++-like comments */ }
"/*"([^*]|("*"[^/]))*"*/"          { /* C-like comments */ 
  /*strtok (ibextext,"\n");
    while (strtok(NULL,"\n")) ++ibex_lineno; */
  char* s=ibextext;
  while ((s=strpbrk(s,"\n"))) { s+=sizeof(char); ++ibex_lineno; }
                                   }

[ \t]+ 			           { /* skipping spaces */ }
"\n"			           { ++ibex_lineno; /* counting CR */ }

"<="                               { return TK_LEQ; }
">="                               { return TK_GEQ; }
"="                                { return TK_EQU; }
":="                               { return TK_ASSIGN; }
.			           { return ibextext [0]; }
<<EOF>>                            {YY_NEW_FILE; yyterminate();}

%%

int ibexwrap () { return 1; }

//"/""*"*([^*]|("*")+[^/])"*"*"/"    { /* C-like comments */ }

// "NEWTON"|"Newton"|"newton"         { return TK_NEWTON; } 
