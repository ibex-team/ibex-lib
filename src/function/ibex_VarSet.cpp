//============================================================================
//                                  I B E X                                   
// File        : ibex_VarSet.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 1, 2015
//============================================================================

#include "ibex_VarSet.h"

using namespace std;

namespace ibex {

VarSet::VarSet(Function& f, const ExprNode& x1) :nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8,x9));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, const ExprNode& x11) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11));
}

VarSet::VarSet(Function& f, const ExprNode& x1, const ExprNode& x2, const ExprNode& x3, const ExprNode& x4, const ExprNode& x5, const ExprNode& x6, const ExprNode& x7, const ExprNode& x8, const ExprNode& x9, const ExprNode& x10, const ExprNode& x11, const ExprNode& x12) : nb_var(-1), nb_param(-1), param_box(1) {
 init(f, Array<const ExprNode>(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12));
}

VarSet::VarSet(Function& f, const Array<const ExprNode>& y) : nb_var(-1), nb_param(-1), param_box(1) {
	 init(f, y);
}

VarSet::VarSet(Function& f, const BitSet& vars) : nb_var(vars.size()), nb_param(f.nb_var()-vars.size()), param_box(nb_param), vars(vars) {

}

void VarSet::init(Function& f, const Array<const ExprNode>& x) {
	assert(x.size()>0);
	assert(f.nb_var()>x.size());

	(int&) nb_var = 0;

	// by default all are parameters
	vars.initialise(0,f.nb_var()-1,BitSet::empt);

	//vars.fill(0, f.nb_var()-1);

	for (int i=0; i<x.size(); i++) {
		int shift;

		const ExprSymbol* symbol = dynamic_cast<const ExprSymbol*>(&x[i]);
		if (symbol) {
			shift = 0;
		} else {
			const ExprIndex* index = dynamic_cast<const ExprIndex*>(&x[i]);
			if (!index) ibex_error("VarSet: not a symbol");

			pair<const ExprSymbol*,int> p= index->symbol_shift();
			if (shift==-1) ibex_error("VarSet: not a symbol");

			symbol=p.first;
			shift=p.second;
		}

		int j=0;
		while (j<f.nb_arg() && strcmp(f.arg(j).name,symbol->name)!=0) j++;

		if (j<f.nb_arg()) {   // y[i] found in the arguments of f
			for (int k=0; k<x[i].dim.size(); k++) {
				vars.add(f.symbol_index[j]+shift+k); //  --> marked as a variable
				((int&) nb_var)++;
			}
		}
	}

	(int&) nb_param = f.nb_var() - nb_var;

	param_box.resize(nb_param);

}

IntervalVector VarSet::extend(const IntervalVector& var_box) const {
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

IntervalVector VarSet::chop(const IntervalVector& full_box) const {
	assert(full_box.size()==nb_param+nb_var);
	IntervalVector var_box(nb_var);
	int jx=0;
	for (int i=0; i<nb_var+nb_param; i++) {
		if (vars[i]) var_box[jx++]=full_box[i];
	}
	return var_box;
}

} // namespace ibex
