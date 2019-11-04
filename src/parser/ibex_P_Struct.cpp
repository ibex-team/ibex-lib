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

P_Struct::~P_Struct() {
	// pop pending scopes in case of parsing error
	while (!scopes.empty()) scopes.pop();
}

void P_Struct::begin() {
	ibex_lineno=-1;
	if (!setlocale(LC_NUMERIC, "C")) // to accept the dot (instead of the french coma) with numeric numbers
		ibexerror("platform does not support \"C\" locale");

	ibex_lineno=1;

	// Bottom scope.
	// Kept for both parsing and generation (the constants are shared by the two steps)
	scopes.push(P_Scope());

	// note: a second scope is created for parsing the constraints block,
	// to store temporary variables.
}

void P_Struct::end() {
	scopes.pop();
}

//---------------------------------------------------------------------------------------------------
P_StructSystem::P_StructSystem(System& sys) : system(sys) {

}

P_Struct::ResultType P_StructSystem::type() const {
	return SYSTEM;
}

void P_StructSystem::begin() {
	P_Struct::begin();
}

void P_StructSystem::end() {

	if (source.goal==NULL && source.ctrs==NULL) {
		// ex: someone tries to load system from a file containing just a function
		throw SyntaxError("not a system");
	}

	P_SysGenerator(scopes).generate(source,system);

	// TODO: we have to cleanup the data in case of Syntax Error
	// this probably requires garbage collector during parsing

	P_Struct::end();
}

//---------------------------------------------------------------------------------------------------
P_StructFunction::P_StructFunction(Function& f) : function(f) {

}

P_Struct::ResultType P_StructFunction::type() const {
	return FUNCTION;
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
	const Function& f=(*source.func[0]);
	Array<const ExprSymbol> x(f.nb_arg());
	varcopy(f.args(),x);
	const ExprNode& y=ExprCopy().copy(f.args(),x,f.expr());

	function.init(x,y,f.name);

	// TODO: see end_system()
	P_Struct::end();
}

//---------------------------------------------------------------------------------------------------
P_StructChoco::P_StructChoco(System& sys) : system(sys) {

}

P_Struct::ResultType P_StructChoco::type() const {
	return CHOCO;
}

void P_StructChoco::begin() {
	P_Struct::begin();

	// ----- generate all the variables {i} -----
	Dim dim=Dim::scalar();
	Domain x(dim);
	x.i()=Interval::all_reals();
	for (int i=0; i<system.nb_var; i++) {
		char* name=append_index("\0",'{','}',i);
		scopes.top().add_var(name,&dim,x);
		free(name);
	}
}

void P_StructChoco::end() {
	if (source.ctrs==NULL) {
		// someone tries to load a Function from a file with CHOCO constraint syntax
		throw SyntaxError("constraint declaration required");
	}

	P_SysGenerator(scopes).generate(source,system);

	// TODO: see end_system()
	P_Struct::end();
}

} // end namespace parser

} // end namespace ibex
