
%{
/*---------------------------------------------------------------------------------
 * 
 * Yacc/Bison input for QUIMPER/Ibex parser
 * ----------------------------------------
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

#include <math.h>
#include <vector>

#include "Interval.h"
#include "Functions.h"

#include "IbexEnv.h"
#include "IbexExpr.h"
#include "IbexSystemFactory.h"
#include "IbexSpace.h"
#include "IbexIdTable.h"
#include "IbexFunction.h"
#include "IbexConstraint.h"

#include "ParserExpr.h"
#include "ParserFunction.h"
#include "ParserBlock.h"
#include "ParserConstraint.h"
#include "ParserContractor.h"
#include "ParserPrinter.h"

#include "Generator.h"

#define DEFAULT_SPLIT_VALUE 10
#define ALL_REALS INTERVAL(BiasNegInf, BiasPosInf)

extern int ibexlex(void);
extern char* ibextext;
extern int ibex_lineno;

void ibexerror (const std::string& msg) {
  throw ibex::SyntaxErrorException(msg, ibextext, ibex_lineno);
}
   
namespace ibex { 
namespace parser {


/*-----------------------------------------------------------------------------------*/
/* conventions for memory:
   ----------------------
   - All the *ibex* objects (Expr, Constraint, Contractor, vectors of constraints and
   vectors of contractors) are always passed by reference to parser-level constructors
   and never disallocated. *Parser* objects (all the class names prefixed by "P_", such
   as "P_Constraint") are also passed by reference but *disallocated* by the destructor
   of the object being built.
   - parser_env and parser_space must be disallocated externally. In case of parser failure
   one or both may have not been built, which is indicated by the NULL pointer value.
   - Objects of type "Expr" and "Constraint" are added to parser_env and thus
   are disallocated automatically with the latter.
   - Objects of type "Contractor" are all in the main_ctc_list and must be disallocated
   externally as well (as main_ctr_list and main_ctc_list themselves), except in case of 
   parsing failure (in which case, their value is NULL).
  -----------------------------------------------------------------------------------*/

const char* DEFAULT_CTR_LIST_NAME="constraints";

stack<Scope>   scopes;


/* The four following objects are allocated by the parser (except 
 * if an error occurs).
 * See ibex::CSP(const* filename) for an example of disallocation */
 
Env*           parser_env;
Space*         parser_space;

const vector<const Constraint*>* main_ctr_list;             // set by Scope::build_main_lists_and_clean()
vector<pair<const char*,const Contractor*> >* main_ctc_list;// set by Scope::build_main_lists_and_clean()

void init() {
  ibex_lineno=-1;
  if (!setlocale(LC_NUMERIC, "C")) // to accept the dot (instead of the french coma) with numeric numbers
    ibexerror("platform does not support \"C\" locale");
  
  ibex_lineno=1;
  parser_env = new Env();
  parser_space = NULL; // to memorize that it is not built yet
  main_ctr_list = NULL;
  main_ctc_list = NULL;
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
  bool      _bool;
  REAL      real;
  INTERVAL* itv;
  vector<const char*>*                   strvec;

  struct ibex::Dim*                      dim;
  const ibex::parser::P_ExtendedSymbol*  extsbl;

  vector<const ibex::parser::P_Assign*>* assvec;
  const ibex::parser::P_Assign*          ass;

  ibex::parser::P_List<ibex::parser::P_Ctc>* _ctclst;
  ibex::parser::P_BlkList<ibex::parser::P_Ctc>*   _ctcblklst;
  const ibex::parser::P_Blk<ibex::parser::P_Ctc>* _ctcblk;
  const ibex::parser::P_Ctc*                      _ctc;

  ibex::parser::P_List<ibex::parser::P_Ctr>* _ctrlst;
  ibex::parser::P_BlkList<ibex::parser::P_Ctr>*   _ctrblklst;
  const ibex::parser::P_Blk<ibex::parser::P_Ctr>* _ctrblk;
  const ibex::parser::P_Ctr*                      _ctr;
//  const ibex::parser::P_Equality*                 _equ;

  vector<ibex::parser::P_Constant>*      cstvec;
  ibex::parser::P_Constant*              cst;

  const ibex::parser::P_Expr*            _expr;
  vector<const ibex::parser::P_Expr*>*   exprvec;
  ibex::parser::P_IterExpr*              iter;

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
%token TK_IN TK_NOT_IN TK_OR TK_MIN TK_MAX TK_INF TK_MID TK_SUP TK_SIGN TK_ABS
%token TK_SQRT TK_EXPO  TK_LOG 
%token TK_COS  TK_SIN   TK_TAN  TK_ARCCOS  TK_ARCSIN  TK_ARCTAN TK_ARCTAN2
%token TK_COSH TK_SINH  TK_TANH TK_ARCCOSH TK_ARCSINH TK_ARCTANH
%token TK_LEQ  TK_GEQ   TK_EQU  TK_ASSIGN

%token TK_BEGIN TK_END TK_FOR TK_FROM TK_TO

%token TK_COLUMN TK_TRANSPOSE TK_ATMOST_NVECTOR
%token TK_CTRS TK_CONTRACTOR TK_CONTRACTOR_LIST TK_CONSTRAINT TK_CONSTRAINT_LIST

%token TK_PROPAG TK_SHAVE TK_NEWTON TK_BOX_NARROW TK_PRECISION TK_Q_INTER TK_LOAD
%token TK_UNION TK_INTERSEC TK_PROJ_UNION TK_PROJ_INTERSEC
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

/* contractors */
%type<_ctclst>    ctc_list
%type<_ctcblklst> ctc_blk_list
%type<_ctcblklst> ctc_blk_list_ // ctc_blk_list without ending semicolon
%type<_ctcblk>    ctc_blk
%type<_ctcblk>    ctc_loop
%type<_ctc>       ctc
%type<_ctc>       ctc_op_loop
%type<_bool>      logic_op

/* constraints */
%type<_ctrlst>    ctr_list
%type<_ctrblklst> ctr_blk_list
%type<_ctrblklst> ctr_blk_list_ // ctr_blk_list without ending semicolon
%type<_ctrblk>    ctr_blk
%type<_ctrblk>    ctr_loop
%type<str>        ctr_list_head
%type<_ctr>       ctr
%type<_ctr>       ctr_         // ctr without surrounding parenthesis
%type<_ctr>       arith_ctr_ 
//%type<_equ>       equality
//%type<_equ>       equality_    // equality without surrounding parenthesis
 
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

program       :                                      { init(); }
                decl_opt_cst                           
                TK_VARS                              { scopes.top().set_entity_type(IBEX_VAR); }
	        decl_ent_list ';'   
                decl_opt_par                         { parser_space = scopes.top().build_space(*parser_env); }
                decl_fnc_list
	        decls                                { scopes.top().build_main_lists_and_clean();
						       scopes.pop(); }
              ;

decl_opt_cst  : 
              | TK_CONST decl_cst_list ';'
	      ;

decl_cst_list : decl_cst                                    
              | decl_cst_list ';' decl_cst
              ;

decl_cst      : TK_NEW_SYMBOL TK_EQU const_expr      { scopes.top().add_cst($1, *$3); free($1); delete $3; }
              | TK_NEW_SYMBOL TK_IN const_expr       { scopes.top().add_cst($1, *$3); free($1); delete $3; }
              ;

decl_opt_par  : 
              | TK_PARAM decl_par_list ';'
              ; 
 
decl_par_list : decl_par                                    
              | decl_par_list ';' decl_par
              | decl_par_list ',' decl_par
              ;

decl_par      : '!'                                  { scopes.top().set_entity_type(IBEX_EPR); } 
                decl_ent 
	      | '*'                                  { scopes.top().set_entity_type(IBEX_UPR); } 
                decl_ent 
	      |                                      { scopes.top().set_entity_type(IBEX_SYB); } 
                decl_ent 
              ;

decl_ent_list : decl_ent 
              | decl_ent_list ';' decl_ent
              | decl_ent_list ',' decl_ent
              ;

decl_ent      : TK_NEW_SYMBOL dimension              { scopes.top().add_entity($1, *$2, ALL_REALS); 
		                                       // it is more convenient to add entities now in the environment 
		                                       // so that they appear in the same order as in the system.
		                                       parser_env->add_symbol($1, *$2); 
		                                       delete $2; free($1); }
              | TK_NEW_SYMBOL dimension 
	        TK_IN const_expr                     { scopes.top().add_entity($1, *$2, *$4); 
		                                       parser_env->add_symbol($1, *$2); 
						       free($1); delete $2; delete $4; }
              ; 


dimension     :                                      { $$=new Dim(0,0,0); }
              | '[' const_expr ']'                   { $$=new Dim(0,0,$2->_2int()); delete $2; }
              | '[' const_expr ']' '[' const_expr ']'{ $$=new Dim(0,$2->_2int(),$5->_2int()); delete $2; delete $5; }
              | '[' const_expr ']' '[' const_expr ']'
 	        '[' const_expr ']'                   { $$=new Dim($2->_2int(),$5->_2int(),$8->_2int()); 
		                                       delete $2; delete $5; delete $8; }
	      ;

identifier    : TK_ENTITY                            { $$ = new P_Symbol($1); free($1); }
              | TK_INPUT                             { $$ = new P_Symbol($1); free($1); }
              | TK_TMP_SYMBOL                        { $$ = new P_Symbol($1); free($1); }
              | TK_OUTPUT                            { $$ = new P_Symbol($1); free($1); }
              | TK_CONSTANT                          { $$ = new P_Symbol($1); free($1); }
              | identifier '[' iter_expr ']'         { $$ = new P_SymboWithIterIndex(*$1 ,*$3); }
              ;


 /**********************************************************************************************************************/
/*                                                FUNCTIONS                                                           */
/**********************************************************************************************************************/

decl_fnc_list : decl_fnc_list decl_fnc 
              | 
              ;

decl_fnc      : TK_FUNCTION                          { scopes.push(Scope(scopes.top(),TK_CONSTANT)); }
                TK_NEW_SYMBOL dimension              { scopes.top().add_func_output($3,*$4); delete $4; }
		TK_EQU TK_NEW_SYMBOL
                '(' fnc_inpt_list ')'
		fnc_code
		TK_END                               { P_Function f($7, $9, $3, $11); free($3);
                                        	       parser_env->add_function(FuncGenerator().generate(f));
						       scopes.pop(); scopes.top().add_func($7); free($7); }
              ;

fnc_inpt_list : fnc_inpt_list ',' fnc_input          { $1->push_back(strdup($3)); free($3); $$=$1; }
              | fnc_input                            { $$=new vector<const char*>(); $$->push_back(strdup($1)); free($1);}
              ;

fnc_input     : TK_NEW_SYMBOL dimension              { scopes.top().add_func_input($1,*$2); $$=$1; delete $2; }
              ;

fnc_code      : fnc_code fnc_assign ';'              { $1->push_back($2); $$=$1; }
              | fnc_assign ';'                       { $$ = new vector<const P_Assign*>(); $$->push_back($1); }
              ;

fnc_assign    : fnc_ass_left TK_EQU expr             { $$ = new P_Assign(*$1,*$3); }
              ;

fnc_ass_left  : identifier                           { $$ = $1; }
              | TK_NEW_SYMBOL                        { scopes.top().add_func_tmp_symbol($1); 
		                                       $$ = new P_Symbol($1); free($1); }
              ;

/**********************************************************************************************************************/

decls         : decls decl
              | decl
              ;

decl          : decl_ctc_list
              | decl_ctc
              | decl_ctr_list
              | decl_ctr
              ;

/**********************************************************************************************************************/
/*                                                CONTRACTORS                                                         */
/**********************************************************************************************************************/

decl_ctc_list : TK_CONTRACTOR_LIST TK_NEW_SYMBOL
                ctc_blk_list TK_END                  { vector<const Contractor*>* vec=new vector<const Contractor*>();
		                                       ListGenerator<P_Ctc,Contractor> g(*parser_env, *vec);
						       g.visit(*$3);
						       delete $3;
						       scopes.top().add_ctc_list($2, *vec);
						       free($2); }
              ;

ctc_blk_list  : ctc_blk_list_ semi_col_opt           { $$ = $1; }
              ;

ctc_blk_list_ : ctc_blk_list_ ';' ctc_blk            { $1->add(*$3); $$ = $1; }
              | ctc_blk                              { $$ = new P_BlkList<P_Ctc>(*$1); }
              ;

ctc_list      : TK_CTC_LIST                          { $$ = new P_BlkListRef<P_Ctc>($1); free($1); }
              | ctc_blk_list                         { $$ = $1; }
              ;

ctc_blk       : ctc_loop                             { $$ = $1; }
              | ctc                                  { $$ = $1; }
              ;

ctc_loop      : TK_FOR TK_NEW_SYMBOL TK_EQU
		const_expr ':' const_expr ';'        
						     { scopes.push(scopes.top());
						       scopes.top().add_iterator($2, $4->_2int(), $6->_2int()); }
                ctc_blk_list TK_END                  { $$ = new P_Loop<P_Ctc>($2, $4->_2int(), $6->_2int(), *$9); 
		                                       scopes.pop();
		                                       free($2); delete $4; delete $6; }
              ;

decl_ctc      : TK_CONTRACTOR TK_NEW_SYMBOL 
                ctc semi_col_opt TK_END              { CtcGenerator g(*parser_env);
						       const Contractor& new_ctc = g.generate(*$3);
						       delete $3;
		                                       //cout << "Contractor " << $3 << endl << new_ctc << endl;
						       scopes.top().add_ctc($2, &new_ctc); free($2); } 
              ;

semi_col_opt  : ';' 
              | 
              ;

ctc           : ctr_                                 { $$ = new P_HC4Revise(*$1); }
              | TK_CTC                               { $$ = new P_CtcRef($1); free($1); }
              | TK_CTC_LIST '(' iter_expr ')'        { $$ = new P_CtcListElemRef($1,*$3); free($1); }
              | TK_PROJ_UNION '(' ctc ',' 
	          identifier ')'                     { $$ = new P_ParamSplit(*$3,*$5,DEFAULT_SPLIT_VALUE,false); }
              | TK_PROJ_UNION '(' ctc ',' 
	          identifier ',' TK_INTEGER ')'      { $$ = new P_ParamSplit(*$3,*$5,$7,false); }
              | TK_PROJ_INTERSEC '(' ctc ',' 
	          identifier ')'                     { $$ = new P_ParamSplit(*$3,*$5,DEFAULT_SPLIT_VALUE,true); }
              | TK_PROJ_INTERSEC '(' ctc ',' 
                  identifier ',' TK_INTEGER ')'      { $$ = new P_ParamSplit(*$3,*$5,$7,true); }
              | TK_PROPAG '(' ctc_list ')'             
                                                     { $$ = new P_Propag(*$3); }
              | TK_PROPAG '(' ctc_list ',' 
	           const_expr  ')'                   { $$ = new P_Propag(*$3, $5->_2real()); delete $5; }
              | TK_PROPAG '(' ctc_list ',' 
                   const_expr ',' TK_BOOL  ')'       { $$ = new P_Propag(*$3, $5->_2real(), $7); delete $5; }
              | TK_SHAVE '(' ctc ')'              
                                                     { $$ = new P_Shave(*$3); }
              | TK_SHAVE '(' ctc ',' const_expr ')'              
                                                     { $$ = new P_Shave(*$3, $5->_2real()); delete $5;}
              | TK_SHAVE '(' ctc ',' const_expr ',' 
                   const_expr ')'                    { $$ = new P_Shave(*$3, $5->_2real(), $7->_2real()); 
		                                       delete $5; delete $7; }
              | TK_NEWTON '(' ctr_list ')'             
                                                     { $$ = new P_Newton(*$3); }
              | TK_NEWTON '(' ctr_list ',' 
                   const_expr ')'                    { $$ = new P_Newton(*$3, $5->_2real()); delete $5;}
              | TK_NEWTON '(' ctr_list ',' 
                   const_expr ',' const_expr ')'     { $$ = new P_Newton(*$3, $5->_2real(), $7->_2real()); 
		                                       delete $5; delete $7; }
              | TK_NEWTON '(' ctr_list ',' 
                   const_expr ',' const_expr ',' 
                   const_expr ')'                    { $$ = new P_Newton(*$3, $5->_2real(), $7->_2real(), $9->_2real()); 
		                                       delete $5; delete $7; delete $9; }
              | TK_BOX_NARROW '(' ctr ')'            { $$ = new P_BoxNarrow(*$3); }
              | TK_BOX_NARROW '(' ctr ',' 
                   const_expr ')'                    { $$ = new P_BoxNarrow(*$3, $5->_2real()); delete $5;}
              | TK_BOX_NARROW '(' ctr ',' 
                   const_expr ',' const_expr ')'     { $$ = new P_BoxNarrow(*$3, $5->_2real(), $7->_2real()); 
                                                       delete $5; delete $7; }
              | TK_PRECISION '(' const_expr ')'      { $$ = new P_Precision($3->_2real()); delete $3; }
              | ctc TK_INTERSEC ctc                  { $$ = new P_LogicOp(*$1, *$3, true); }
              | ctc TK_UNION ctc                     { $$ = new P_LogicOp(*$1, *$3, false); }
	      | ctc_op_loop                          { $$=$1; }
              | '(' ctc ')'                          { $$ = $2; }
              | TK_Q_INTER '(' identifier ','
	         identifier ',' const_expr ')'       { $$ = new P_QInter(*$3,*$5, $7->_2int()); delete $7;}
              | TK_LOAD '(' TK_STRING ',' 
                            identifier ')'           { $$ = new P_Load($3,*$5); }
              ;

ctc_op_loop   : logic_op TK_NEW_SYMBOL TK_EQU
		const_expr ':' const_expr ';'        { scopes.push(scopes.top());
						       scopes.top().add_iterator($2,$4->_2int(),$6->_2int()); }
                ctc_list TK_END                      { $$ = new P_LogicOpLoop($2, $4->_2int(), $6->_2int(), *$9, $1); 
		                                       scopes.pop();
						       free($2); delete $4; delete $6; }
              ;

logic_op      : TK_INTERSEC                          { $$=true; }
              | TK_UNION                             { $$=false; }
              ;


/**********************************************************************************************************************/

/*                                                CONSTRAINTS                                                         */
/**********************************************************************************************************************/

decl_ctr_list : ctr_list_head
                ctr_blk_list TK_END                  { vector<const Constraint*>* vec = new vector<const Constraint*>();
		                                       ListGenerator<P_Ctr,Constraint> g(*parser_env, *vec);
						       g.visit(*$2);
						       delete $2;
						       scopes.top().add_ctr_list($1, *vec);
						       free($1); }
              ;

ctr_list_head : TK_CONSTRAINT_LIST TK_NEW_SYMBOL     { $$ = $2; }
	      | TK_CTRS                              { $$ = strdup(DEFAULT_CTR_LIST_NAME); }
              ;

ctr_blk_list  : ctr_blk_list_ semi_col_opt           { $$ = $1; }
              ;

ctr_blk_list_ : ctr_blk_list_ ';' ctr_blk            { $1->add(*$3); $$ = $1; }
              | ctr_blk                              { $$ = new P_BlkList<P_Ctr>(*$1); }
              ;

ctr_list      : TK_CTR_LIST                          { $$ = new P_BlkListRef<P_Ctr>($1); free($1); }
              | ctr_blk_list                         { $$ = $1; }
              ;

ctr_blk       : ctr_loop                             { $$ = $1; }
              | ctr                                  { $$ = $1; }
              ;

ctr_loop      : TK_FOR TK_NEW_SYMBOL TK_EQU
		const_expr ':' const_expr ';'        { scopes.push(scopes.top());
						       scopes.top().add_iterator($2, $4->_2int(), $6->_2int()); }
                ctr_blk_list TK_END                  { $$ = new P_Loop<P_Ctr>($2, $4->_2int(), $6->_2int(), *$9); 
						       scopes.pop();
		                                       free($2); delete $4; delete $6; }
              ;

decl_ctr      : TK_CONSTRAINT TK_NEW_SYMBOL 
                ctr semi_col_opt TK_END              { CtrGenerator g(*parser_env);
						       const Constraint& new_ctr = g.generate(*$3);
						       delete $3;
						       scopes.top().add_ctr($2, &new_ctr); free($2); } 
              ;


ctr           : ctr_                                 { $$ = $1; }
              | '(' ctr ')'                          { $$ = $2; }

ctr_          : arith_ctr_                           { $$ = $1; }
              | expr TK_NOT_IN expr                  { $$ = new P_NotIn(*$1,*$3); }
              | TK_ATMOST_NVECTOR '(' '{' 
                expr_list '}' ',' const_expr ')'     { $$ = new P_AtMostNVector(*$4, $7->_2int()); delete $7;}
              | TK_CTR                               { $$ = new P_CtrRef($1); free($1); }
              | TK_CTRS '(' iter_expr ')'            { $$ = new P_CtrListElemRef(DEFAULT_CTR_LIST_NAME,*$3); }
              | TK_CTR_LIST '(' iter_expr ')'        { $$ = new P_CtrListElemRef($1,*$3); free($1); }
              ;

arith_ctr_    : expr TK_EQU expr                     { $$ = &(*$1 =*$3); }
              | expr TK_IN  expr                     { $$ = &(*$1 =*$3); }
              | expr TK_LEQ expr                     { $$ = &(*$1<=*$3); }
              | expr TK_GEQ expr                     { $$ = &(*$1>=*$3); }
              | expr   '<'  expr                     { $$ = &(*$1< *$3); }
              | expr   '>'  expr                     { $$ = &(*$1> *$3); }
              ; 

/* equality rule is incomplete: should also generate P_EquRef and P_CtrListEquElemRef objects 
   (to be used e.g. by BoxNarrow)

equality      : equality_                            { $$ = $1; }
              | '(' equality ')'                     { $$ = $2; }
              ;

equality_     : expr TK_EQU expr                     { $$ = &(*$1 =*$3); }
; */

/**********************************************************************************************************************/
/*                                      CONSTANT EXPRESSIONS (evaluated on-the-fly)                                   */
/**********************************************************************************************************************/

const_expr    : const_expr  '+' const_expr	     { *$1 = *$1 + *$3;      $$=$1; delete $3; }
	      | const_expr  '-' const_expr	     { *$1 = *$1 - *$3;      $$=$1; delete $3; }
	      | const_expr  '*' const_expr	     { *$1 = *$1 * *$3;      $$=$1; delete $3; }
              | const_expr  '/' const_expr	     { *$1 = *$1 / *$3;      $$=$1; delete $3; }
              | const_expr  '^' const_expr	     { *$1 = pow (*$1,*$3);  $$=$1; delete $3; }
              | TK_ABS     '(' const_expr ')'        { *$3 = abs (*$3);      $$=$3; }
              | TK_SQRT    '(' const_expr ')'        { *$3 = sqrt (*$3);     $$=$3; }
              | TK_EXPO    '(' const_expr ')'        { *$3 = exp  (*$3);     $$=$3; }
              | TK_LOG     '(' const_expr ')'        { *$3 = log  (*$3);     $$=$3; }
              | TK_COS     '(' const_expr ')'        { *$3 = cos  (*$3);     $$=$3; }
              | TK_SIN     '(' const_expr ')'        { *$3 = sin  (*$3);     $$=$3; }
              | TK_TAN     '(' const_expr ')'        { *$3 = tan  (*$3);     $$=$3; }
              | TK_ARCCOS  '(' const_expr ')'        { *$3 = arccos (*$3);   $$=$3; }
              | TK_ARCSIN  '(' const_expr ')'        { *$3 = arcsin (*$3);   $$=$3; }
              | TK_ARCTAN  '(' const_expr ')'        { *$3 = arctan (*$3);   $$=$3; }
              | TK_ARCTAN2 '(' const_expr ',' 
	                       const_expr ')'        { *$3 = arctan2(*$3,*$5);   $$=$3; delete $5; }
              | TK_COSH    '(' const_expr ')'        { *$3 = cosh (*$3);     $$=$3; }
              | TK_SINH    '(' const_expr ')'        { *$3 = sinh (*$3);     $$=$3; }
              | TK_TANH    '(' const_expr ')'        { *$3 = tanh (*$3);     $$=$3; }
              | TK_ARCCOSH '(' const_expr ')'        { *$3 = arccosh(*$3);   $$=$3; }
              | TK_ARCSINH '(' const_expr ')'        { *$3 = arcsinh(*$3);   $$=$3; }
              | TK_ARCTANH '(' const_expr ')'        { *$3 = arctanh(*$3);   $$=$3; }
              | TK_TRANSPOSE '(' const_expr ')'      { *$3 = transpose(*$3); $$=$3; }
              | '+' const_expr                       {                       $$=$2; }
              | '-' const_expr                       { *$2 = -*$2;           $$=$2; }
	      | '(' const_expr ')'		     {                       $$=$2; }
              | TK_CONSTANT                          { $$ = new P_Constant(scopes.top().cst_value($1)); free($1); }
              | TK_CONSTANT '[' const_expr ']'       { $$ = new P_Constant((scopes.top().cst_value($1))[$3->_2int()]); 
                                                       free($1); delete $3; }
              | TK_CONSTANT '[' const_expr ']' 
                            '[' const_expr ']'       { $$ = new P_Constant((scopes.top().cst_value($1))[$3->_2int()][$6->_2int()]); 
                                                       free($1); delete $3; delete $6; }
              | TK_FLOAT                             { $$ = new P_Constant($1);       }
              | TK_INTEGER                           { $$ = new P_Constant($1);       }
              | interval                             { $$ = new P_Constant(*$1); delete $1;  }
              | const_vector                         { $$ = $1; }
              ;

const_vector  : '[' const_list ']'                   { $$ = new P_Constant(*$2); delete $2; }
              ;

const_list    : const_expr                           { $$ = new vector<P_Constant>(); $$->push_back(*$1); delete $1; }
              | const_list ';' const_expr            { $1->push_back(*$3); delete $3; $$=$1; }
              ;

iter_expr     : iter_expr  '+' iter_expr	     { $$ = &(*$1 + *$3); }
	      | iter_expr  '-' iter_expr	     { $$ = &(*$1 - *$3); }
	      | iter_expr  '*' iter_expr	     { $$ = &(*$1 * *$3); }
	      | '+' iter_expr 		             { $$ = $2; }
	      | '(' iter_expr ')'		     { $$ = $2; }
	      | TK_ITERATOR                          { $$ = new P_IterSymbol($1); free($1); }
              | TK_INTEGER                           { $$ = new P_IterCst($1); }
	      | TK_CONSTANT                          { $$ = new P_IterCst(scopes.top().cst_value($1)._2int()); free($1); }
              | TK_CONSTANT '[' iter_expr ']'        { $$ = new P_IterCstSymbolWithIndex(scopes.top().cst_value($1), *$3); free($1); }
              ;

/* note : il (upper bound < lower bound), Bias/Profil will swap them !*/
interval      : '[' const_expr ',' const_expr ']'    { $$=new INTERVAL($2->_2real(), $4->_2real()); 
		                                       delete $2; delete $4;}
              | '<' const_expr ',' const_expr '>'    { $$=new INTERVAL($2->_2real()-($4->_2real()/2), 
								       $2->_2real()+($4->_2real()/2)); 
		                                       delete $2; delete $4;}
              ;

/**********************************************************************************************************************/
/*                                                EXPRESSIONS                                                         */
/**********************************************************************************************************************/

expr          : expr '+' expr	                     { $$ = &(*$1 + *$3); }
              | expr '-' expr	                     { $$ = &(*$1 - *$3); }
	      | expr '*' expr	                     { $$ = &(*$1 * *$3); }
              | expr '/' expr	                     { $$ = &(*$1 / *$3); }
              | expr '^' TK_INTEGER	             { $$ = &pow(*$1, $3); }
              | TK_ABS  '(' expr ')'                 { $$ = &abs    (*$3); }
              | TK_SQRT '(' expr ')'                 { $$ = &sqrt   (*$3); }
              | TK_EXPO '(' expr ')'                 { $$ = &exp    (*$3); }
              | TK_LOG '(' expr ')'                  { $$ = &ln     (*$3); }
              | TK_COS '(' expr ')'                  { $$ = &cos    (*$3); }
              | TK_SIN '(' expr ')'                  { $$ = &sin    (*$3); }
              | TK_TAN '(' expr ')'                  { $$ = &tan    (*$3); }
              | TK_ARCCOS '(' expr ')'               { $$ = &arccos (*$3); }
              | TK_ARCSIN '(' expr ')'               { $$ = &arcsin (*$3); }
              | TK_ARCTAN '(' expr ')'               { $$ = &arctan (*$3); }
              | TK_ARCTAN2 '(' expr ',' expr ')'     { $$ = &arctan2(*$3,*$5); }
              | TK_COSH '(' expr ')'                 { $$ = &cosh   (*$3); }
              | TK_SINH '(' expr ')'                 { $$ = &sinh   (*$3); }
              | TK_TANH '(' expr ')'                 { $$ = &tanh   (*$3); }
              | TK_ARCCOSH '(' expr ')'              { $$ = &arccosh(*$3); }
              | TK_ARCSINH '(' expr ')'              { $$ = &arcsinh(*$3); }
              | TK_ARCTANH '(' expr ')'              { $$ = &arctanh(*$3); }
              | TK_TRANSPOSE '(' expr ')'            { $$ = &transpose(*$3); }
              | '+' expr                             { $$ = $2; }
              | '-' expr                             { $$ = &(-*$2); }
	      | '(' expr ')'		             { $$ = $2; }
              | TK_SIGN '(' expr ')'                 { $$ = &sign(*$3); }
              | identifier                           { $$ = $1; }
              | TK_MAX '(' expr_list ')'             { $$ = &max(*$3); delete $3; }
              | TK_MIN '(' expr_list ')'             { $$ = &min(*$3); delete $3; }
              | TK_INF '(' expr ')'                  { $$ = &inf(*$3); }
              | TK_MID '(' expr ')'                  { $$ = &mid(*$3); }
              | TK_SUP '(' expr ')'                  { $$ = &sup(*$3); }
              | TK_FUNC '(' expr_list ')'            { $$ = new P_Apply($1,$3); free($1); }
/*               | TK_FUNC '(' ')'                { $$ = &P_Apply::new_($1,vector<const P_Expr*>()); free($1); } */
              | TK_ITERATOR                          { $$ = new P_IterSymbol($1); free($1); }
              | TK_FLOAT                             { $$ = &((new P_Constant($1))->_2expr()); }
              | TK_INTEGER                           { $$ = &((new P_Constant($1))->_2expr()); }
              | interval                             { $$ = &((new P_Constant(*$1))->_2expr()); delete $1; }
              | const_vector                         { $$ = &($1->_2expr()); }
	      ;

expr_list     : expr_list ',' expr                   { $1->push_back($3); $$=$1; }
              | expr                                 { $$ = new vector<const P_Expr*>; $$->push_back($1); }
              ;

