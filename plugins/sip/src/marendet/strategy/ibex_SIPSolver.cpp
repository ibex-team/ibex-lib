/* ============================================================================
 * I B E X - ibex_SIPSolver.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_SIPSolver.h"

#include "ibex_CellBufferNeighborhood.h"
#include "ibex_Cell.h"
#include "ibex_Interval.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_Solver.h"
#include "ibex_SIPSolverOutputBox.h"
#include "ibex_SIPManifold.h"

#include <cassert>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

using namespace std;

namespace ibex {

namespace {
class EmptyBoxException : Exception {
};
}

SIPSolver::SIPSolver(const SIPSystem& sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer, const Vector& eps_x_min,
    const Vector& eps_x_max, bool pathFinding)
    : ctc(ctc)
    , bsc(bsc)
    , buffer(buffer)
    , eps_x_min(eps_x_min)
    , eps_x_max(eps_x_max)
    , boundary_test(SIPSolver::ALL_TRUE)
    , time_limit(-1)
    , cell_limit(-1)
    , trace(0)
    , impact(BitSet::all(ctc.nb_var))
    , solve_init_box(sys.extractInitialBox())
    , pathFinding(pathFinding)
    , eqs(NULL)
    , ineqs(NULL)
    , params(sys.nb_var, BitSet::empty(sys.nb_var),
          false) /* no forced parameter by default */
    , manif(NULL)
    , time(0)
    , nb_cells(0)
{

    assert(solve_init_box.size() == ctc.nb_var);

    int nb_eq = 0;

    /*************** DONT SUPPORT EQUALITIES FOR NOW ********************
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
	 */
    ineqs = &sys;
    eqs = NULL;

    n = sys.nb_var;
    //m=eqs? eqs->f_ctrs.image_dim() : 0;
    m = 0;

    //nb_ineq=ineqs? ineqs->f_ctrs.image_dim() : 0;
    nb_ineq = sys.normal_constraints_.size() + sys.sic_constraints_.size();

    if (m == 0 || m == n)
        boundary_test = SIPSolver::ALL_FALSE;

    manif = new SIPManifold(n, m, nb_ineq);
}

void SIPSolver::set_params(const VarSet& _params)
{
    params = _params;
}

SIPSolver::~SIPSolver()
{
    // ineqs is a ref to the original system
    /*if (ineqs) {
		delete ineqs;
		if (eqs) {
			delete eqs;
		}
	}*/
    if (manif)
        delete manif;
}

void SIPSolver::start(const IntervalVector& init_box)
{
    buffer.flush();

    if (manif)
        delete manif;

    manif = new SIPManifold(n, m, nb_ineq);

    Cell* root = new Cell(init_box);

    // add data required by this solver
    ctc.add_property(init_box, root->prop);

    // add data required by the bisector
    bsc.add_property(init_box, root->prop);

    buffer.push(root);
    nb_cells = 1;

    time = 0;

    timer.restart();
}

void SIPSolver::start(const char* input_paving)
{
    buffer.flush();

    if (manif)
        delete manif;
    manif = new SIPManifold(n, m, nb_ineq);
    manif->load(input_paving);

    vector<SIPSolverOutputBox>::const_iterator it = manif->unknown.begin();

    // the unknown and pending boxes have to be processed
    while (it != manif->pending.end()) {
        if (it == manif->unknown.end())
            it = manif->pending.begin();
        if (it == manif->pending.end())
            break;

        Cell* cell = new Cell(it->existence());

        // add data required by this solver
        ctc.add_property(it->existence(), cell->prop); // Not efficient...

        // add data required by the bisector
        bsc.add_property(it->existence(), cell->prop);

        buffer.push(cell);

        it++;
    }

    nb_cells = 0; // no new cell created!

    manif->unknown.clear();
    manif->pending.clear();

    timer.restart();
}

