/* ============================================================================
 * I B E X - Functions
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * Last Update : Nov 22, 2017
 * ---------------------------------------------------------------------------- */

#include <sstream>

#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_ExprCopy.h"
#include "ibex_ExprDiff.h"
#include "ibex_Expr2Minibex.h"
#include "ibex_Eval.h"
#include "ibex_HC4Revise.h"
#include "ibex_Gradient.h"
#include "ibex_String.h"
#include "ibex_UnknownFileException.h"
#include "ibex_SyntaxError.h"
#include "ibex_P_Struct.h"

#ifndef _WIN32 // MinGW does not support mutex
#include <mutex>
namespace {
std::mutex mtx;
}
#define LOCK mtx.lock()
#define UNLOCK mtx.unlock()
#else
#define LOCK
#define UNLOCK
#endif

using namespace std;


extern void ibexparse_string(const char* syntax);

namespace ibex {

namespace {

const char* DIFF_PREFIX = "d"; // when the differential of a function is generated, the name is prefixed with DIFF_PREFIX

/*
 * Find the components used in the function
 * \pre the symbol keys must have been set
 */
class FindInputsUsed: public ExprVisitor<void> {
public:
	BitSet& is_used;
	int* symbol_index;
	NodeMap<bool> visited;

	FindInputsUsed(const Array<const ExprSymbol>& x, const ExprNode& y, int* symbol_index, BitSet& is_used) : is_used(is_used), symbol_index(symbol_index) {
		if (!add(y)) visit(y);
	}

	bool add_symbol(const ExprNode& e) {
		const ExprSymbol* s=dynamic_cast<const ExprSymbol*>(&e);
		if (s)  {
			int current_index = symbol_index[s->key];
			for (int i=0; i<s->dim.size(); i++) {
				is_used.add(current_index+i);
			}
			return true;
		} else
			return false;
	}

	bool add_index(const ExprNode& e) {
		const ExprIndex* idx=dynamic_cast<const ExprIndex*>(&e);
		if (idx && idx->indexed_symbol())  {
			pair<const ExprSymbol*, bool**> p = idx->symbol_mask();
			if (p.first==NULL) return false;
			const ExprSymbol& s= *p.first;
			bool** mask=p.second;
			int current_index = symbol_index[s.key];
			for (int i=0; i<s.dim.nb_rows(); i++) {
				for (int j=0; j<s.dim.nb_cols(); j++)
					if (mask[i][j])  is_used.add(current_index+(i*s.dim.nb_cols())+j);
				delete[] mask[i];
			}
			delete[] mask;
			return true;
		} else
			return false;
	}

	bool add(const ExprNode& e) {
		return add_symbol(e) || add_index(e);
	}

	virtual void visit(const ExprNode& e)     {
		if (visited.found(e)) return;
		e.accept_visitor(*this);
		visited.insert(e,true);
	}

	virtual void visit(const ExprIndex& e)    {
		// note: if called => e is not an indexed symbol
		// which means that e.expr is also not an indexed symbol
		assert(!add(e.expr));
		visit(e.expr);
	}

	virtual void visit(const ExprSymbol& e)   {	}

	virtual void visit(const ExprConstant&)   { }

	virtual void visit(const ExprNAryOp& e)   {
		for (int i=0; i<e.nb_args; i++) {
			if (!add(e.arg(i)))
				visit(e.arg(i));
		}
	}

	virtual void visit(const ExprBinaryOp& e) {
		if (!add(e.left)) visit(e.left);
		if (!add(e.right)) visit(e.right);
	}

	virtual void visit(const ExprUnaryOp& e)  {
		if (!add(e.expr)) visit(e.expr);
	}

};

const char* duplicate_or_generate(const char* name) {
	if (name) return strdup(name);
	else return ibex::next_generated_func_name();
}

}

Function::Function() : name(NULL), comp(NULL), df(NULL), _def_domain(NULL), zero(NULL),
		_eval(NULL), _hc4revise(NULL), _grad(NULL), _inhc4revise(NULL) {
	// root==NULL <=> the function is not initialized yet
}

Function::Function(const ExprSymbol& x, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19),y,name);
}

