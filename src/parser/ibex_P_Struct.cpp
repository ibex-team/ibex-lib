//============================================================================
//                                  I B E X                                   
// File        : ibex_P_Struct.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 9, 2012
// Last Update : Nov 3, 2019
//============================================================================

#include "ibex_P_Struct.h"
#include "ibex_P_SysGenerator.h"
#include "ibex_ExprCopy.h"
#include "ibex_SyntaxError.h"
#include "ibex_String.h"

#include <sstream>

using namespace std;

extern char* ibextext;
extern int ibex_lineno;

// note: do not confuse with ibex_error in tools/ibex_Exception.h
void ibexerror (const string& msg) {
	throw ibex::SyntaxError(msg, ibextext, ibex_lineno);
}


namespace ibex {
namespace parser {

P_Struct* pstruct;

void init_symbol_domain(const char* destname, Domain& dest, const Domain& src) {
	if (src.dim==dest.dim) {
		dest=src;
	} else if (src.dim.is_scalar()) {
		// when a vector/matrix is initialized with a single interval
		const Interval& x=src.i();
		switch(dest.dim.type()) {
		case Dim::SCALAR :      dest.i()=x; break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   dest.v().init(x); break;
		case Dim::MATRIX:       dest.m().init(x); break;
		}
	} else {
		stringstream s;
		s << "Symbol \"" << destname << "\"";

		if (dest.dim.type()==Dim::ROW_VECTOR && src.dim.type()==Dim::COL_VECTOR && dest.dim.vec_size()==src.dim.vec_size()) {
			s << " is a column vector and is initialized with a row vector";
			s << " (you have probably used \",\" instead of \";\" in the constant vector)";
			ibexerror(s.str());
		}
		else {
			s << " is not initialized correctly (dimensions do not match)";
			ibexerror(s.str());
		}
	}
}


void init_function_by_copy(Function& dest, const Function& src) {
	Array<const ExprSymbol> x(src.nb_arg());
	varcopy(src.args(),x);
	const ExprNode& y=ExprCopy().copy(src.args(),x,src.expr());
	dest.init(x,y,src.name);
}

P_Struct::P_Struct() : func_scope(NULL), current_scope(&scopes) { }

P_Struct::~P_Struct() {
	// destructor of P_Scope will destroy pending scopes in case of parsing error
}

P_Scope& P_Struct::scope() {
	return *current_scope;
}

void P_Struct::begin() {
	ibex_lineno=-1;
	if (!setlocale(LC_NUMERIC, "C")) // to accept the dot (instead of the french coma) with numeric numbers
		ibexerror("platform does not support \"C\" locale");

	ibex_lineno=1;
}

void P_Struct::begin_function() {
	func_scope = new P_Scope(scopes);
	current_scope = func_scope;
}

void P_Struct::end() {
	scopes.pop();
}

void P_Struct::end_function(const char* name, const P_ExprNode& y) {
	Function* f=new Function(scope().var_symbols(),y.generate(),name);
	delete func_scope;
	func_scope = NULL;
	current_scope = &scopes;

	scopes.add_func(name,f);
	source.func.push_back(f);
}

//---------------------------------------------------------------------------------------------------
P_StructSystem::P_StructSystem(System& sys, int simpl_level) : system(sys), simpl_level(simpl_level) {

}

void P_StructSystem::begin() {
	P_Struct::begin();
}

void P_StructSystem::end() {

	if (source.goal==NULL && source.ctrs==NULL) {
		// ex: someone tries to load system from a file containing just a function
		throw SyntaxError("not a system");
	}

	P_SysGenerator(scope()).generate(source,system,simpl_level);

	// TODO: we have to cleanup the data in case of Syntax Error
	// this probably requires garbage collector during parsing

	P_Struct::end();
}

//---------------------------------------------------------------------------------------------------
P_StructFunction::P_StructFunction(Function& f) : function(f) {

}

void P_StructFunction::begin() {
	P_Struct::begin();
}

void P_StructFunction::end() {

	if (source.goal!=NULL || source.ctrs!=NULL ) {
		throw SyntaxError("unexpected (global) variable declaration for a function");
	}
	if (source.func.empty()) {
		throw SyntaxError("no function declared in file");
	}
	init_function_by_copy(function, *source.func[0]);

	// TODO: see end_system()
	P_Struct::end();
}

//---------------------------------------------------------------------------------------------------
P_StructChoco::P_StructChoco(System& sys, int simpl_level) : system(sys), simpl_level(simpl_level) {

}

void P_StructChoco::begin() {
	P_Struct::begin();

	// ----- generate all the variables {i} -----
	Dim dim=Dim::scalar();
	Domain x(dim);
	x.i()=Interval::all_reals();
	for (int i=0; i<system.nb_var; i++) {
		char* name=append_index("\0",'{','}',i);
		scope().add_var(name,&dim,x);
		free(name);
	}
}

void P_StructChoco::end() {
	if (source.ctrs==NULL) {
		// someone tries to load a Function from a file with CHOCO constraint syntax
		throw SyntaxError("constraint declaration required");
	}

	P_SysGenerator(scope()).generate(source, system, simpl_level);

	// TODO: see end_system()
	P_Struct::end();
}


} // end namespace parser

} // end namespace ibex
