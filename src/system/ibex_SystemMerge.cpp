//============================================================================
//                                  I B E X                                   
// File        : ibex_SystemMerge.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 27, 2013
// Last Update : May 09, 2019
//============================================================================

#include "ibex_SystemFactory.h"
#include "ibex_Exception.h"

#include <sstream>

using namespace std;

namespace ibex {

namespace {

class SystemMerge : public SystemFactory {

public:

	SystemMerge(const System& sys1, const System& sys2) {

		set_simplification_level(0); // constraints do not need to be simplified again

		SymbolMap<const ExprSymbol*> map;

		/* the set of symbols of the resulting system,
		 * starting by that of sys1 */
		std::vector<const ExprSymbol*> vars1;
		/* the same set but starting with symbols of sys2
		 * (the symbols of sys1 that do not appear in sys2
		 * are actually not appended because they are not necessary) */
		std::vector<const ExprSymbol*> vars2;

		std::vector<IntervalVector> bound;
		int n=0; // number of variables

		for (int i=0; i<sys1.args.size(); i++) {
			map.insert_new(sys1.args[i].name,&sys1.args[i]);
			vars1.push_back(&sys1.args[i]);
		}
		bound.push_back(sys1.box);
		n = sys1.box.size();

		int kk=0; // indice  in sys.box
		int ff = 0;
		for (int i=0; i<sys2.args.size(); i++) {
			ff = kk+(sys2.args[i].size);
			if (map.used(sys2.args[i].name)) {
				const ExprSymbol* x=map[sys2.args[i].name];
				if (x->dim!=sys2.args[i].dim) {
					std::stringstream s;
					s << "cannot merge systems: symbol\"" << x->name << "\" has different dimensions";
					ibex_error(s.str().c_str());
				}
			} else {
				map.insert_new(sys2.args[i].name,&sys2.args[i]);
				vars1.push_back(&sys2.args[i]);

				bound.push_back(sys2.box.subvector(kk, ff-1));
				n += (sys2.args[i].size);
			}
			vars2.push_back(map[sys2.args[i].name]);
			kk = ff;
		}

		// FIXME: the merge of two sys.box is not clean. If a variable exist in sys1 and sys2, we keep only those of sys1
		Array<const ExprSymbol> newvars(vars1);
		IntervalVector newbound(n);
		int i=0;
		for (std::vector<IntervalVector>::const_iterator it=bound.begin(); it!=bound.end(); it++) {
			newbound.put(i,*it);
			i+=(*it).size();
		}


		add_var(newvars,newbound);

		// -------------  Add goal (if any) -----------
		if (sys1.goal!=NULL) {
			if (sys2.goal!=NULL)
				ibex_error("cannot merge two systems with goal functions");
			else {
				const ExprNode& e=ExprCopy().copy(sys1.goal->args(), vars1, sys1.goal->expr());
				add_goal(e);
				cleanup(e, false);
			}
		}
		else if (sys2.goal) {
			const ExprNode& e=ExprCopy().copy(sys2.goal->args(), vars2, sys2.goal->expr());
			add_goal(e);
			cleanup(e, false);
		}

		// -------------  Add constraints -----------
		vector<const ExprNode*> to_be_deleted;
		for (int i=0; i<sys1.ctrs.size(); i++) {
			const ExprNode& e=ExprCopy().copy(sys1.ctrs[i].f.args(), vars1, sys1.ctrs[i].f.expr());
			add_ctr(ExprCtr(e,sys1.ctrs[i].op));

			// don't cleanup nodes now because SystemFactory keeps track of nodes in f_ctrs_copy.
			to_be_deleted.push_back(&e);
		}
		for (int i=0; i<sys2.ctrs.size(); i++) {
			const ExprNode& e=ExprCopy().copy(sys2.ctrs[i].f.args(), vars2, sys2.ctrs[i].f.expr());
			add_ctr(ExprCtr(e,sys2.ctrs[i].op));

			// see comment supra
			to_be_deleted.push_back(&e);
		}

		for (vector<const ExprNode*>::iterator it=to_be_deleted.begin(); it!=to_be_deleted.end(); it++) {
			cleanup(**it, false);
		}
	}
};

} // end anonymous namespace


System::System(const System& sys1, const System& sys2) : id(next_id()), nb_var(0), nb_ctr(0), ops(NULL), box(1) {
	init(SystemMerge(sys1,sys2));
}

} // end namespace ibex



