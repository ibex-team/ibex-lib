//============================================================================
//                                  I B E X                                   
// File        : ibex_VarSet.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 1, 2015
//============================================================================

#include "ibex_VarSet.h"
#include <sstream>

using namespace std;

namespace ibex {

VarSet::VarSet(Function& f, const ExprNode& x1, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8,x9),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, const ExprNode& x11, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11),var);
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, const ExprNode& x11, const ExprNode& x12, bool var) : nb_var(-1), nb_param(-1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12),var);
}

VarSet::VarSet(Function& f, const Array<const ExprNode>& y, bool var) : nb_var(-1), nb_param(-1) {
	 init(f, y, var);
}

VarSet::VarSet(int total, const BitSet& x, bool var) :
		nb_var  (var? x.size()            : total-x.size()),
		nb_param(var? total-x.size() : x.size()),
		vars    (var? x                   : BitSet::all(total)) {

	if (!var)
		vars.setminus_with(x);
}

VarSet::VarSet(const VarSet& v) : nb_var(v.nb_var), nb_param(v.nb_param) {
	vars.clone(v.vars);
}

void VarSet::init(Function& f, const Array<const ExprNode>& x, bool var) {
	assert(x.size()>0);
	assert(f.nb_var()>x.size());

	vars.initialise(0,f.nb_var()-1,BitSet::empt);

	if (var) {
		// by default, all are parameters
		(int&) nb_var = 0;
	} else {
		(int&) nb_var = f.nb_var();
		// by default, all are variables
		vars.fill(0, f.nb_var()-1);
	}

	for (int i=0; i<x.size(); i++) {
		int shift=0; // by default (if x[i] is a symbol)

		const ExprSymbol* symbol = dynamic_cast<const ExprSymbol*>(&x[i]);
		bool** mask;
		if (symbol) {
			mask=symbol->mask();
		} else {
			const ExprIndex* index = dynamic_cast<const ExprIndex*>(&x[i]);
			if (!index) ibex_error("VarSet: not a symbol");

			pair<const ExprSymbol*,bool**> p= index->symbol_mask();
			symbol=p.first;
			mask=p.second;

			if (mask==NULL) ibex_error("VarSet: not a symbol");
		}

		int j=0;
		while (j<f.nb_arg() && strcmp(f.arg(j).name,symbol->name)!=0) j++;

		if (j>=f.nb_arg()) {   // y[i] found in the arguments of f
			stringstream s;
			s << x[i] << " is not an argument of the function";
			ibex_error(s.str().c_str());
		}
		for (int r=0; r<symbol->dim.nb_rows(); r++) {
			for (int c=0; c<symbol->dim.nb_cols(); c++) {
				if (mask[r][c]) {
					int k=f.symbol_index[j]+(r*symbol->dim.nb_cols())+c;
					if (var)  {
						vars.add(k);  //  --> marked as a variable
						((int&) nb_var)++;
					}
					else {
						vars.remove(k);   //  --> marked as a parameter
						((int&) nb_var)--;
					}
				}
			}
			delete[] mask[r];
		}
		delete[] mask;
	}

	(int&) nb_param = f.nb_var() - nb_var;
}

IntervalVector VarSet::full_box(const IntervalVector& var_box, const IntervalVector& param_box) const {
	assert(var_box.size()==nb_var);

	IntervalVector full_box(nb_var+nb_param);
	int jx=0;
	int jy=0;

	if (var_box.is_empty())
		full_box.set_empty();
	else {
		for (int i=0; i<nb_var+nb_param; i++) {
			if (vars[i]) full_box[i]=var_box[jx++];
			else         full_box[i]=param_box[jy++];
		}
	}
	return full_box;
}

IntervalVector VarSet::var_box(const IntervalVector& full_box) const {
	assert(full_box.size()==nb_param+nb_var);
	IntervalVector var_box(nb_var);

	if (full_box.is_empty()) var_box.set_empty();
	else {
		int j=0;
		for (int i=0; i<nb_var+nb_param && j<nb_var; i++) {
			if (vars[i]) var_box[j++]=full_box[i];
		}
	}
	return var_box;
}

IntervalVector VarSet::param_box(const IntervalVector& full_box) const {
	assert(full_box.size()==nb_param+nb_var);
	IntervalVector param_box(nb_param);

	if (full_box.is_empty()) param_box.set_empty();
	else {
		int j=0;
		for (int i=0; i<nb_var+nb_param && j<nb_param; i++) {
			if (!vars[i]) param_box[j++]=full_box[i];
		}
	}
	return param_box;
}

void VarSet::set_var_box(IntervalVector& full_box, const IntervalVector& var_box) const {
	assert(var_box.size()==nb_var);
	assert(full_box.size()==nb_var+nb_param);

	int j=0;

	if (var_box.is_empty())
		full_box.set_empty();
	else {
		for (int i=0; i<nb_var+nb_param && j<nb_var; i++) {
			if (vars[i]) full_box[i]=var_box[j++];
		}
	}
}

void VarSet::set_param_box(IntervalVector& full_box, const IntervalVector& param_box) const {
	assert(param_box.size()==nb_param);
	assert(full_box.size()==nb_var+nb_param);

	int j=0;

	if (param_box.is_empty())
		full_box.set_empty();
	else {
		for (int i=0; i<nb_var+nb_param && j<nb_param; i++) {
			if (!vars[i]) full_box[i]=param_box[j++];
		}
	}
}

std::ostream& operator<<(std::ostream& os, const VarSet& v) {
	for (int i=0; i<v.nb_var+v.nb_param; i++) {
		os << (v.vars[i] ? 'v' : 'p') << ' ';
	}
	return os;
}


} // namespace ibex
