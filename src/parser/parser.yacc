%{
//============================================================================
//                                  I B E X                                   
// File        : Yacc/Bison input for Ibex parser
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//===========================================================================

#include <math.h>
#include <vector>

#include "ibex_Interval.h"
#include "ibex_Array.h"
#include "ibex_System.h"

#include "ibex_SyntaxError.h"
#include "ibex_ParserSource.h"
#include "ibex_P_Expr.h"
#include "ibex_P_NumConstraint.h"
#include "ibex_MainGenerator.h"
#include "ibex_ConstantGenerator.h"
#include "ibex_P_ExprGenerator.h"

using namespace std;

extern int ibexlex(void);
extern char* ibextext;
extern int ibex_lineno;

void ibexerror (const std::string& msg) {
  throw ibex::SyntaxError(msg, ibextext, ibex_lineno);
}
   
namespace ibex { 

System* parser_result;

namespace parser {

P_Source source;

Entity::Type ent_type; // current type, either var/epr/syb
stack<Scope> scopes;

void begin() {
  ibex_lineno=-1;
  if (!setlocale(LC_NUMERIC, "C")) // to accept the dot (instead of the french coma) with numeric numbers
    ibexerror("platform does not support \"C\" locale");
  
  ibex_lineno=1;

  /* there may be some pending scopes (if the previous call to the parser failed).
   */
  while (!scopes.empty()) scopes.pop(); 
  
  scopes.push(Scope()); // a fresh new scope!
}

int _2int(const ExprNode& expr) {
	int n=ConstantGenerator(scopes.top()).eval_integer(expr);
	cleanup(expr,true); // false or true (there is no symbols)
	return n;
}

double _2dbl(const ExprNode& expr) {
	double d=ConstantGenerator(scopes.top()).eval_double(expr);
	cleanup(expr,true); // false or true (there is no symbols)
	return d;
}

Domain _2domain(const ExprNode& expr) {
	Domain d=ConstantGenerator(scopes.top()).eval(expr);
	cleanup(expr,true); // false or true (there is no symbols)
	return d;
}

void end() {
   MainGenerator().generate(source,*parser_result);
	source.cleanup();
    
    // we have to cleanup the data in case of Syntax Error 
    // TO DO...
    
}

} // end namespace
} // end namespace


using namespace ibex;
using namespace parser;
using namespace std;

%}	

%union{
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

}

%token <str> TK_CONSTANT
%token <str> TK_NEW_SYMBOL
%token <str> TK_FUNC_SYMBOL
%token <str> TK_FUNC_RET_SYMBOL
%token <str> TK_FUNC_INP_SYMBOL
%token <str> TK_FUNC_TMP_SYMBOL
%token <str> TK_ENTITY
%token <str> TK_ITERATOR

%token <str> TK_STRING
%token <itg> TK_INTEGER
%token <real> TK_FLOAT
%token <_bool> TK_BOOL

%token TK_PARAM TK_CONST TK_VARS TK_FUNCTION 
%token TK_MIN TK_MAX TK_INF TK_MID TK_SUP TK_SIGN TK_ABS
%token TK_SQRT TK_EXPO  TK_LOG 
%token TK_COS  TK_SIN   TK_TAN  TK_ACOS  TK_ASIN  TK_ATAN TK_ATAN2
%token TK_COSH TK_SINH  TK_TANH TK_ACOSH TK_ASINH TK_ATANH
%token TK_LEQ  TK_GEQ   TK_EQU  TK_ASSIGN

%token TK_BEGIN TK_END TK_FOR TK_FROM TK_TO

%token TK_CTRS TK_MINIMIZE

%token TK_IN

%nonassoc TK_EQU '<' TK_LEQ '>' TK_GEQ    /* on interdit donc a < b < c */
%left '+' '-' TK_UNION
%left '*' '/' TK_INTERSEC
%left '^'
%left '['

/* --------------------------- Nonterminals types -------------------------- */

%type<dim>        dimension

/* entities */
%type<entity_symbol>     decl_entity

