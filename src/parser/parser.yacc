%{
//============================================================================
//                                  I B E X                                   
// File        : Yacc/Bison input for Ibex parser
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//==========================

#include <math.h>
#include <vector>

#include "ibex_ParserExpr.h"
#include "ibex_ParserNumConstraint.h"
#include "ibex_Array.h"

using namespace std;

extern int ibexlex(void);
extern char* ibextext;
extern int ibex_lineno;

void ibexerror (const std::string& msg) {
  throw ibex::SyntaxErrorException(msg, ibextext, ibex_lineno);
}
   
namespace ibex { 
namespace parser {


stack<Scope>   scopes;

void init() {
  ibex_lineno=-1;
  if (!setlocale(LC_NUMERIC, "C")) // to accept the dot (instead of the french coma) with numeric numbers
    ibexerror("platform does not support \"C\" locale");
  
  ibex_lineno=1;

  /* there may be some pending scopes (if the previous call to the parser failed).
   * (in this case, main_ctr_list and main_ctc_list could not have been created). */
  while (!scopes.empty()) scopes.pop(); 
  scopes.push(Scope());
}

} // end namespace
} // end namespace


using namespace ibex;
using namespace parser;

%}	

%union{
  char*     str;
  int       itg;
  double    real;
  Interval* itv;
  vector<const char*>*                   strvec;

  struct ibex::Dim*                      dim;
  const ibex::parser::P_ExtendedSymbol*  extsbl;

  vector<const ibex::parser::P_Assign*>* assvec;
  const ibex::parser::P_Assign*          ass;


  vector<ibex::parser::P_NumConstraint*> _ctrblklst;
  ibex::parser::P_NumConstraint*         _ctrblk;
  const ibex::NumConstraint*             _ctr;

  vector<ibex::parser::P_ConstantExpr>* cstvec;
  ibex::parser::P_ConstantExpr*         cst;
  const ibex::ExprNode*                 _expr;
  vector<const ibex::ExprNode*>*        exprvec;

}

%token <str> TK_ENTITY
%token <str> TK_NEW_SYMBOL
%token <str> TK_TMP_SYMBOL
%token <str> TK_INPUT
%token <str> TK_OUTPUT
%token <str> TK_ITERATOR
%token <str> TK_CTC_LIST
%token <str> TK_CTR_LIST
%token <str> TK_CTC
%token <str> TK_CTR
%token <str> TK_FUNC

%token <str> TK_STRING
%token <itg> TK_INTEGER
%token <real> TK_FLOAT
%token <_bool> TK_BOOL
%token <str> TK_CONSTANT

%token TK_PARAM TK_CONST TK_VARS TK_FUNCTION 
%token TK_MIN TK_MAX TK_INF TK_MID TK_SUP TK_SIGN TK_ABS
%token TK_SQRT TK_EXPO  TK_LOG 
%token TK_COS  TK_SIN   TK_TAN  TK_ARCCOS  TK_ARCSIN  TK_ARCTAN TK_ARCTAN2
%token TK_COSH TK_SINH  TK_TANH TK_ARCCOSH TK_ARCSINH TK_ARCTANH
%token TK_LEQ  TK_GEQ   TK_EQU  TK_ASSIGN

%token TK_BEGIN TK_END TK_FOR TK_FROM TK_TO

%token TK_CTRS TK_CONSTRAINT TK_CONSTRAINT_LIST

%nonassoc TK_EQU

%nonassoc '<' TK_LEQ '>' TK_GEQ    /* on interdit donc a < b < c */
%left '+' '-' TK_UNION
%left '*' '/' TK_INTERSEC
%left '^'


/* --------------------------- Nonterminals types -------------------------- */

%type<extsbl>     identifier
%type<dim>        dimension

/* functions */
%type<assvec>     fnc_code
%type<strvec>     fnc_inpt_list
%type<str>        fnc_input
%type<ass>        fnc_assign
%type<extsbl>     fnc_ass_left

/* constraints */
%type<_ctrblklst> ctr_blk_list
%type<_ctrblklst> ctr_blk_list_ // ctr_blk_list without ending semicolon
%type<_ctrblk>    ctr_blk
%type<_ctrblk>    ctr_loop
%type<_ctr>       ctr 
 
/* const expressions */
%type<cst>        const_expr
%type<cst>        const_vector
%type<cstvec>     const_list
%type<itv>        interval

/* expressions */
%type<iter>       iter_expr
%type<_expr>      expr
%type<exprvec>    expr_list

%%


/**********************************************************************************************************************/