SIPSolverOutputBox* SIPSolver::next()
{
    while (!buffer.empty()) {
        if (time_limit > 0)
            timer.check(time_limit);

        if (trace == 2)
            cout << buffer << endl;

        Cell* c = buffer.top();
        if (pathFinding) {
            const auto pathBuffer = static_cast<CellBufferNeighborhood*>(&buffer);
            if (pathBuffer->isPathFound) {
                const auto path = pathBuffer->pathFound;
				cout << "Path, in Mathematica format as a sequence of vectors (temp):" << endl << endl;
                std::cout << "{";
				for(size_t i = 0; i < path.size()-1; ++i) {
                    std::cout << "{" << path[i].mid()[0] << ", " << path[i].mid()[1] << "}, ";
                }
				std::cout << "{" << path[path.size()-1].mid()[0] << ", " << path[path.size()-1].mid()[1] << "}";
                std::cout << "}";
				std::cout << endl << endl;
                throw PathFoundException();
            }
        }
    	//BxpNodeData* data=(BxpNodeData*) c->prop[BxpNodeData::id];
    	//if (!data) ibex_error("[ibexopt-sip]: no node data!");
    	//BxpNodeData::sip_system->loadBxpNodeData(data);

        /*const auto& list = c->get<BxpNodeData>().sic_constraints_caches[0].parameter_caches_;
		 for(const auto& param_cache : list) {
		 cout << param_cache.parameter_box << " eval=" << param_cache.evaluation <<  endl;
		 }*/
        ContractContext context(c->prop);

		int v=c->bisected_var; // last bisected var.

		if (v!=-1) { // not the root node :  impact set to the last bisected variable only
			context.impact = BitSet::singleton(n,v);
		}

        try {
            ctc.contract(c->box, context);
            //std::cout << c->box << std::endl;

            if (c->box.is_empty())
                throw EmptyBoxException();

            /*if (v != -1)
                impact.remove(v);
            else
                // root node : impact set to 0 for all variables after contraction
                impact.clear();*/

            // certification is performed at each intermediate step
            // if the system is under constrained
            if (!c->box.is_empty() && m < n) {
                // note: cannot return PENDING status
                SIPSolverOutputBox new_sol = check_sol(*c);
                if (new_sol.status != SIPSolverOutputBox::UNKNOWN) {
                    if ((m == 0 && new_sol.status == SIPSolverOutputBox::INNER) || !is_too_large(new_sol.existence())) {
                        delete buffer.pop();
                        return &store_sol(new_sol);
                    } else {
                        // otherwise: continue search...
                    }
                } else {
                    // otherwise: continue search...
                }
            }

            try {
                if (is_too_small(c->box))
                    throw NoBisectableVariableException();

                // next line may also throw NoBisectableVariableException
                pair<Cell*, Cell*> new_cells = bsc.bisect(*c);

                delete buffer.pop();
                buffer.push(new_cells.first);
                buffer.push(new_cells.second);
                nb_cells += 2;
                if (cell_limit >= 0 && nb_cells >= cell_limit)
                    throw CellLimitException();
            }

            catch (NoBisectableVariableException&) {
                SIPSolverOutputBox new_sol = check_sol(*c);
                delete buffer.pop();
                return &store_sol(new_sol);
            }
        } catch (EmptyBoxException&) {
            delete buffer.pop();
            //impact.remove(v); // note: in case of the root node, we should clear the bitset
            // instead but since the search is over, the impact is not used anymore.
            // JN: that make a bug with Mingw
            if (v != -1) // no root node :  impact set to 1 for last bisected var only
                impact.remove(v);
            else
                // root node : impact set to 1 for all variables
                impact.clear();

            continue;
        }
    }

    return NULL;
}

SIPSolver::Status SIPSolver::solve(const IntervalVector& init_box)
{
    start(init_box);
    return solve();
}

SIPSolver::Status SIPSolver::solve(const char* init_paving)
{
    start(init_paving);
    return solve();
}