/* functions */
%type<func_input_symbols>  fnc_inpt_list
%type<func_input_symbol>   fnc_input

/* constraints */
%type<constraints>  ctr_blk_list
%type<constraints>  ctr_blk_list_ // ctr_blk_list without ending semicolon
%type<constraint>   ctr_blk
%type<constraint>   ctr_loop
%type<constraint>   ctr 
 
%type<itv>         interval

/* expressions */
%type<expression>  expr
%type<expressions> expr_list

%%


program       :                                      { begin(); }
              decl_opt_cst 
              TK_VARS                   
	          decl_var_list ';'   
              decl_opt_par
              decl_fnc_list
              decl_opt_goal
	          decl_ctr_list                          { end(); }
              ;

/**********************************************************************************************************************/
/*                                                SYMBOLS                                                         */
/**********************************************************************************************************************/

decl_opt_cst  : 
              | TK_CONST decl_cst_list ';'
	          ;

decl_cst_list : decl_cst                                    
              | decl_cst_list ';' decl_cst
              ;

decl_cst      : TK_NEW_SYMBOL TK_EQU expr       { scopes.top().add_cst($1, _2domain(*$3)); free($1); }
              | TK_NEW_SYMBOL TK_IN expr        { scopes.top().add_cst($1, _2domain(*$3)); free($1); }
              ;

decl_opt_par  : 
              | TK_PARAM decl_par_list ';'
              ; 
 
decl_par_list : decl_par                                    
              | decl_par_list ';' decl_par
              | decl_par_list ',' decl_par
              ;

decl_par      : '!' decl_entity                 { $2->type=Entity::EPR; source.vars.push_back($2); }
	          |     decl_entity                 { $1->type=Entity::SYB; source.vars.push_back($1); }
              ;

decl_var_list : decl_var                             
              | decl_var_list ';' decl_var           
              | decl_var_list ',' decl_var           
              ;

decl_var      : decl_entity                     { $1->type=Entity::VAR; 
	                                              source.vars.push_back($1); }
              ;
              
decl_entity   : TK_NEW_SYMBOL dimension         { $$ = new Entity($1,*$2,Interval::ALL_REALS);
		                                          scopes.top().add_entity($1,$$);  
		                                          free($1); delete $2; }
              | TK_NEW_SYMBOL dimension 
	            TK_IN expr                      { $$ = new Entity($1,*$2,_2domain(*$4));
		                                          scopes.top().add_entity($1,$$); 
						                          free($1); delete $2; }
              ; 

dimension     :                                 { $$=new Dim(); }
              | '[' expr ']'                    { $$=new Dim(Dim::col_vec(_2int(*$2)));  }
              | '[' expr ']' '[' expr ']'       { $$=new Dim(Dim::matrix(_2int(*$2),_2int(*$5))); }
              | '[' expr ']' '[' expr ']' '[' expr ']'                    
                                                { $$=new Dim(Dim::matrix_array(_2int(*$2),_2int(*$5),_2int(*$8))); }
	          ;

interval      : '[' expr ',' expr ']'           { $$=new Interval(_2dbl(*$2), _2dbl(*$4)); }
              ;

/**********************************************************************************************************************/
/*                                                FUNCTIONS                                                           */
/**********************************************************************************************************************/

decl_fnc_list : decl_fnc_list decl_fnc 
              | 
              ;

decl_fnc      : TK_FUNCTION                     { scopes.push(Scope(scopes.top(),true)); }
                TK_NEW_SYMBOL                   { scopes.top().add_func_return($3); }
                TK_EQU TK_NEW_SYMBOL
                '(' fnc_inpt_list ')'
                fnc_code
                TK_FUNC_RET_SYMBOL TK_EQU expr semicolon_opt
                TK_END                          { 
                								  Array<const ExprSymbol> x($8->size());
                								  int i=0;
                								  for(vector<const ExprSymbol*>::const_iterator it=$8->begin(); it!=$8->end(); it++)
                								      x.set_ref(i++,ExprSymbol::new_((*it)->name,(*it)->dim));
                								  const ExprNode& y= ExprGenerator(scopes.top()).generate(Array<const ExprSymbol>(*$8),x,*$13);
                								  Function* f=new Function(x,y,$6);
                                                  source.func.push_back(f);
                                                  scopes.pop();
                                                  scopes.top().add_func($3,f); 
                                                  source.func.push_back(f);
                                                  free($3); free($6); 
                                                  cleanup(*$13,true); // will also delete symbols in $8
                                                  delete $8; }
              ;

