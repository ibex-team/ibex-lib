%{

//============================================================================
//                                  I B E X                                   
// File        : Flex input for Ibex lexer
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================	

#include <stdlib.h>
#include <stack>

#include "Interval.h"

//#include "ParserExpr.h"
//#include "ParserFunction.h"
//#include "ParserBlock.h"
////#include "ParserConstraint.h"
//#include "ParserContractor.h"
//#include "ParserPrinter.h"

#include "parser.h"
#include "ibex_ParserScope.h"

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

"parameters"|"Parameters"|"PARAMETERS"	 { return TK_PARAM; }
"constants"|"Constants"|"CONSTANTS"	     { return TK_CONST; }
"variables"|"Variables"|"VARIABLES"      { return TK_VARS; }
"function"|"Function"|"FUNCTION" { return TK_FUNCTION; }
"max"                            { return TK_MAX; }
"min"                            { return TK_MIN; }
"atan2"                          { return TK_ATAN2; }
"inf"                            { return TK_INF; }
"mid"                            { return TK_MID; }
"sup"                            { return TK_SUP; }
"sign"                           { return TK_SIGN; }
"abs"                            { return TK_ABS; }
"exp"                            { return TK_EXPO; }
"ln"                             { return TK_LOG; }
"sqrt"                           { return TK_SQRT; }
"cos"                            { return TK_COS; }
"sin"                            { return TK_SIN; }
"tan"                            { return TK_TAN; }
"acos"                           { return TK_ACOS; }
"asin"                           { return TK_ASIN; }
"atan"                           { return TK_ATAN; }
"cosh"                           { return TK_COSH; }
"sinh"                           { return TK_SINH; }
"tanh"                           { return TK_TANH; }
"acosh"                          { return TK_ACOSH; }
"asinh"                          { return TK_ASINH; }
"atanh"                          { return TK_ATANH; }
"begin"|"Begin"|"BEGIN"          { return TK_BEGIN; }
"end"|"End"|"END"                { return TK_END;  }
"for"|"For"|"FOR"                { return TK_FOR;  }
"constraints"|"Constraints"|"CONSTRAINTS" { return TK_CTRS; }
"oo"                                      { ibexlval.real = POS_INFINITY; return TK_FLOAT; } 
"\""[^\n]*"\""                            { ibexlval.str = (char*) malloc(strlen(ibextext)-1);
                                     		strncpy(ibexlval.str,&ibextext[1],strlen(ibextext)-2);   // copy while removing quotes
				     						ibexlval.str[strlen(ibextext)-2]='\0';
                                     		return TK_STRING; 
                                     	  }
[_a-zA-Z][_a-zA-Z0-9]*	          { ibexlval.str = (char*) malloc(strlen(ibextext)+1);
				     				strcpy(ibexlval.str,ibextext);
				     				return scopes.top().token(ibextext);				       				                               
				     			  }
([0-9]{6,10}[0-9]*|([0-9][0-9]*\.[0-9]*)|(\.[0-9]+))(e(\-|\+)?[0-9]+)?|([0-9]{1,5}e(\-|\+)?[0-9]+)  { 
                                    ibexlval.real = atof(ibextext); return TK_FLOAT; 
                                  }
[0-9]+                            { ibexlval.itg = atoi(ibextext); return TK_INTEGER; }

"//".*                            { /* C++-like comments */ }
"/*"([^*]|("*"[^/]))*"*/"         { /* C-like comments */ 
  									/*strtok (ibextext,"\n");
    								while (strtok(NULL,"\n")) ++ibex_lineno; */
  									char* s=ibextext;
  									while ((s=strpbrk(s,"\n"))) { s+=sizeof(char); ++ibex_lineno; }
                                  }

[ \t]+ 			           		  { /* skipping spaces */ }
"\n"			           		  { ++ibex_lineno; /* counting CR */ }

"<="                              { return TK_LEQ; }
">="                              { return TK_GEQ; }
"="                               { return TK_EQU; }
":="                              { return TK_ASSIGN; }
.			           			  { return ibextext [0]; }
<<EOF>>                           {YY_NEW_FILE; yyterminate();}

%%

int ibexwrap () { return 1; }

//"/""*"*([^*]|("*")+[^/])"*"*"/"    { /* C-like comments */ }