Function::Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19, const ExprSymbol& x20, const ExprNode& y, const char* name) {
	init(Array<const ExprSymbol>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20),y,name);
}

Function::Function(const Array<const ExprSymbol>& x, const ExprNode& y, const char* name) {
	init(x,y,name);
}

} // end namespace ibex

extern int ibexparse();
extern void ibexrestart(FILE *);
extern FILE* ibexin;

namespace ibex {

Function::Function(const char* x, const char* y) {
	build_from_string(Array<const char*>(x),y);
}

Function::Function(const char* x1, const char* x2, const char* y) {
	build_from_string(Array<const char*>(x1,x2),y);
}

Function::Function(const char* x1, const char* x2, const char* x3, const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3),y);
}

Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4),y);
}

Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5),y);
}

Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6),y);
}

Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7),y);
}

Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,  const char* x12,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,  const char* x12,  const char* x13,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,  const char* x12,  const char* x13,  const char* x14,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,  const char* x12,  const char* x13,  const char* x14,  const char* x15,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,  const char* x12,  const char* x13,  const char* x14,  const char* x15,  const char* x16,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,  const char* x12,  const char* x13,  const char* x14,  const char* x15,  const char* x16,  const char* x17,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,  const char* x12,  const char* x13,  const char* x14,  const char* x15,  const char* x16,  const char* x17,  const char* x18,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,  const char* x12,  const char* x13,  const char* x14,  const char* x15,  const char* x16,  const char* x17,  const char* x18,  const char* x19,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19),y);
}

Function::Function(const char* x1,  const char* x2,  const char* x3,  const char* x4,  const char* x5,  const char* x6,  const char* x7,  const char* x8,  const char* x9,  const char* x10,  const char* x11,  const char* x12,  const char* x13,  const char* x14,  const char* x15,  const char* x16,  const char* x17,  const char* x18,  const char* x19,  const char* x20,const char* y) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20),y);
}

Function::Function(int n, const char** x, const char* y) {
	Array<const char*> a(n);
	for (int i=0; i<n; i++) a.set_ref(i,x[i]);
	build_from_string(a,y);
}

void Function::build_from_string(const Array<const char*>& x, const char* y, const char* name) {

	stringstream s;

	const char* name_copy=duplicate_or_generate(name);

	s << "function " << name_copy << "(";
	for (int i=0; i<x.size(); i++) {
		s << x[i];
		if (i<x.size()-1) s << ",";
	}
	s << ")\n";
	s << "  return " << y << ";\n";
	s << "end\n";

	free((char*) name_copy);

	char* syntax = strdup(s.str().c_str());

	LOCK;
	try {
		parser::pstruct = new parser::P_StructFunction(*this);
		ibexparse_string(syntax);
		delete parser::pstruct;
		parser::pstruct = NULL;
		free(syntax);
	} catch(SyntaxError& e) {
		delete parser::pstruct;
		parser::pstruct = NULL;
		free(syntax);
		UNLOCK;
		throw e;
	}
	UNLOCK;
}

Function::Function(const char* filename) {

	FILE *fd;
	if ((fd = fopen(filename, "r")) == NULL) throw UnknownFileException(filename);

	LOCK;

	ibexrestart(fd);

	if (ibexin == fd) {
		rewind(fd);
	}

	ibexin = fd;	

	try {
		parser::pstruct = new parser::P_StructFunction(*this);
		ibexparse();
		delete parser::pstruct;
		parser::pstruct = NULL;
	}
	catch(SyntaxError& e) {
		delete parser::pstruct;
		parser::pstruct = NULL;
		fclose(fd);
		UNLOCK;
		throw e;
	}

	fclose(fd);
	UNLOCK;
}