semicolon_opt : ';' | ;

fnc_inpt_list : fnc_inpt_list ',' fnc_input     { $1->push_back($3); $$=$1; }
              | fnc_input                       { $$=new vector<const ExprSymbol*>(); $$->push_back($1); }
              ;

fnc_input     : TK_NEW_SYMBOL dimension         { $$=&ExprSymbol::new_($1,*$2);
                                                  scopes.top().add_func_input($1,$$);  
                                                  free($1); delete $2; }
              ;

fnc_code      : fnc_code fnc_assign ';'              
              | fnc_assign ';'                       
              ;

fnc_assign    : TK_NEW_SYMBOL TK_EQU expr       { /* note: if this tmp symbol is not used, the expr $3 will never be deleted */
                                                  scopes.top().add_func_tmp_symbol($1,$3); free($1); }
              ;

/**********************************************************************************************************************/
/*                                                  GOAL                                                              */
/**********************************************************************************************************************/
decl_opt_goal :                                 { source.goal = NULL; }
              | TK_MINIMIZE expr semicolon_opt  { source.goal = $2; }
              ;

/**********************************************************************************************************************/
/*                                                CONSTRAINTS                                                         */
/**********************************************************************************************************************/
decl_ctr_list : TK_CTRS
                ctr_blk_list TK_END             { source.ctrs=new P_ConstraintList(*$2); }
              ;
              
ctr_blk_list  : ctr_blk_list_ semicolon_opt     { $$ = $1; }
              ;

ctr_blk_list_ : ctr_blk_list_ ';' ctr_blk       { $1->push_back($3); $$ = $1; }
              | ctr_blk                         { $$ = new vector<P_NumConstraint*>();
              								      $$->push_back($1); }
              ;

ctr_blk       : ctr_loop                        { $$ = $1; }
              | ctr                             { $$ = $1; }
              ;


ctr_loop      : TK_FOR TK_NEW_SYMBOL TK_EQU
				expr ':' expr ';'               { scopes.push(scopes.top());
						       					 scopes.top().add_iterator($2); }
                ctr_blk_list TK_END             { $$ = new P_ConstraintLoop($2, _2int(*$4), _2int(*$6), *$9); 
						                          scopes.pop();
		                                          free($2); }
              ;

ctr           : expr TK_EQU expr                { $$ = new P_OneConstraint(*$1,NumConstraint::EQ,*$3); }
              | expr TK_LEQ expr                { $$ = new P_OneConstraint(*$1,NumConstraint::LEQ,*$3); }
              | expr TK_GEQ expr                { $$ = new P_OneConstraint(*$1,NumConstraint::GEQ,*$3); }
              | expr   '<'  expr                { $$ = new P_OneConstraint(*$1,NumConstraint::LT,*$3); }
              | expr   '>'  expr                { $$ = new P_OneConstraint(*$1,NumConstraint::GT,*$3); }
              | '(' ctr ')'                     { $$ = $2; }
              ; 
              
/**********************************************************************************************************************/
/*                                                EXPRESSIONS                                                         */
/**********************************************************************************************************************/

