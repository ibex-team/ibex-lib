//============================================================================
//                                  I B E X                                   
// File        : ibex_NormalizedSystem.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 20, 2013
// Last Update : Dec 20, 2013
//============================================================================

#include "ibex_NormalizedSystem.h"
#include "ibex_SystemFactory.h"
#include "ibex_ExprCopy.h"
#include <utility>

using namespace std;

namespace ibex {

namespace {

class SystemNormalize : public SystemFactory {

	public:

	SystemNormalize(const System& sys, double eps) {

		// do not initialize variables with sys.f.args
		// since f may be uninitialized (unconstrained problem)
		add_var(sys.args);

		if (sys.goal!=NULL) add_goal(*sys.goal);

		// TODO: factorize code with SystemExtend
		// note: sys.ctrs.size()<>sys.nb_ctr in general but
		// with NORMALIZE, they have to match (only scalar constraints).
		if(sys.ctrs.size()!=sys.nb_ctr) {
			not_implemented("normalization with vector equations");
		}
		for (int i=0; i<sys.ctrs.size(); i++) {

			Function& fi=sys.ctrs[i].f;

			if (sys.ctrs[i].op==EQ) {
				pair<const ExprNode*, const Interval*> p=sys.ctrs[i].is_thick_equality();
				if (p.first!=NULL || eps>0) {
					const ExprNode *f_1;
					const ExprNode *f_2;
					if (p.first!=NULL) {
						f_1=&( ExprCopy().copy(fi.args(), sys.args, *p.first) - p.second->ub());
						f_2=&(-ExprCopy().copy(fi.args(), sys.args, *p.first) - (-p.second->lb()));
					} else {
						f_1=&( ExprCopy().copy(fi.args(), sys.args, fi.expr()) - eps);
						f_2=&(-ExprCopy().copy(fi.args(), sys.args, fi.expr()) - eps);
					}
					add_ctr(ExprCtr(*f_1,LEQ));
					cleanup(*f_1, false);
					add_ctr(ExprCtr(*f_2,LEQ));
					cleanup(*f_2, false);
				} else {
					const ExprNode* f_i=&ExprCopy().copy(fi.args(), sys.args, fi.expr());
					add_ctr(ExprCtr(*f_i,EQ));
					cleanup(*f_i, false);
				}
			} else {

				const ExprNode* f_i=&ExprCopy().copy(fi.args(), sys.args, fi.expr());

				switch (sys.ctrs[i].op) {
				case LT:
					ibex_warning("warning: strict inequality (<) replaced by inequality (<=).");
				case LEQ:
					break;
				case GT:
					ibex_warning("warning: strict inequality (>) replaced by inequality (>=).");
				case GEQ:
					f_i = & (- (*f_i)); // reverse the inequality
					break;
				default:
					assert(false);
					break;
				}
				add_ctr(ExprCtr(*f_i,LEQ));

				cleanup(*f_i, false);
			}


		}
	}
};

} // end anonymous namespace

NormalizedSystem::NormalizedSystem(const System& sys, double eps) {
	init(SystemNormalize(sys,eps));

	_orig_index = new int[nb_ctr];
	int j=0;
	for (int i=0; i<sys.nb_ctr; i++) {
		_orig_index[j++]=i;
		if (sys.ctrs[i].op==EQ && (sys.ctrs[i].is_thick_equality().first!=NULL || eps>0)) _orig_index[j++]=i;
	}
	assert(j==nb_ctr);
}

NormalizedSystem::~NormalizedSystem() {
	delete[] _orig_index;
}

} // end namespace ibex
