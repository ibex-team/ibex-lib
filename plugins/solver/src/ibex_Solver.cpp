//============================================================================
//                                  I B E X                                   
// File        : ibex_Solver.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : Oct 01, 2017
//============================================================================

#include "ibex_Solver.h"
#include "ibex_Newton.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_LinearException.h"
#include "ibex_Manifold.h"

#include <cassert>

using namespace std;

namespace ibex {

namespace {
	class EmptyBoxException : Exception { };
}

Solver::Solver(const System& sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer,
		const Vector& eps_x_min, const Vector& eps_x_max) :
		  ctc(ctc), bsc(bsc), buffer(buffer), eps_x_min(eps_x_min), eps_x_max(eps_x_max),
		  boundary_test(ALL_TRUE), time_limit(-1), cell_limit(-1), trace(0),
		  solve_init_box(sys.box), eqs(NULL), ineqs(NULL),
		  params(sys.nb_var,BitSet::empty(sys.nb_var),false) /* no forced parameter by default */,
		  manif(NULL), time(0), nb_cells(0) {

	assert(sys.box.size()==ctc.nb_var);

	int nb_eq=0;

	// count the dimension of equalities
	for (int i=0; i<sys.nb_ctr; i++) {
		if (sys.ctrs[i].op==EQ) nb_eq+=sys.ctrs[i].f.image_dim();
	}

	if (nb_eq==sys.f_ctrs.image_dim())
		eqs=&sys; // useless to create a new one
	else {
		ineqs=new System(sys,System::INEQ_ONLY);

		if (nb_eq>0) {
			eqs=new System(sys,System::EQ_ONLY);
		}
		// else: strange to use the solver for inequalities only...
	}

	n=sys.nb_var;
	m=eqs? eqs->f_ctrs.image_dim() : 0;
	nb_ineq=ineqs? ineqs->f_ctrs.image_dim() : 0;

	if (m==0 || m==n)
		boundary_test=ALL_FALSE;

	if (m>n)
		ibex_warning("Certification not implemented for over-constrained systems ");
}

void Solver::set_var_names() {

	const System& sys=eqs? *eqs : *ineqs;

	int v=0;
	for (int s=0; s<sys.args.size(); s++) {
		const ExprSymbol& x=sys.args[s];
		switch (x.dim.type()) {
		case Dim::SCALAR:
			manif->var_names[v++].assign(x.name);
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
			for (int i=0; i<x.dim.vec_size(); i++)
				manif->var_names[v++].assign(string(x.name)+'('+to_string(i+1)+')');
			break;
		default: // MATRIX
			for (int i=0; i<x.dim.nb_rows(); i++)
				for (int j=0; j<x.dim.nb_cols(); j++)
					manif->var_names[v++].assign(string(x.name)+'('+to_string(i+1)+','+to_string(j+1)+')');
			break;
		}
	}
	assert(v==sys.nb_var);
}

void Solver::set_params(const VarSet& _params) {
	params=_params;
}

Solver::~Solver() {
	if (ineqs) {
		delete ineqs;
		if (eqs) {
			delete eqs;
		}
	}

	if (manif) delete manif;
}

void Solver::start(const IntervalVector& init_box) {

	buffer.flush();

	if (manif) delete manif;

	manif = new Manifold(n,m,nb_ineq);

	set_var_names();

	Cell* root=new Cell(init_box);

	// add data required by the bisector
	bsc.add_property(init_box, root->prop);

	// add data required by the contractor
	ctc.add_property(init_box, root->prop);

	buffer.add_property(init_box, root->prop);

	buffer.push(root);
	nb_cells = 1;

	time = 0;

	timer.restart();
}

void Solver::start(const char* input_paving) {
	buffer.flush();

	if (manif) delete manif;
	manif = new Manifold(n,m,nb_ineq);
	manif->load(input_paving);

	// may erase former variable names if the input paving was actually
	// not calculated with the same minibex file.
	set_var_names();

	vector<QualifiedBox>::const_iterator it=manif->unknown.begin();

	// the unknown and pending boxes have to be processed
	while (it!=manif->pending.end()) {
		if (it==manif->unknown.end())
			it=manif->pending.begin();
		if (it==manif->pending.end())
			break;

		Cell* cell=new Cell(it->existence());

		buffer.add_property(it->existence(), cell->prop);

		// add data required by the bisector
		bsc.add_property(it->existence(), cell->prop);

		// add data required by the contractor
		ctc.add_property(it->existence(), cell->prop);

		buffer.push(cell);

		it++;
	}

	nb_cells=0; // no new cell created!

	manif->unknown.clear();
	manif->pending.clear();

	timer.restart();
}

QualifiedBox* Solver::next() {
	while (!buffer.empty()) {
		if (time_limit >0) timer.check(time_limit);

		if (trace==2) cout << buffer << endl;

		Cell* c=buffer.top();

		ContractContext context(c->prop);

		int v=c->bisected_var; // last bisected var.

		if (v!=-1) { // not the root node :  impact set to the last bisected variable only
			context.impact = BitSet::singleton(n,v);
		}

		try {
			ctc.contract(c->box,context);

			if (c->box.is_empty()) throw EmptyBoxException();

			// certification is performed at each intermediate step
			// if the system is under constrained
			if (!c->box.is_empty() && m<n) {
				// note: cannot return PENDING status
				QualifiedBox new_sol=check_sol(c->box);
				if (new_sol.status!=QualifiedBox::UNKNOWN) {
					if ((m==0 && new_sol.status==QualifiedBox::INNER) ||
							!is_too_large(new_sol.existence())) {
						delete buffer.pop();
						return &store_sol(new_sol);
					} else {
						// otherwise: continue search...
					}
				}
				else {
					// otherwise: continue search...
				}
			}

			try {
				if (is_too_small(c->box))
					throw NoBisectableVariableException();

				// next line may also throw NoBisectableVariableException
				pair<Cell*,Cell*> new_cells=bsc.bisect(*c);

				delete buffer.pop();
				buffer.push(new_cells.first);
				buffer.push(new_cells.second);
				nb_cells+=2;
				if (cell_limit >=0 && nb_cells>=cell_limit) throw CellLimitException();
			}

			catch (NoBisectableVariableException&) {
				QualifiedBox new_sol=check_sol(c->box);
				delete buffer.pop();
				return &store_sol(new_sol);
			}
		}
		catch (EmptyBoxException&) {
			delete buffer.pop();
			//impact.remove(v); // note: in case of the root node, we should clear the bitset
			// instead but since the search is over, the impact is not used anymore.
			// JN: that make a bug with Mingw

			continue;
		}
	}

	return NULL;
}

Solver::Status Solver::solve(const IntervalVector& init_box) {
	start(init_box);
	return solve();
}

Solver::Status Solver::solve(const char* init_paving) {
	start(init_paving);
	return solve();
}

Solver::Status Solver::solve() {

	try {

		while (next()!=NULL) { }

		if (manif->unknown.size()>0)
			manif->status = NOT_ALL_VALIDATED;
		else if (manif->inner.size()>0 || manif->boundary.size()>0)
			manif->status = SUCCESS;
		else
			manif->status = INFEASIBLE;

	} catch (TimeOutException&) {
		flush();
		manif->status = TIME_OUT;
	}
	catch (CellLimitException&) {
		flush();
		manif->status = CELL_OVERFLOW;
	}

	timer.stop();
	time = timer.get_time();

	manif->time += time;
	manif->nb_cells += nb_cells;

	return manif->status;
}


QualifiedBox Solver::check_sol(const IntervalVector& box) {

	//QualifiedBox sol(n);
	QualifiedBox::sol_status status = QualifiedBox::INNER; // by default
	IntervalVector existence(box);
	IntervalVector* unicity = NULL;
	VarSet* varset = NULL;

	if (eqs) {

		if (m>n) {
			// Certification not implemented for over-constrained systems
			status = QualifiedBox::UNKNOWN;
		} else if (m<n) {
			// ====== under-constrained =========
			try {

				unicity = new IntervalVector(n);

				VarSet _varset=get_newton_vars(eqs->f_ctrs,box.mid(),params);

				if (inflating_newton(eqs->f_ctrs, _varset, box, existence, *unicity)) {
					if (params.nb_param<n-m)
						varset = new VarSet(_varset);
				} else
					status = QualifiedBox::UNKNOWN;

			} catch(SingularMatrixException& e) {
				status = QualifiedBox::UNKNOWN;
			}
		} else {
			// ====== well-constrained =========
			unicity = new IntervalVector(n);

			if (!inflating_newton(eqs->f_ctrs, box.mid(), existence, *unicity)) {
				status = QualifiedBox::UNKNOWN;
			}
		}
	}

	if (status==QualifiedBox::UNKNOWN) {
		existence = box;
		if (unicity!=NULL) delete unicity;
		unicity=NULL;
		assert(varset==NULL);
	} else {
		// The inflating Newton may cause the existence box to be disjoint from the input box.

		// Note that the following line also tests the case of an existence box outside
		// the initial box of the search
		if (box.is_disjoint(existence)) {
			if (unicity) delete unicity;
			if (varset) delete varset;
			throw EmptyBoxException();
		}
	}

	if (status==QualifiedBox::INNER && !existence.is_subset(solve_init_box))
		// BOUNDARY by default: will be verified later
		status = QualifiedBox::BOUNDARY;

	if (ineqs) {
		Interval y,r;
		for (int i=0; i<ineqs->nb_ctr; i++) {
			NumConstraint& c=ineqs->ctrs[i];
			assert(c.f.image_dim()==1);
			y=c.f.eval(existence);
			r=c.right_hand_side().i();

			if (y.is_disjoint(r)) {
				if (unicity) delete unicity;
				if (varset) delete varset;
				throw EmptyBoxException();
			}

			if (status==QualifiedBox::INNER && !y.is_subset(r)) {
				// BOUNDARY by default: will be verified later
				status = QualifiedBox::BOUNDARY;
			}
		}
	}

	if (status==QualifiedBox::BOUNDARY) {
		if (!is_boundary(existence))
			status = QualifiedBox::UNKNOWN;

	}

	if (eqs && n==m) {
		// Check if the solution is new, that is, that the solution is not included in the unicity
		// box of a previously found solution. For efficiency reason, this test is not performed in
		// the case of under-constrained systems (m<n).

		for (vector<QualifiedBox>::iterator it=manif->inner.begin(); it!=manif->inner.end(); it++) {
			if (it->unicity().is_superset(existence)) {
				if (unicity) delete unicity;
				if (varset) delete varset;
				throw EmptyBoxException();
			}
		}
	}

	return QualifiedBox(existence,status,unicity,varset);
}

bool Solver::is_boundary(const IntervalVector& box) {

	switch (boundary_test) {
	case ALL_TRUE : return true;
	case ALL_FALSE : return false;
	case FULL_RANK : {
		// get active boundary constraints
		BitSet bound(n);
		for (int i=0; i<n; i++) {
			if (solve_init_box[i].lb()>=box[i].lb()) bound.add(i);
			if (solve_init_box[i].ub()<=box[i].ub()) {
				if (bound[i]) return false;
				else bound.add(i);
			}
		}

		// get active inequalities
		BitSet ineq_active=ineqs? ineqs->active_ctrs(box) : BitSet::empty(n);

		int size = bound.size() + m + ineq_active.size();

		IntervalMatrix J(size, n);
		int i=0;
		int v;
		for (; i<bound.size(); i++) {
			J[i].clear();
			v=(i==0? bound.min() : bound.next(v));
			J[i][v]=1.0;
		}
		if (m>0) {
			J.put(i,0,eqs->f_ctrs.jacobian(box));
			i+=m;
		}
		if (ineqs!=NULL) {
			J.put(i,0,ineqs->f_ctrs.jacobian(box,ineq_active));
		}
		return full_rank(J);
	}
	case HALF_BALL:
	default:
		not_implemented("\"half-ball\" boundary test");
		return false;
	}
}

bool Solver::is_too_small(const IntervalVector& box) {
	for (int i=0; i<n; i++)
		if (box[i].diam()>eps_x_min[i]) return false;
	return true;
}

bool Solver::is_too_large(const IntervalVector& box) {
	for (int i=0; i<n; i++)
		if (box[i].diam()>eps_x_max[i]) return true;
	return false;
}

QualifiedBox& Solver::store_sol(const QualifiedBox& sol) {

	if (trace >=1) cout << sol << endl;

	switch (sol.status) {
	case QualifiedBox::INNER    :
		manif->inner.push_back(sol);
		return manif->inner.back();
	case QualifiedBox::BOUNDARY :
		manif->boundary.push_back(sol);
		return manif->boundary.back();
	case QualifiedBox::UNKNOWN  :
		manif->unknown.push_back(sol);
		return manif->unknown.back();
	case QualifiedBox::PENDING :
	default:
		manif->pending.push_back(sol);
		return manif->pending.back();
	}
}

void Solver::flush() {
	while (!buffer.empty()) {
		Cell* cell=buffer.top();
		QualifiedBox sol(cell->box,QualifiedBox::PENDING);
		store_sol(sol);
		delete buffer.pop();
	}
}

void Solver::report() {

	switch(manif->status) {
	case SUCCESS: cout << "\033[32m" << " solving successful!" << endl;
	break;
	case INFEASIBLE: cout << "\033[31m" << " infeasible problem" << endl;
	break;
	case NOT_ALL_VALIDATED: cout << "\033[31m" << " done! but some boxes have 'unknown' status." << endl;
	break;
	case TIME_OUT: cout << "\033[31m" << " time limit " << time_limit << "s. reached " << endl;
	break;
	case CELL_OVERFLOW: cout << "\033[31m" << " cell overflow" << endl;
	}

	cout << "\033[0m" << endl;

	cout << " number of inner boxes:\t\t" << manif->inner.size() << endl;
	cout << " number of boundary boxes:\t" << manif->boundary.size() << endl;
	cout << " number of unknown boxes:\t" << manif->unknown.size() << endl;
	cout << " number of pending boxes:\t" << manif->pending.size() << endl;
	cout << " cpu time used:\t\t\t" << time << "s";
	if (manif->time!=time)
		cout << " [total=" << manif->time << "]";
	cout << endl;
	cout << " number of cells:\t\t" << nb_cells;
	if (manif->nb_cells!=nb_cells)
		cout << " [total=" << manif->nb_cells << "]";
	cout << endl << endl;
}

} // end namespace ibex