SIPSolver::Status SIPSolver::solve()
{

    try {

        while (next() != NULL) {
        }

        if (manif->unknown.size() > 0)
            manif->status = SIPSolver::NOT_ALL_VALIDATED;
        else if (manif->inner.size() > 0 || manif->boundary.size() > 0)
            manif->status = SIPSolver::SUCCESS;
        else
            manif->status = SIPSolver::INFEASIBLE;
    } catch (TimeOutException&) {
        flush();
        manif->status = SIPSolver::TIME_OUT;
    } catch (CellLimitException&) {
        flush();
        manif->status = SIPSolver::CELL_OVERFLOW;
    } catch (PathFoundException&) {
        flush();
        manif->status = SIPSolver::PATH_FOUND;
    }

    timer.stop();
    time = timer.get_time();

    manif->time += time;
    manif->nb_cells += nb_cells;

    return manif->status;
}

SIPSolverOutputBox SIPSolver::check_sol(const Cell& c)
{

    SIPSolverOutputBox sol(n);

    (SIPSolverOutputBox::sol_status&)sol.status = SIPSolverOutputBox::INNER; // by default
    /******* NO EQS ***********
	 if (eqs) {

	 sol._unicity=new IntervalVector(n);

	 if (m>n) {
	 ibex_warning("Certification not implemented for over-constrained systems ");
	 (SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;
	 } else if (m<n) {
	 // ====== under-constrained =========
	 try {
	 VarSet varset=get_newton_vars(eqs->f_ctrs,box.mid(),params);

	 if (inflating_newton(eqs->f_ctrs, varset, box, sol._existence, *sol._unicity)) {
	 if (params.nb_param<n-m)
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
	 }*/
    //} else {
    sol._existence = c.box;
    sol._unicity = NULL;
    //}

    if (sol.status == SIPSolverOutputBox::UNKNOWN) {
        sol._existence = c.box;
        if (sol._unicity != NULL)
            delete sol._unicity;
        sol._unicity = NULL;
    } else {
        // The inflating Newton may cause the existence box to be disjoint from the input box.

        // Note that the following line also tests the case of an existence box outside
        // the initial box of the search
        if (c.box.is_disjoint(sol.existence())) {
            throw EmptyBoxException();
        }
    }

    if (sol.status == SIPSolverOutputBox::INNER && !sol.existence().is_subset(solve_init_box))
        // BOUNDARY by default: will be verified later
        (SIPSolverOutputBox::sol_status&)sol.status = SIPSolverOutputBox::BOUNDARY;

    if (ineqs) {
        Interval y, r;
        for (int i = 0; i < ineqs->sic_constraints_.size(); i++) {
            //NumConstraint& c=ineqs->ctrs[i];
            //assert(c.f.image_dim()==1);
            //y=c.f.eval(sol.existence());
            auto& cache = ((BxpNodeData*)c.prop[BxpNodeData::id])->sic_constraints_caches[i];
            y = ineqs->sic_constraints_[i].evaluateWithoutCachedValue(sol.existence(), cache);
            //r=c.right_hand_side().i();
            r = Interval::NEG_REALS; // Constraint is always scalar and <= 0
            if (y.is_disjoint(r)) {
                throw EmptyBoxException();
            }
            if (sol.status == SIPSolverOutputBox::INNER && !y.is_subset(r)) {
                // BOUNDARY by default: will be verified later
                (SIPSolverOutputBox::sol_status&)sol.status = SIPSolverOutputBox::BOUNDARY;
            }
        }
        for (int i = 0; i < ineqs->normal_constraints_.size(); i++) {
            //NumConstraint& c=ineqs->ctrs[i];
            //assert(c.f.image_dim()==1);
            //y=c.f.eval(sol.existence());
            y = ineqs->normal_constraints_[i].evaluate(sol.existence());
            //r=c.right_hand_side().i();
            r = Interval::NEG_REALS; // Constraint is always scalar and <= 0
            if (y.is_disjoint(r)) {
                throw EmptyBoxException();
            }
            if (sol.status == SIPSolverOutputBox::INNER && !y.is_subset(r)) {
                // BOUNDARY by default: will be verified later
                (SIPSolverOutputBox::sol_status&)sol.status = SIPSolverOutputBox::BOUNDARY;
            }
        }
    }

    if (sol.status == SIPSolverOutputBox::BOUNDARY) {
        if (!is_boundary(sol.existence()))
            (SIPSolverOutputBox::sol_status&)sol.status = SIPSolverOutputBox::UNKNOWN;
    }

    if (eqs && n == m) {
        // Check if the solution is new, that is, that the solution is not included in the unicity
        // box of a previously found solution. For efficiency reason, this test is not performed in
        // the case of under-constrained systems (m<n).

        for (vector<SIPSolverOutputBox>::iterator it = manif->inner.begin(); it != manif->inner.end(); it++) {
            if (it->unicity().is_superset(sol._existence))
                throw EmptyBoxException();
        }
    }

    return sol;
}

