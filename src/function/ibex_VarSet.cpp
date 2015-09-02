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

VarSet::VarSet(Function& f, const ExprNode& y1, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3,y4), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3,y4,y5), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3,y4,y5,y6), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3,y4,y5,y6,y7), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3,y4,y5,y6,y7,y8), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const ExprNode& y9, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3,y4,y5,y6,y7,y8,y9), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const ExprNode& y9, const ExprNode& y10, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3,y4,y5,y6,y7,y8,y9,y10), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const ExprNode& y9, const ExprNode& y10, const ExprNode& y11, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11), init_box);
}

VarSet::VarSet(Function& f, const ExprNode& y1, const ExprNode& y2, const ExprNode& y3, const ExprNode& y4, const ExprNode& y5, const ExprNode& y6, const ExprNode& y7, const ExprNode& y8, const ExprNode& y9, const ExprNode& y10, const ExprNode& y11, const ExprNode& y12, const IntervalVector& init_box) : y_init(1) {
 init(f, Array<const ExprNode>(y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12), init_box);
}

VarSet::VarSet(Function& f, const Array<const ExprNode>& y, const IntervalVector& init_box) : y_init(1) {
	 init(f, y, init_box);
}

VarSet::VarSet(Function& f, const BitSet& vars, const IntervalVector& init_box) : y_init(1) {
	//CtcQuantif(ctc, vars, init_box, prec, own_ctc);
}

void VarSet::init(Function& f, const Array<const ExprNode>& y, const IntervalVector& init_box) {
	assert(y.size()>0);
	assert(f.nb_var()>y.size());

	nb_param = 0;

	vars.initialise(0,f.nb_var()-1,BitSet::empt);

	vars.fill(0, f.nb_var()-1); // by default all are variables

	for (int i=0; i<y.size(); i++) {
		int shift;

		const ExprSymbol* symbol = dynamic_cast<const ExprSymbol*>(&y[i]);
		if (symbol) {
			shift = 0;
		} else {
			const ExprIndex* index = dynamic_cast<const ExprIndex*>(&y[i]);
			if (!index) ibex_error("VarSet: not a symbol");

			pair<const ExprSymbol*,int> p= index->symbol_shift();
			if (shift==-1) ibex_error("VarSet: not a symbol");

			symbol=p.first;
			shift=p.second;
		}

		int j=0;
		while (j<f.nb_arg() && strcmp(f.arg(j).name,symbol->name)!=0) j++;

		if (j<f.nb_arg()) {   // y[i] found in the arguments of f
			for (int k=0; k<y[i].dim.size(); k++) {
				vars.remove(f.symbol_index[j]+shift+k); //  --> marked as a parameter
				nb_param++;
			}
		}
	}

	y_init.resize(nb_param);

	assert(init_box.size()==nb_param);

	y_init = init_box;

	nb_var = f.nb_var() - nb_param;
}

IntervalVector VarSet::extend(const IntervalVector& x) const {
	assert(x.size()==nb_var);

	IntervalVector fullbox(nb_var+nb_param);
	int jx=0;
	int jy=0;

	if (x.is_empty())
		fullbox.set_empty();
	else {
		for (int i=0; i<nb_var+nb_param; i++) {
			if (vars[i]) fullbox[i]=x[jx++];
			else         fullbox[i]=y_init[jy++];
		}
	}
	return fullbox;
}

IntervalVector VarSet::chop(const IntervalVector& fullbox) const {
	assert(fullbox.size()==nb_param+nb_var);
	IntervalVector x(nb_var);
	int jx=0;
	for (int i=0; i<nb_var+nb_param; i++) {
		if (vars[i]) x[jx++]=fullbox[i];
	}
	return x;
}

} // namespace ibex
