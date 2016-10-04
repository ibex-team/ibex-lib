/* ============================================================================
 * I B E X - Continuation (main algorithm)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 *
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#include "ibex_Cont.h"
#include "ibex_ParametricProof.h"
#include "ibex.h"

#include <fstream>

using namespace std;

namespace ibex {

int Cont::iteration = 0;

class ChooseFail : public Exception { };


Function* Cont::merge(Function &f, Function& g) {
	int N=f.nb_arg();

	// copy the original variables
	Array<const ExprSymbol> x(N);
	varcopy(f.args(),x); // get the right dimensions and names

	// create |g| slack variables
	Array<const ExprSymbol> y(g.image_dim());
	for (int j=0; j<g.image_dim(); j++) {
		const char* name=append_index("slack",'_','_',j);
		y.set_ref(j,ExprSymbol::new_(name));
	}

	Array<const ExprNode> fg(f.image_dim() + g.image_dim());

	int i=0;
	for (int j=0; j<f.image_dim(); j++)
		fg.set_ref(i++, (f[j]((Array<const ExprNode>&) x)));

	for (int j=0; j<g.image_dim(); j++)
		fg.set_ref(i++, (g[j]((Array<const ExprNode>&) x)-y[j]));

	x.add(y);

	return new Function(x,ExprVector::new_(fg,false));
}

Cont::Cont(Function &f, Function &g, double h_min, double alpha, double beta) : n(f.nb_var()+g.image_dim()), m(f.image_dim()+g.image_dim()), f(*merge(f,g)), domain(this->f.nb_var()), g(&g), h_min(h_min), alpha(alpha), beta(beta), dfs(false) {

	for (int i=0; i<g.image_dim(); i++)
		domain[f.nb_var()+i] = Interval::NEG_REALS;
}

Cont::Cont(Function &f, const IntervalVector& domain, double h_min, double alpha, double beta) : n(f.nb_var()), m(f.image_dim()), f(f), g(NULL), domain(domain), h_min(h_min), alpha(alpha), beta(beta), dfs(false) {

}

Cont::Cont(Function &f, double h_min, double alpha, double beta) : n(f.nb_var()), m(f.image_dim()), f(f), domain(f.nb_var(),Interval::ALL_REALS), g(NULL), h_min(h_min), alpha(alpha), beta(beta), dfs(false) {

}

ContCell& Cont::next_cell() {
	return dfs? l.back() : l.front();
}

void Cont::remove_next_cell() {
	if (dfs) l.pop_back();
	else l.pop_front();
}

Cont::~Cont() {
	if (g!=NULL) delete &f;
}

void Cont::start(IntervalVector x, double h, int kmax) {

	iteration=0; // count the iterations

	if (g!=NULL) {
		x.resize(n);
		if (g->image_dim()==1)
			x[n-1]=g->eval(x);
		else
			x.put(n-g->image_dim(),g->eval_vector(x));
	}

	do {

		try {
			//cout << "solution=" << x << endl;

			// Build a cell around the current solution x
			ContCell new_cell=choose(x,h);
			//cout << "new cell:" << new_cell.box << endl;

			// Intersects the list of existing cells
			// with the current cell and vice-versa
			diff(new_cell);

			// assert
			check_no_facet_contains(x);

		} catch(ChooseFail&) {
			//cout << "ChooseFail:" << endl;
			//cout << "  x="  << x << " vars=" << vars << endl;
			//cout << "  h=" << h << endl;

			move_facet_to_fails(true);
		}

		// look for a new solution point by considering each
		// cell in turn and search in all its facets.
		// Some facets may be discarded and some cells may be moved
		// to l_empty_facets or l_solution_find_fail_facets.
		cout << "k=" << iteration << " ";
		cout << "#todo=" << l.size() << " ";
		cout << "#done=" << l_empty_facets.size() << " ";
		cout << "#failed=(" << l_choose_failed_facets.size() << ", ";
		cout <<                l_find_solution_failed_facets.size() << ") ";
		cout << "#facets=" << ContCell::total_facet_count() << " ";
		cout << "h=" << h << " ";
		if (!l.empty()) cout << "(" << l.back().vars << ")";
		cout << endl;

		h=find_solution_in_cells(x);
		h*=1+0.001*(RNG::rand(0,1)-0.5);

		int i=0;
//		for (list<ContCell>::const_iterator it=l.begin(); it!=l.end(); it++)
//			//cout << "Cell n°" << i++ << endl << *it << endl;
//			cout << it->nb_facets() << " "; //
//		cout << endl;
		iteration++;
	} while ((kmax==-1 || iteration<kmax) && !l.empty());
}

void Cont::diff(ContCell& ivb) {
	for (list<ContCell>::iterator it=l.begin(); it!=l.end(); ) {
		it->diff(ivb.box,f);
		ivb.diff(it->box,f);

		if (it->empty_facets()) { // move the cell to the list without facets
			l_empty_facets.push_back(*it);
			it=l.erase(it); // it points to the next element
		} else it++;
	}

	for (list<ContCell>::iterator it=l_empty_facets.begin(); it!=l_empty_facets.end(); it++) {
		ivb.diff(it->box,f);
	}

	// Try to remove cells in the solution-find-fail list
	for (list<IntervalVector>::iterator it=l_find_solution_failed_facets.begin(); it!=l_find_solution_failed_facets.end(); ) {
		IntervalVector* result;
		int nb_boxes=it->diff(ivb.box,result);
		if (nb_boxes>0) {
			for (int i=0; i<nb_boxes; i++) {
				l_find_solution_failed_facets.push_front(result[i]);
			}
		}
		delete[] result;
		it=l_find_solution_failed_facets.erase(it); // it points to the next element
	}

	// Choose where to store the new cell
	if (ivb.empty_facets())
		l_empty_facets.push_back(ivb);
	else
		l.push_back(ivb);
}

ContCell Cont::choose(const IntervalVector& x, double h) {
	IntervalVector x_box(m);
	IntervalVector p_box(n-m);
	IntervalVector p_box_h(n-m);
	IntervalVector box(n);
	IntervalVector box_unicity(n);
	IntervalVector box_existence(n);
	VarSet vars(n,BitSet::empty(n),true); // ignore init values

	// Dimensions that have to be parameters (by default: none).
	BitSet forced_params(BitSet::empty(n));

	// IntervalVector g_box(g? g->image_dim() : 1 /* unused if g==NULL*/);

	bool success;         // have we found a new cell?

	do {

		// We need to recalculate variables even when success==false
		// because the existence box may cross the boundary
		// of the domain but the manifold may not.

		try {

			vars=get_vars(f, x.mid(), forced_params);
			x_box=vars.var_box(x);
			p_box=vars.param_box(x);
			p_box_h = p_box + IntervalVector(vars.nb_param,Interval(-h,h));
			box=vars.full_box(x_box, p_box_h);

			success=inflating_newton(f,vars,box,box_existence,box_unicity);

		} catch(SingularMatrixException&) {
			// May happen if we are near a corner of the bounding domain:
			// some dimension are forced to be parameter erroneously. In
			// this case, we have to decrease h.
			success = false;
		}

		/*
		if(success && g!=NULL){
            if(g->image_dim()==1)
                g_box[0]=g->eval(box_existence);
            else
                g_box=g->eval_vector(box_existence);
            if(! (g_box.ub().max()<=0) )
                success=false;
        }*/

		if(success) {
			bool correct_params=true;  // have we chose correct parameters?

			// Find dimensions along with the box exceeds domain
			// and force them to be parameters
			for (int i=0; i<n; i++) {
				if (!box_existence[i].is_subset(domain[i])) {
					forced_params.add(i);
					if (vars.vars[i]) correct_params=false;
				}
			}

			if (correct_params) {
				ContCell cell(box_existence,box_unicity,domain,vars);

				// Contract each facet separately
				CtcParamNewton ctc(f,vars);
				cell.contract_facets(ctc);

				return cell;
			} else {
				// note: we don't change h in this case
			}
		} else {
			h = alpha*h;

			// No risk of infinite loop because h -> 0
			forced_params = BitSet::empty(n);
		}


	} while(h>=h_min);

	throw ChooseFail();
}