bool SIPSolver::is_boundary(const IntervalVector& box)
{
    // Always FULL_RANK because m = 0
    return false;
    /*
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
	 */
}

bool SIPSolver::is_too_small(const IntervalVector& box)
{
    for (int i = 0; i < n; i++)
        if (box[i].diam() > eps_x_min[i])
            return false;
    return true;
}

bool SIPSolver::is_too_large(const IntervalVector& box)
{
    for (int i = 0; i < n; i++)
        if (box[i].diam() > eps_x_max[i])
            return true;
    return false;
}

SIPSolverOutputBox& SIPSolver::store_sol(const SIPSolverOutputBox& sol)
{

    if (trace >= 1)
        cout << sol << endl;

    switch (sol.status) {
    case SIPSolverOutputBox::INNER:
        manif->inner.push_back(sol);
        if (pathFinding) {
            static_cast<CellBufferNeighborhood*>(&buffer)->pushInner(new Cell(sol.existence()));
        }
        return manif->inner.back();
    case SIPSolverOutputBox::BOUNDARY:
        manif->boundary.push_back(sol);
        return manif->boundary.back();
    case SIPSolverOutputBox::UNKNOWN:
        manif->unknown.push_back(sol);
        return manif->unknown.back();
    case SIPSolverOutputBox::PENDING:
    default:
        manif->pending.push_back(sol);
        return manif->pending.back();
    }
}

void SIPSolver::flush()
{
    while (!buffer.empty()) {
        Cell* cell = buffer.top();
        if (cell == nullptr)
            break;
        SIPSolverOutputBox sol(n);
        (SIPSolverOutputBox::sol_status&)sol.status = SIPSolverOutputBox::PENDING;
        sol._existence = cell->box;
        sol._unicity = NULL;
        store_sol(sol);
        delete buffer.pop();
    }
    buffer.flush();
}

void SIPSolver::report()
{

    switch (manif->status) {
    case SIPSolver::SUCCESS:
        cout << "\033[32m"
             << " solving successful!" << endl;
        break;
    case SIPSolver::INFEASIBLE:
        cout << "\033[31m"
             << " infeasible problem" << endl;
        break;
    case SIPSolver::NOT_ALL_VALIDATED:
        cout << "\033[31m"
             << " done! but some boxes have 'unknown' status." << endl;
        break;
    case SIPSolver::TIME_OUT:
        cout << "\033[31m"
             << " time limit " << time_limit << "s. reached " << endl;
        break;
    case SIPSolver::CELL_OVERFLOW:
        cout << "\033[31m"
             << " cell overflow" << endl;
    case SIPSolver::PATH_FOUND:
        cout << "\033[32m"
             << " path found!" << endl;
    }

    cout << "\033[0m" << endl;

    cout << " number of inner boxes:\t\t" << manif->inner.size() << endl;
    cout << " number of boundary boxes:\t" << manif->boundary.size() << endl;
    cout << " number of unknown boxes:\t" << manif->unknown.size() << endl;
    cout << " number of pending boxes:\t" << manif->pending.size() << endl;
    cout << " cpu time used:\t\t\t" << time << "s";
    if (manif->time != time)
        cout << " [total=" << manif->time << "]";
    cout << endl;
    cout << " number of cells:\t\t" << nb_cells;
    if (manif->nb_cells != nb_cells)
        cout << " [total=" << manif->nb_cells << "]";
    cout << endl
         << endl;
}
} // end namespace ibex