/*                                                CONSTRAINTS                                                         */
/**********************************************************************************************************************/
decl_ctr_list : TK_CTRS
                ctr_blk_list TK_END            { vector<const NumConstraint*>* vec = new vector<const NumConstraint*>();
		                                         Generator g(*vec);
						                         g.visit(*$2);
						                         delete $2;
						                       }
              ;
              
ctr_blk_list  : ctr_blk_list_ semi_col_opt     { $$ = $1; }
              ;

ctr_blk_list_ : ctr_blk_list_ ';' ctr_blk      { $1->add(*$3); $$ = $1; }
              | ctr_blk                        { $$ = new vector<P_NumConstraint*>(*$1); }
              ;

ctr_blk       : ctr_loop                       { $$ = $1; }
              | ctr                            { $$ = new P_OneConstraint($1); }
              ;


ctr_loop      : TK_FOR TK_NEW_SYMBOL TK_EQU
				const_expr ':' const_expr ';'  { scopes.push(scopes.top());
						       					 scopes.top().add_iterator($2); }
                ctr_blk_list TK_END            { $$ = new P_ConstraintLoop($2, $4->_2int(), $6->_2int(), *$9); 
						                         scopes.pop();
		                                         free($2); delete $4; delete $6; }
              ;

ctr           : expr TK_EQU expr               { $$ = &(*$1 =*$3); }
              | expr TK_LEQ expr               { $$ = &(*$1<=*$3); }
              | expr TK_GEQ expr               { $$ = &(*$1>=*$3); }
              | expr   '<'  expr               { $$ = &(*$1< *$3); }
              | expr   '>'  expr               { $$ = &(*$1> *$3); }
              | '(' num_ctr ')'                { $$ = $2; }
              ; 

/**********************************************************************************************************************/
/*                                      CONSTANT EXPRESSIONS (evaluated on-the-fly)                                   */
/**********************************************************************************************************************/

const_expr    : const_expr  '+' const_expr	   { *$1 = *$1 + *$3;      $$=$1; delete $3; }
	          | const_expr  '*' const_expr	   { *$1 = *$1 * *$3;      $$=$1; delete $3; }
	          | const_expr  '-' const_expr	   { *$1 = *$1 - *$3;      $$=$1; delete $3; }
              | const_expr  '/' const_expr	   { *$1 = *$1 / *$3;      $$=$1; delete $3; }
              | const_expr  '^' const_expr	   { *$1 = pow (*$1,*$3);  $$=$1; delete $3; }
              | TK_ATAN2   '(' const_expr ',' 
	                           const_expr ')'  { *$3 = atan2(*$3,*$5); $$=$3; delete $5; }
	          | TK_ABS   '(' const_expr ')'    { *$3 = abs (*$3);      $$=$3; }
              | TK_SQRT  '(' const_expr ')'    { *$3 = sqrt (*$3);     $$=$3; }
              | TK_EXPO  '(' const_expr ')'    { *$3 = exp  (*$3);     $$=$3; }
              | TK_LOG   '(' const_expr ')'    { *$3 = log  (*$3);     $$=$3; }
              | TK_COS   '(' const_expr ')'    { *$3 = cos  (*$3);     $$=$3; }
              | TK_SIN   '(' const_expr ')'    { *$3 = sin  (*$3);     $$=$3; }
              | TK_TAN   '(' const_expr ')'    { *$3 = tan  (*$3);     $$=$3; }
              | TK_ACOS  '(' const_expr ')'    { *$3 = acos (*$3);     $$=$3; }
              | TK_ASIN  '(' const_expr ')'    { *$3 = asin (*$3);     $$=$3; }
              | TK_ATAN  '(' const_expr ')'    { *$3 = atan (*$3);     $$=$3; }
              | TK_COSH  '(' const_expr ')'    { *$3 = cosh (*$3);     $$=$3; }
              | TK_SINH  '(' const_expr ')'    { *$3 = sinh (*$3);     $$=$3; }
              | TK_TANH  '(' const_expr ')'    { *$3 = tanh (*$3);     $$=$3; }
              | TK_ACOSH '(' const_expr ')'    { *$3 = acosh(*$3);     $$=$3; }
              | TK_ASINH '(' const_expr ')'    { *$3 = asinh(*$3);     $$=$3; }
              | TK_ATANH '(' const_expr ')'    { *$3 = atanh(*$3);     $$=$3; }
              | '+' const_expr                 {                       $$=$2; }
              | '-' const_expr                 { *$2 = -*$2;           $$=$2; }
	          | '(' const_expr ')'		       {                       $$=$2; }
              | TK_CONSTANT                    { $$ = new P_ExprConstant(scopes.top().get_cst($1)); free($1); }
              | TK_CONSTANT '[' const_expr ']' { $$ = new P_ExprConstant((scopes.top().get_cst($1))[$3->_2int()]); 
                                                free($1); delete $3; }
              | TK_CONSTANT '[' const_expr ']' 
                            '[' const_expr ']' { $$ = new P_Constant((scopes.top().get_cst($1))[$3->_2int()][$6->_2int()]); 
                                                free($1); delete $3; delete $6; }
              | TK_FLOAT                       { $$ = new P_ExprConstant($1);       }
              | TK_INTEGER                     { $$ = new P_ExprConstant($1);       }
              | interval                       { $$ = new P_ExprConstant(*$1); delete $1;  }
              | const_vector                   { $$ = $1; }
              ;