Function::Function(FILE* fd) {

	LOCK;

	ibexin = fd;

	try {
		parser::pstruct = new parser::P_StructFunction(*this);
		ibexparse();
		delete parser::pstruct;
		parser::pstruct = NULL;
	}
	catch(SyntaxError& e) {
		delete parser::pstruct;
		parser::pstruct = NULL;
		ibexrestart(ibexin);
		UNLOCK;
		throw e;
	}
	UNLOCK;
}


Function::Function(const Function& f, copy_mode mode) {

	assert(f.cf.code!=NULL);

	// Create the new symbols
	// we have to proceed first the symbols in order to guarantee that
	// they appear in the same order in "dest". Indeed, if we add the symbols
	// as they appear in the expression the copy of a a function (x,y)->y+x
	// would yield (y,x)->y+x.
	Array<const ExprSymbol> x(f.nb_arg());
	varcopy(f.args(),x);

	// Duplicate the expression
	const ExprNode* y;
	if (mode==COPY) {
		y= & ExprCopy().copy(f.symbs,x,f.expr());
		init(x,*y,f.name);
	} else {
		char* name = (char*) malloc(strlen(f.name)+strlen(DIFF_PREFIX)+1); // +1 for null character
		strcpy((char*) name,DIFF_PREFIX);
		strcat((char*) name,f.name);
		y= & ExprDiff(f.symbs,x).diff(f.expr(),f.symbs);
		init(x,*y,name);
		free(name);
	}
}

void Function::generate_comp() {
	if (expr().type()==Dim::SCALAR) {
		comp=new Function*[1];
		comp[0]=(Function*) this; // a function cannot be modified anyway
		return;
	}

	// rem: image_dim()==expr().dim.vec_size() if expr() is a vector
	//      and also fvec->nb_args if, in addition, fvec!=NULL

	int m=_image_dim.is_vector() ? _image_dim.vec_size() : _image_dim.nb_rows();

	comp = new Function*[m];

	for (int i=0; i<m; i++) {
		Array<const ExprSymbol> x(nb_arg());
		varcopy(symbs,x);
		const ExprIndex& yi_tmp=expr()[i];
		// simplification level 1 should be enough here
		const ExprNode& yi=ExprCopy().copy(symbs, x, yi_tmp).simplify(ExprNode::default_simpl_level);
		delete &yi_tmp;
		Function* fi=new Function(x,yi);
		const ExprConstant* c=dynamic_cast<const ExprConstant*>(&(fi->expr()));
		if (c && c->dim.is_scalar() && c->get_value()==Interval::zero()) { // use a more efficient structure than a DAG!
			if (!zero) zero=fi;
			else delete fi;
			comp[i] = zero;
		} else {
			comp[i] = fi;
		}
	}

	// This old code was generating all the m*n components
	// in the case of a matrix-valued function
	// -----------------------------------------------------------------------------------------
	//	else {
	//		comp = new Function*[image_dim()];		int n=expr().dim.nb_cols();
	//		for (int i=0; i<expr().dim.nb_rows(); i++) {
	//
	//			for (int j=0; j<n; j++) {
	//				Array<const ExprSymbol> x(nb_arg());
	//				varcopy(symbs,x);
	//				const ExprIndex& yij_tmp=expr()[DoubleIndex::one_elt(expr().dim,i,j)];
	//				const ExprNode& yij=ExprCopy().copy(symbs, x, yij_tmp).simplify();
	//				delete &yij_tmp;
	//				Function* fij=new Function(x,yij);
	//				const ExprConstant* c=dynamic_cast<const ExprConstant*>(&(fij->expr()));
	//				if (c && c->get_value()==Interval::zero()) { // use a more efficient structure than a DAG!
	//					if (!zero) zero=fij;
	//					else delete fij;
	//						comp[i*n+j] = zero;
	//				} else {
	//					comp[i*n+j] = fij;
	//				}
	//			}
	//		}
	//	}
	// -----------------------------------------------------------------------------------------


//	cout << "--------- separation ---------" << endl;
//	for (int i=0; i<dimension(); i++) {
//		cout << (*this)[i] << endl << endl;
//	}
//	cout << "------------------------------" << endl;
}