double Cont::find_solution_in_cells(IntervalVector& x) {
	x.set_empty(); // by default

	int i=0; // count the cells (for debug)

	while (x.is_empty() && !l.empty()) {

		//cout << "[find_solution] lookup in cell n°" << i << endl;

		ContCell& cell=next_cell();

		try {
			cell.find_solution_in_facets(f,x);

			// No solution in all the remaining facets => remove the cell
			if (x.is_empty()) {
				l_empty_facets.push_back(cell);
				remove_next_cell();
			} else {
				return cell.h*beta;
			}
		} catch(FindSolutionFail&) {
			// Try to remove the facet using inflating Newton
			IntervalVector facet2(x.size());
			IntervalVector _ignore_(x.size());
			bool success=inflating_newton(f,cell.vars,cell.facets.front(),facet2,_ignore_);

			if (success) {

				cell.facets.front() &= facet2;

				if (cell.facets.front().is_empty()) {
					cell.pop_front_facet();
					continue;
				}
			}

			move_facet_to_fails(false);

		}
		i++;
	}

	return -1;
}

void Cont::move_facet_to_fails(bool choose) {
	// only happens if the initial point is not feasible
	if (l.empty()) {
		cerr << "Initial point infeasible" << endl;
		exit(1);
	}

	ContCell& cell=next_cell();

	if (choose)
		l_choose_failed_facets.push_back(cell.pop_front_facet());
	else
		l_find_solution_failed_facets.push_back(cell.pop_front_facet());

	if (cell.empty_facets()) {
		l_empty_facets.push_back(cell);
		remove_next_cell();
	}
}

