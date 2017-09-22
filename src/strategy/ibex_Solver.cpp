//============================================================================
//                                  I B E X                                   
// File        : ibex_Solver.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#include "ibex_Solver.h"
#include "ibex_Newton.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_LinearException.h"

#include <cassert>

using namespace std;

namespace ibex {

namespace {
	class EmptyBoxException : Exception { };
}

Solver::Solver(const System& sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer,
		const Vector& eps_x_min, const Vector& eps_x_max) :
		  ctc(ctc), bsc(bsc), buffer(buffer), eps_x_min(eps_x_min), eps_x_max(eps_x_max),
		  boundary_test(ALL_TRUE), time_limit(-1), cell_limit(-1), trace(0), impact(BitSet::all(ctc.nb_var)),
		  solve_init_box(IntervalVector::empty(ctc.nb_var)), eqs(NULL), ineqs(NULL), params(NULL),
		  nb_inner(0), nb_boundary(0), nb_unknown(0), time(0), nb_cells(0) {

	init(sys, NULL);

}

Solver::Solver(const System& sys, const BitSet& _params, Ctc& ctc, Bsc& bsc, CellBuffer& buffer,
		const Vector& eps_x_min, const Vector& eps_x_max) :
		  ctc(ctc), bsc(bsc), buffer(buffer), eps_x_min(eps_x_min), eps_x_max(eps_x_max),
		  boundary_test(ALL_TRUE), time_limit(-1), cell_limit(-1), trace(0), impact(BitSet::all(ctc.nb_var)),
		  solve_init_box(IntervalVector::empty(ctc.nb_var)), eqs(NULL), ineqs(NULL), params(NULL),
		  nb_inner(0), nb_boundary(0), nb_unknown(0), time(0), nb_cells(0) {

	init(sys,&_params);

}

void Solver::init(const System& sys, const BitSet* _params) {

	int nb_eq=0;

	if (_params) {
		this->params=new BitSet(*_params);
	}

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

	if (m==0 || m==n)
		boundary_test=ALL_FALSE;
}

Solver::~Solver() {
	if (params)
		delete params;

	if (ineqs) {
		delete ineqs;
		if (eqs) {
			delete eqs;
		}
	}
}

void Solver::start(const IntervalVector& init_box) {
	buffer.flush();
	solutions.clear();

	nb_inner = 0;
	nb_boundary = 0;
	nb_unknown = 0;

	solve_init_box = init_box;
	nb_cells = 0;
	time = 0;

	assert(init_box.size()==ctc.nb_var);

	Cell* root=new Cell(init_box);

	// add data required by this solver
	root->add<BisectedVar>();

	// add data required by the bisector
	bsc.add_backtrackable(*root);

	buffer.push(root);

	int nb_var=init_box.size();

	IntervalVector tmpbox(ctc.nb_var);

	Timer::start();
}

Solver::Status Solver::next(const SolverOutputBox*& sol) {
	try  {
		while (!buffer.empty()) {

			if (trace==2) cout << buffer << endl;

			Cell* c=buffer.top();

			int v=c->get<BisectedVar>().var;      // last bisected var.

			if (v!=-1)                          // no root node :  impact set to 1 for last bisected var only
				impact.add(v);
			else                                // root node : impact set to 1 for all variables
				impact.fill(0,ctc.nb_var-1);

			try {
				ctc.contract(c->box,impact);

				if (c->box.is_empty()) throw EmptyBoxException();

				if (v!=-1)
					impact.remove(v);
				else                              // root node : impact set to 0 for all variables after contraction
					impact.clear();

				// certification is performed at each intermediate step
				// if the system is under constrained
				if (!c->box.is_empty() && m<n) {
					SolverOutputBox new_sol=check_sol(c->box);
					if (new_sol.status!=SolverOutputBox::UNKNOWN) {
						if ((m==0 && new_sol.status==SolverOutputBox::INNER) ||
								!is_too_large(new_sol.existence())) {
							delete buffer.pop();
							store_sol(new_sol);
							sol = &solutions.back();
							if (new_sol.status==SolverOutputBox::INNER) nb_inner++;
							else nb_boundary++;
							return SUCCESS;
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
					pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);

					pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

					delete buffer.pop();
					buffer.push(new_cells.first);
					buffer.push(new_cells.second);
					nb_cells+=2;
					if (cell_limit >=0 && nb_cells>=cell_limit) throw CellLimitException();
				}

				catch (NoBisectableVariableException&) {
					SolverOutputBox new_sol=check_sol(c->box);
					delete buffer.pop();
					store_sol(new_sol);
					sol = &solutions.back();

					switch (sol->status) {
					case SolverOutputBox::INNER:
						nb_inner++;
						return SUCCESS;
					case SolverOutputBox::BOUNDARY:
						nb_boundary++;
						return SUCCESS;
					case SolverOutputBox::UNKNOWN:
						nb_unknown++;
						return NOT_ALL_VALIDATED;
					}
					// note that we skip time_limit_check() here.
					// In the case where "next" is called by "solve",
					// and if time has exceeded, the exception will be raised by the
					// very next call to "next" anyway. This holds, unless "next" finds
					// new solutions again and again endlessly. So there is a little risk
					// of uncaught timeout in this case (but this case is probably already
					// an error case).
				}
			}
			catch (EmptyBoxException&) {
				delete buffer.pop();
				impact.remove(v); // note: in case of the root node, we should clear the bitset
				// instead but since the search is over, the impact is not used anymore.
				continue;
			}
			time_limit_check();
		}
	}
	catch (TimeOutException&) {
		sol = NULL;
		return TIME_OUT;
	}
	catch (CellLimitException&) {
		sol = NULL;
		return CELL_OVERFLOW;
	}

	Timer::stop();
	time+= Timer::VIRTUAL_TIMELAPSE();

	sol = NULL;
	return INFEASIBLE;
}

Solver::Status Solver::solve(const IntervalVector& init_box) {
	start(init_box);

	status=INFEASIBLE; // by default

	const SolverOutputBox* sol; //unused

	while (true) {
		switch(next(sol)) {
		case SUCCESS :
			if (status==INFEASIBLE) status=SUCCESS;
			break;
		case INFEASIBLE:
			return status;
		case NOT_ALL_VALIDATED :
			status = NOT_ALL_VALIDATED;
			break;
		case TIME_OUT :
			status = TIME_OUT;
			return status;
		case CELL_OVERFLOW:
			status = CELL_OVERFLOW;
			return status;
		}
	}
}

void Solver::time_limit_check () {
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	if (time_limit >0 &&  time >=time_limit) throw TimeOutException();
	Timer::start();
}

SolverOutputBox Solver::check_sol(const IntervalVector& box) {

	SolverOutputBox sol(n);

	(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::INNER; // by default

	if (eqs) {

		sol._unicity=new IntervalVector(n);

		if (m>n) {
			ibex_warning("Certification not implemented for over-constrained systems ");
			(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;
		} else if (m<n) {
			// ====== under-constrained =========
			try {
				VarSet varset=get_newton_vars(eqs->f_ctrs,box.mid(),params? *params: BitSet::empty(n));

				if (inflating_newton(eqs->f_ctrs, varset, box, sol._existence, *sol._unicity)) {
					if (!params || ((int) params->size())<n-m)
						sol.varset = new VarSet(varset);
				} else
					(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;

			} catch(SingularMatrixException& e) {
				(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;
			}
		} else {
			// ====== well-constrained =========
			if (!inflating_newton(eqs->f_ctrs, box.mid(), sol._existence, *sol._unicity)) {
				(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;
			}
		}
	} else {
		sol._existence=box;
		sol._unicity=NULL;
	}

	if (sol.status==SolverOutputBox::UNKNOWN) {
		sol._existence=box;
		if (sol._unicity!=NULL) delete sol._unicity;
		sol._unicity=NULL;
	} else {
		// The inflating Newton may cause the existence box to be disjoint from the input box.

		// Note that the following line also tests the case of an existence box outside
		// the initial box of the search
		if (box.is_disjoint(sol.existence())) {
			throw EmptyBoxException();
		}
	}

	if (sol.status==SolverOutputBox::INNER && !sol.existence().is_subset(solve_init_box))
		// BOUNDARY by default: will be verified later
		(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::BOUNDARY;

	if (ineqs) {
		Interval y,r;
		for (int i=0; i<ineqs->nb_ctr; i++) {
			NumConstraint& c=ineqs->ctrs[i];
			assert(c.f.image_dim()==1);
			y=c.f.eval(sol.existence());
			r=c.right_hand_side().i();
			if (y.is_disjoint(r)) throw EmptyBoxException();
			if (sol.status==SolverOutputBox::INNER && !y.is_subset(r)) {
				// BOUNDARY by default: will be verified later
				(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::BOUNDARY;
			}
		}
	}

	if (sol.status==SolverOutputBox::BOUNDARY) {
		if (!is_boundary(sol.existence()))
			(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;

	}

	if (eqs && n==m) {
		// Check if the solution is new, that is, that the solution is not included in the unicity
		// box of a previously found solution. For efficiency reason, this test is not performed in
		// the case of under-constrained systems (m<n).

		for (vector<SolverOutputBox>::iterator it=solutions.begin(); it!=solutions.end(); it++) {
			if (it->status==SolverOutputBox::INNER && it->unicity().is_superset(sol._existence))
				throw EmptyBoxException();
		}
	}

	return sol;
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

void Solver::store_sol(const SolverOutputBox& sol) {

	solutions.push_back(sol);

	if (trace >=1) cout << sol << endl;
}

std::string Solver::format() {
	return
	"\n"
	"-------------------------------------------------------------\n"
	"Output format with --quiet (for automatic output processing):\n"
	"-------------------------------------------------------------\n"
	"[line 1] - 3 values: number of variables, number of equalities, number of inequalities (excluding initial box)\n"
	"[line 2] - 1 value: the status of the search. Possible values are:\n"
	"\t\t* 0=complete search:   all solutions found and all output boxes validated\n"
	"\t\t* 1=complete search:   infeasible problem\n"
	"\t\t* 2=incomplete search: minimal width (--eps-min) reached\n"
	"\t\t* 3=incomplete search: time out\n"
	"\t\t* 4=incomplete search: cell overflow\n"
	"[line 3] - 3 values: number of inner, boundary and unknown boxes\n"
	"[line 4] - 2 values: time (in seconds) and number of cells.\n"
	"\n[lines 5-...] If --sols is enabled, the subsequent lines describe the \"solutions\" (output boxes).\n"
	"\t Each line corresponds to one box and contains the following information:\n"
	"\t - 2*n values: lb(x1), ub(x1),...,ub(x1), ub(xn)\n"
	"\t - 1 value:\n"
	"\t\t* 0 the box is 'inner'\n"
	"\t\t* 1 the box is 'boundary'\n"
	"\t\t* 2 the box is 'unknown'\n"
	"\t - (n-m) values where n=#variables and m=#equalities: the indices of the variables\n"
	"\t   considered as parameters in the parametric proof. Indices start from 1 and if the\n"
	"\t   box is 'unknown', a sequence of n-m zeros are displayed. Nothing is displayed if\n"
	"\t   m=0 or m=n.\n\n";
}

void Solver::report(report_mode mode, bool print_sols) {
	if (mode!=VERBOSE) {
		// character to separate elements in a list
		char s=mode==MMA? ',' : ' ';
		// character that ends a list
		//char e=mode==MMA? ' ' : '}';

		if (mode==MMA) cout << "{{";
		cout << n << s << m << s << (ineqs? ineqs->nb_ctr : 0);
		if (mode==MMA) cout << "},"; else cout << '\n';
		cout << status;
		if (mode==MMA) cout << ",{"; else cout << '\n';
		cout << nb_inner << s << nb_boundary << s << nb_unknown;
		if (mode==MMA) cout << "},{"; else cout << '\n';
		cout << get_time() << s << get_nb_cells();
		if (mode==MMA) cout << "}"; else cout << '\n';

		if (print_sols) {
			if (mode==MMA) cout << ",{";
			for (vector<SolverOutputBox>::const_iterator it=solutions.begin(); it!=solutions.end(); it++) {
				if (it!=solutions.begin() && mode==MMA) cout << ',';
				if (mode==MMA) cout << "{{";
				const IntervalVector& box=(*it);
				for (int i=0; i<n; i++) {
					if (i>0) cout << s;
					if (mode==MMA) cout << '{';
					cout << box[i].lb() << s << box[i].ub();
					if (mode==MMA) cout << '}';
				}
				if (mode==MMA) cout << '}';
				cout << s << it->status;
				if (m>0 && m<n) {
					cout << s;
					if (mode==MMA) cout << '{';
					if (it->status!=SolverOutputBox::UNKNOWN && it->varset!=NULL) {
						for (int i=0; i<it->varset->nb_param; i++) {
							if (i>0) cout << s;
							cout << (it->varset->param(i)) + 1;
						}
					} else {
						for (int i=0; i<n-m; i++) {
							if (i>0) cout << s;
							cout << '0';
						}
					}
					if (mode==MMA) cout << "},";
				}
				if (mode==MMA) cout << "}"; else cout << endl;
			}
			if (mode==MMA) cout << '}';
		}
		if (mode==MMA) cout << "}\n";
		return;
	}

	switch(status) {
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

	cout << " number of inner boxes : " << nb_inner << endl;
	cout << " number of boundary boxes : " << nb_boundary << endl;
	cout << " number of unknown boxes: " << nb_unknown << endl;
	cout << " cpu time used: " << time << "s." << endl;
	cout << " number of cells: " << nb_cells << endl;

	cout << endl;

	if (print_sols) {
		int i=0;
		for (vector<SolverOutputBox>::const_iterator it=solutions.begin(); it!=solutions.end(); it++) {
			cout << " sol n°" << (i++) << " = " << *it << endl;
		}
	}

}

} // end namespace ibex