void Function::init(const Array<const ExprSymbol>& x, const ExprNode& y, const char* name) {

	df=NULL;
	_def_domain=NULL;
	comp=NULL;
	zero=NULL;

	this->name=duplicate_or_generate(name);

	__all_symbols_scalar=true; // by default

	symbs.resize(x.size());
	__symbol_index = new int[x.size()];
	int index=0;

	(int&) _nb_var=0;

	for (int i=0; i<x.size(); i++) {
		symbs.set_ref(i,x[i]);
		((ExprSymbol&) x[i]).key = i;
		__all_symbols_scalar &= x[i].dim.is_scalar();
		__symbol_index[i]=index;

		(int&) _nb_var += x[i].dim.size();

		for (int j=0; j<x[i].dim.size(); j++) {
			index++;
		}
	}

//	// Find input used
//	ExprSubNodes nodes(y);
//	NodeMap<bool> visited;
//	for (int i=0; i<nodes.size(); i++) {
//		ExprNAryOp* nary_op = dynamic_cast<const ExprNA>)
//	}


	is_used.resize(_nb_var);

	FindInputsUsed fsu(x, y, __symbol_index, is_used);
	for (BitSet::const_iterator it=is_used.begin(); it!=is_used.end(); ++it) {
		((vector<int>&) used_vars).push_back((int) it);
	}

	_image_dim = y.dim;

	nodes.init(&x,y,true);

	decorate(x,y);

	_eval = new Eval(*this);
	_hc4revise = new HC4Revise(*_eval);
	_grad = new Gradient(*_eval);
	_inhc4revise = new InHC4Revise(*_eval);

	// ===== display adjacency (debug) =========
//	cout << "adjacency of function" << *this << ":" << endl;
//	for (int i=0; i<nb_used_inputs; i++)
//		cout << used_input[i] << " ";
//	cout << endl;
}

void Function::decorate(const Array<const ExprSymbol>& x, const ExprNode& y) {

	((CompiledFunction&) cf).compile(*this);

	// The following loop has two purposes:
	// 1/ Symbols that do not appear in the expression y will not
	//    be handled by the following loop
	// 2/ Symbols that appear in y should be inside x. So we decorate
	//    first the symbols in order to quickly check if this is the case.
	for (int i=0; i<nb_arg(); i++) {
		if (arg(i).f!=NULL)
			ibex_error("A symbol inside the expression of a function already belongs to another function");
		else
			((ExprSymbol&) arg(i)).f=this;
	}

	for (int i=0; i<nb_nodes(); i++) {

		const ExprSymbol* symbol=dynamic_cast<const ExprSymbol*> (&node(i));

		if (symbol) {
			//-------------------------------------------------------
			// we check that a symbol appearing in the expression
			// is an argument of the function
			if ((*symbol).f!=this) { // can be either NULl or something else
				ibex_error("A symbol used in the expression of a function is not an argument");
			}
			//-------------------------------------------------------
		} else {
			if (node(i).f!=NULL)
				ibex_error("A sub-expression of a function already belongs to another function");
			else
				((ExprNode&) node(i)).f=this;
		}
	}
}

std::string Function::minibex(bool human) const {
	stringstream s;
	s << "function " << name << "(";
	for (int i=0; i<nb_arg(); i++) {
		const ExprSymbol& x=arg(i);
		s << x.name;
		if (x.dim.nb_rows()>1 || x.dim.nb_cols()>1) s << '[' << x.dim.nb_rows() << ']';
		if (x.dim.nb_cols()>1) s << '[' << x.dim.nb_cols() << ']';
		if (i<nb_arg()-1) s << ",";
	}
	s << ")" << endl;

	Expr2Minibex().print(s,expr(),human);
	s << endl << "end";
	s.flush();
	return s.str();
}

} // namespace ibex