void Cont::check_no_facet_contains(const IntervalVector& x) {
	for (list<ContCell>::iterator it=l.begin(); it!=l.end(); it++) {
		it->check_no_facet_contains(x);
	}
}

void Cont::cells_to_mathematica(const list<ContCell>& l, const std::string& filename) const {
    ofstream file (filename.c_str());
    file << '{';
    int count=0;
    for (list<ContCell>::const_iterator it=l.begin(); it!=l.end(); it++) {
        if (it!=l.begin()) file << ',';
        file << '{';
        const IntervalVector& box=it->box;
        for (int j=0; j<box.size(); j++) {
            file << '{' << box[j].lb() << ',' << box[j].ub() << '}';
            if (j<box.size()-1) file << ',';
        }
        file << '}';
        count++;
    }
    file << '}' << '\n';
    file.close();
}

void Cont::boxes_to_mathematica(const list<IntervalVector>& l, const string& filename) const {
    ofstream file (filename.c_str());
    file << '{';
    int count=0;
    for (list<IntervalVector>::const_iterator it=l.begin(); it!=l.end(); it++) {
        if (it!=l.begin()) file << ',';
        file << '{';
        const IntervalVector& box=*it;
        for (int j=0; j<box.size(); j++) {
            file << '{' << box[j].lb() << ',' << box[j].ub() << '}';
            if (j<box.size()-1) file << ',';
        }
        file << '}';
        count++;
    }
    file << '}' << '\n';
    file.close();
}

void Cont::to_mathematica(const std::string& basename) const {
    cells_to_mathematica(l_empty_facets,basename+".txt");
    boxes_to_mathematica(l_choose_failed_facets,basename+"-choose-failed.txt");
    boxes_to_mathematica(l_find_solution_failed_facets,basename+"-find-failed.txt");
    cells_to_mathematica(l,basename+"-todo.txt");

    list<IntervalVector> todo_facets;
    for (list<ContCell>::const_iterator it=l.begin(); it!=l.end(); it++) {
    	todo_facets.insert(todo_facets.end(), it->facets.begin(), it->facets.end());
    }

    boxes_to_mathematica(todo_facets,basename+"-todo-facets.txt");
}

} /* namespace ibex */