expr          : expr '+' expr	                { $$ = &(*$1 + *$3); }
              | expr '*' expr	                { $$ = &(*$1 * *$3); }
              | expr '-' expr	                { $$ = &(*$1 - *$3); }
              | expr '/' expr	                { $$ = &(*$1 / *$3); }
              | TK_MAX '(' expr ',' expr ')'    { $$ = &max(*$3,*$5); }
              | TK_MIN '(' expr ',' expr ')'    { $$ = &min(*$3,*$5); }
              | TK_ATAN2 '(' expr ',' expr ')'  { $$ = &atan2(*$3,*$5); }
              | '-' expr                        { $$ = &(-*$2); }
              | TK_ABS  '(' expr ')'            { $$ = &abs  (*$3); }
              | TK_SIGN '(' expr ')'            { $$ = &sign (*$3); }
              | expr '^' expr	                { $$ = new P_ExprPower(*$1, *$3); }
              | TK_SQRT '(' expr ')'            { $$ = &sqrt (*$3); }
              | TK_EXPO '(' expr ')'            { $$ = &exp  (*$3); }
              | TK_LOG '(' expr ')'             { $$ = &log  (*$3); }
              | TK_COS '(' expr ')'             { $$ = &cos  (*$3); }
              | TK_SIN '(' expr ')'             { $$ = &sin  (*$3); }
              | TK_TAN '(' expr ')'             { $$ = &tan  (*$3); }
              | TK_ACOS '(' expr ')'            { $$ = &acos (*$3); }
              | TK_ASIN '(' expr ')'            { $$ = &asin (*$3); }
              | TK_ATAN '(' expr ')'            { $$ = &atan (*$3); }
              | TK_COSH '(' expr ')'            { $$ = &cosh (*$3); }
              | TK_SINH '(' expr ')'            { $$ = &sinh (*$3); }
              | TK_TANH '(' expr ')'            { $$ = &tanh (*$3); }
              | TK_ACOSH '(' expr ')'           { $$ = &acosh(*$3); }
              | TK_ASINH '(' expr ')'           { $$ = &asinh(*$3); }
              | TK_ATANH '(' expr ')'           { $$ = &atanh(*$3); }
              | '+' expr                        { $$ = $2; }
              | '(' expr ')'		            { $$ = $2; }
              | expr '[' expr ']'               { $$ = new P_ExprIndex(*$1,*$3); }
              | '(' expr_list ')'               { $$ = &ExprVector::new_(Array<const ExprNode>(*$2),false); delete $2; }
              | TK_ENTITY                       { $$ = &scopes.top().get_entity($1).symbol; free($1); /* cannot happen inside a function expr */}
              | TK_ITERATOR                     { $$ = new ExprIter($1); free($1); }
              | TK_FUNC_INP_SYMBOL              { $$ = &scopes.top().get_func_input_symbol($1); free($1); }
              | TK_FUNC_TMP_SYMBOL              { $$ = &scopes.top().get_func_tmp_expr($1); free($1); }
              | TK_CONSTANT                     { $$ = &ExprConstant::new_(scopes.top().get_cst($1)); free($1); }
              | TK_FUNC_SYMBOL '(' expr ')'     { $$ = &scopes.top().get_func($1)(*$3); free($1); }
              | TK_FUNC_SYMBOL '(' expr_list ')'{ $$ = &scopes.top().get_func($1)(*$3); free($1); delete $3; }
              | TK_FLOAT                        { $$ = &ExprConstant::new_scalar($1); }
              | TK_INTEGER                      { $$ = &ExprConstant::new_scalar((double) $1); }
              | interval                        { $$ = &ExprConstant::new_scalar(*$1); delete $1; }
              | TK_INF '(' expr ')'             { $$ = &ExprConstant::new_scalar(_2domain(*$3).i().lb()); }
              | TK_MID '(' expr ')'             { $$ = &ExprConstant::new_scalar(_2domain(*$3).i().ub()); }
              | TK_SUP '(' expr ')'             { $$ = &ExprConstant::new_scalar(_2domain(*$3).i().mid()); }
              ;
	      
expr_list     : expr_list ',' expr              { $1->push_back($3); $$=$1; }
              | expr ',' expr                   { $$ = new vector<const ExprNode*>; 
                                                  $$->push_back($1); $$->push_back($3); }
              ;