const_vector  : '[' const_list ']'             { $$ = new P_ExprConstant(*$2); delete $2; }
              ;

const_list    : const_expr                     { $$ = new vector<P_ExprConstant>(); 
	 											 $$->push_back(*$1); delete $1; }
              | const_list ';' const_expr      { $1->push_back(*$3); delete $3; $$=$1; }
              ;
              
interval      : '[' const_expr ',' const_expr ']'    { $$=new INTERVAL($2->_2real(), $4->_2real()); 
		                                               delete $2; delete $4; }
              | '<' const_expr ',' const_expr '>'    { $$=new INTERVAL($2->_2real()-($4->_2real()/2), 
								                       $2->_2real()+($4->_2real()/2)); 
		                                               delete $2; delete $4; }
              ;

/**********************************************************************************************************************/
/*                                                EXPRESSIONS                                                         */
/**********************************************************************************************************************/

expr          : expr '+' expr	                     { $$ = &(*$1 + *$3); }
              | expr '*' expr	                     { $$ = &(*$1 * *$3); }
              | expr '-' expr	                     { $$ = &(*$1 - *$3); }
              | expr '/' expr	                     { $$ = &(*$1 / *$3); }
              | TK_MAX '(' expr_list ')'             { $$ = &max(*$3); delete $3; }
              | TK_MIN '(' expr_list ')'             { $$ = &min(*$3); delete $3; }
              | TK_ATAN2 '(' expr ',' expr ')'       { $$ = &atan2(*$3,*$5); }
              | '-' expr                             { $$ = &(-*$2); }
              | TK_ABS  '(' expr ')'                 { $$ = &abs    (*$3); }
              | TK_SIGN '(' expr ')'                 { $$ = &sign(*$3); }
              | expr '^' TK_INTEGER	                 { $$ = &pow(*$1, $3); }
              | TK_SQRT '(' expr ')'                 { $$ = &sqrt (*$3); }
              | TK_EXPO '(' expr ')'                 { $$ = &exp  (*$3); }
              | TK_LOG '(' expr ')'                  { $$ = &ln   (*$3); }
              | TK_COS '(' expr ')'                  { $$ = &cos  (*$3); }
              | TK_SIN '(' expr ')'                  { $$ = &sin  (*$3); }
              | TK_TAN '(' expr ')'                  { $$ = &tan  (*$3); }
              | TK_ACOS '(' expr ')'                 { $$ = &acos (*$3); }
              | TK_ASIN '(' expr ')'                 { $$ = &asin (*$3); }
              | TK_ATAN '(' expr ')'                 { $$ = &atan (*$3); }
              | TK_COSH '(' expr ')'                 { $$ = &cosh (*$3); }
              | TK_SINH '(' expr ')'                 { $$ = &sinh (*$3); }
              | TK_TANH '(' expr ')'                 { $$ = &tanh (*$3); }
              | TK_ACOSH '(' expr ')'                { $$ = &acosh(*$3); }
              | TK_ASINH '(' expr ')'                { $$ = &asinh(*$3); }
              | TK_ATANH '(' expr ')'                { $$ = &atanh(*$3); }
              | '+' expr                             { $$ = $2; }
              | '(' expr ')'		                 { $$ = $2; }
              | TK_SYMBOL                            { $$ = scope.get_symbol($1); free($1); }
              | TK_FUNC '(' expr_list ')'            { $$ = scope.get_func($1)(*$3); free($1); }
              | TK_FLOAT                             { $$ = new P_ExprConstant($1); }
              | TK_INTEGER                           { $$ = new P_ExprConstant($1); }
              | interval                             { $$ = new P_ExprConstant($1); delete $1; }
              | const_vector                         { $$ = $1; }
              | expr '[' expr ']'                    { $$ = new P_ExprIndexExpr(*$3); }
	      	  ;
	      
expr_list     : expr_list ',' expr                   { $1->push_back($3); $$=$1; }
              | expr                                 { $$ = new vector<const ExprNode*>; 
              										   $$->push_back($1); }
              ;