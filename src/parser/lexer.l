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
#include <vector>
#include "ibex_Scope.h"
#include "ibex_Interval.h"
#include "ibex_Expr.h"
#include "ibex_SyntaxError.h"
#include "ibex_P_NumConstraint.h"

#include "parser.tab.hh"

namespace ibex {
namespace parser {
extern std::stack<Scope>& scopes();
extern bool choco_start;

} // end namespace
} // end namespace

int ibex_lineno = 1;

using namespace ibex;
using namespace ibex::parser;

%}

%%

%{
  if (choco_start) {
    choco_start = false; // reinit
    /* return pseudo-start token (to avoid shift/reduce conflict) */
    return TK_CHOCO;
  }
%}

"constants"|"Constants"|"CONSTANTS"	     { return TK_CONST; }
"variables"|"Variables"|"VARIABLES"      { return TK_VARS; }
"parameters"|"Parameters"|"PARAMETERS"	 { return TK_PARAM; }
"function"|"Function"|"FUNCTION"         { return TK_FUNCTION; }
"minimize"|"Minimize"|"MINIMIZE"         { return TK_MINIMIZE; }
"max"                            { return TK_MAX; }
"min"                            { return TK_MIN; }
"atan2"                          { return TK_ATAN2; }
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
"chi"                            { return TK_CHI; }

"in"                             { return TK_IN;  }
"inf"                            { return TK_INF; }
"mid"                            { return TK_MID; }
"sup"                            { return TK_SUP; }

"return"|"Return"|"RETURN"       { return TK_RETURN; }
"begin"|"Begin"|"BEGIN"          { return TK_BEGIN; }
"end"|"End"|"END"                { return TK_END;  }
"for"|"For"|"FOR"                { return TK_FOR;  }

"constraints"|"Constraints"|"CONSTRAINTS" { return TK_CTRS; }
 
"oo"                             { return TK_INFINITY; } 
"\""[^\n]*"\""                   { ibexlval.str = (char*) malloc(strlen(ibextext)-1);
                                   /* copy while removing quotes */
                                   strncpy(ibexlval.str,&ibextext[1],strlen(ibextext)-2);   
                                   ibexlval.str[strlen(ibextext)-2]='\0';
                                   return TK_STRING; 
                                 }
[_a-zA-Z][_a-zA-Z0-9]*	         { ibexlval.str = (char*) malloc(strlen(ibextext)+1);
                                   strcpy(ibexlval.str,ibextext);
                                   return scopes().top().token(ibextext);				       
                                 }
([0-9]{6,10}[0-9]*|([0-9][0-9]*\.[0-9]*)|(\.[0-9]+))(e(\-|\+)?[0-9]+)?|([0-9]{1,5}e(\-|\+)?[0-9]+)  { 
                                   ibexlval.real = atof(ibextext); return TK_FLOAT; 
                                 }
[0-9]+                           { ibexlval.itg = atoi(ibextext); return TK_INTEGER; }

"//".*                           { /* C++-like comments */ }
"/*"([^*]|("*"[^/]))*"*/"        { /* C-like comments */ 
                                   /*strtok (ibextext,"\n");
                                   while (strtok(NULL,"\n")) ++ibex_lineno; */
                                   char* s=ibextext;
                                   while ((s=strpbrk(s,"\n"))) { s+=sizeof(char); ++ibex_lineno; }
                                 }

[ \t]+                           { /* skipping spaces */ }
"\n"                             { ++ibex_lineno; /* counting CR */ }

"<="                             { return TK_LEQ; }
">="                             { return TK_GEQ; }
"="                              { return TK_EQU; }
":="                             { return TK_ASSIGN; }
.			         { return ibextext [0]; }
<<EOF>>                          {YY_NEW_FILE; yyterminate();}

%%

int ibexwrap () { return 1; }

extern int ibexparse();

void ibexparse_string(const char* syntax) {

	// We have to delete the buffer created (new_buff) at the end of
	// this function otherwise the next call to ibexparse_string(...)
	// will create another buffer and the first one would leak in memory.
	// But, then, if we instead parse a file (not a string) right after this
	// call, a memory fault occurs because YY_CURRENT_BUFFER (which points
	// to new_buff) has been destroyed.
	// 
	// I tried to save YY_CURRENT_BUFFER and restore its old value 
	// with ibex_switch_to_buffer(...) at the end, but it did not work.
	
	// So I force here deletion of YY_CURRENT_BUFFER
	// and then recreate the buffer using ibexrestart(...) at the end. 
	
	ibex_delete_buffer(YY_CURRENT_BUFFER);
	
	// copy string into a new buffer and switch buffers
	YY_BUFFER_STATE new_buff = ibex_scan_string (syntax);

	// analyze the string
	try {
	  ibexparse();
	} catch(SyntaxError& e) {
	  // delete the new buffer
	  ibex_delete_buffer(new_buff);
	  ibexrestart(stdin);
      throw(e);
	}

	// delete the new buffer
	ibex_delete_buffer(new_buff);
		
	// necessary, otherwise provokes a memory fault when another 
	// file is read after (this fault does not occur if we parse a system 
	// from a string again). I also tried to save YY_CURRENT_BUFFER
	// and restore the old buffer with ibex_switch_to_buffer(...) but
	// it did not work.  
	ibexrestart(stdin);
}

//"/""*"*([^*]|("*")+[^/])"*"*"/"    { /* C-like comments */ }
