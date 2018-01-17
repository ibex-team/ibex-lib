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

#include "ibex_ExprCopy.h"
#include "ibex_String.h"
#include "ibex_Random.h"
#include "ibex_Timer.h"
#include "ibex_Newton.h"
#include "ibex_Linear.h"
#include <fstream>
#include <iomanip>

using namespace std;

namespace ibex {

int Cont::iteration = 0;

class ChooseFail : public Exception { };


void Cont::add_to_neighbors(ContCell* cell) {
	list<ContCell*> neighbors;

	for(IBEX_NEIGHBORHOOD::iterator it=neighborhood.begin(); it!=neighborhood.end(); it++){
		if(cell->unicity_box.intersects(it->first->unicity_box)) {
			neighbors.push_back(it->first);
			it->second.push_back(cell);
		}
	}

	neighborhood.insert(make_pair(cell,neighbors));
}

//void Cont::update_neighbors(ContCell* cell) {
//	for(IBEX_NEIGHBORHOOD::iterator it=neighborhood.begin(); it!=neighborhood.end(); it++){
//		if(cell->existence_box.intersects(it->first->existence_box)) {
//			neighbors.push_back(it->first);
//			it->second.push_back(cell);
//		}
//	}
//}

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

	return new Function(x,ExprVector::new_col(fg));
}

Cont::Cont(Function &f, Function &g, double h_min, double alpha, double beta) : dfs(false), full_diff(true), n(f.nb_var()+g.image_dim()), m(f.image_dim()+g.image_dim()), f(*merge(f,g)), g(&g), domain(this->f.nb_var()), h_min(h_min), alpha(alpha), beta(beta) {

	for (int i=0; i<g.image_dim(); i++)
		domain[f.nb_var()+i] = Interval::NEG_REALS;
}

Cont::Cont(Function &f, const IntervalVector& domain, double h_min, double alpha, double beta) : dfs(false), full_diff(true), n(f.nb_var()), m(f.image_dim()), f(f), g(NULL), domain(domain), h_min(h_min), alpha(alpha), beta(beta) {

}

Cont::Cont(Function &f, double h_min, double alpha, double beta) : dfs(false), full_diff(true), n(f.nb_var()), m(f.image_dim()), f(f), g(NULL), domain(f.nb_var(),Interval::ALL_REALS), h_min(h_min), alpha(alpha), beta(beta) {

}

ContCell* Cont::next_cell() {
	return dfs? l.back() : l.front();
}

void Cont::remove_next_cell() {
	if (dfs) l.pop_back();
	else l.pop_front();
}

Cont::~Cont() {
	if (g!=NULL) delete &f;

	// Destroy all the cells
	for(IBEX_NEIGHBORHOOD::iterator it=neighborhood.begin(); it!=neighborhood.end(); it++) {
		delete it->first;
	}
}

void Cont::start(IntervalVector x, double h, int kmax) {

	iteration=0; // count the iterations
	RNG::srand(1);

	// Assign the domains of slack variables (extend x)
	if (g!=NULL) {
		x.resize(n);
		if (g->image_dim()==1)
			x[n-1]=g->eval(x);
		else
			x.put(n-g->image_dim(),g->eval_vector(x));
	}

	// The cell/facet to which the current solution x belongs
	// NULL means "none".
	pair<ContCell*,ContCell::Facet*> p(NULL,NULL);

	// Computation times (for profiling)
	choose_time=0;
	find_time=0;
	diff_time=0;
    neighborhood_time=0;
	Timer timer;

	do {

		try {
			//cout << "solution=" << x << endl;
			timer.restart();
			// Build a cell around the current solution x
			ContCell* new_cell=choose(p.second,x,h);
			//cout << "new cell:" << new_cell.box << endl;
			timer.stop();
			choose_time += timer.get_time();
            
            // Update neighborhoods
            timer.restart();
            add_to_neighbors(new_cell);
            timer.stop();
            neighborhood_time+= timer.get_time();

			// Intersects the list of existing cells
			// with the current cell and vice-versa
			timer.restart();
			diff(new_cell);
			timer.stop();
			diff_time+=timer.get_time();

			// assert
			if (full_diff)
				// if full_diff is false, the facet where x has been found is still alive
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
		timer.restart();
		p=find_solution_in_cells(x);
		timer.stop();
		find_time+= timer.get_time();

		// In case of success, the value of h for the next "choose" is set
		// to the parameter width of the cell where the solution has been
		// found (multiplied by beta).
		if (!x.is_empty()) {
			h=p.first->h*beta;
			//h*=1+0.001*(RNG::rand(0,1)-0.5);
		}

//		int i=0;
//		for (list<ContCell>::const_iterator it=l.begin(); it!=l.end(); it++)
//			//cout << "Cell n°" << i++ << endl << *it << endl;
//			cout << it->nb_facets() << " "; //
//		cout << endl;

		iteration++;

		cout << "k=" << iteration << " ";
		cout << "#todo=" << l.size() << " ";
		cout << "#done=" << l_empty_facets.size() << " ";
		cout << "#failed=(" << l_choose_failed_facets.size() << ", ";
		cout <<                l_find_solution_failed_facets.size() << ") ";
		cout << "#facets=" << ContCell::total_facet_count() << " ";
		cout << "h=" << h << " ";
		if (!l.empty()) cout << "(" << l.back()->vars << ")";
		cout << " t=(" << choose_time << "," << diff_time << "," << find_time << "," << neighborhood_time << ")" << endl;

	} while ((kmax==-1 || iteration<kmax) && !l.empty());
}

void Cont::diff(ContCell* new_cell) {

	// No: we can't erase in l using iterators in neighorhood[new_cell] !
	//	for (list<ContCell*>::iterator it=neighborhood[new_cell].begin(); it!=neighborhood[new_cell].end(); ) {

	for (list<ContCell*>::iterator it=l.begin(); it!=l.end(); ) {

		if (!full_diff && ((*it)->vars!=new_cell->vars)) {
			it++;
			continue;
		}

		new_cell->diff((*it)->unicity_box,f,(*it)->vars);

		if (!(*it)->empty_facets()) {
			(*it)->diff(new_cell->unicity_box,f,new_cell->vars);

			if ((*it)->empty_facets()) { // move the cell to the list without facets
				l_empty_facets.push_back(*it);
				it=l.erase(it); // "it" points to the next element
			} else it++;
		} else it++;
	}

	for (list<ContCell*>::iterator it=l_empty_facets.begin(); it!=l_empty_facets.end(); it++) {
		new_cell->diff((*it)->unicity_box,f,(*it)->vars);
	}

	// Try to remove cells in the solution-find-fail list
	for (list<IntervalVector>::iterator it=l_find_solution_failed_facets.begin(); it!=l_find_solution_failed_facets.end(); ) {
		IntervalVector* result;
		int nb_boxes=it->diff(new_cell->unicity_box,result);
		if (nb_boxes>0) {
			for (int i=0; i<nb_boxes; i++) {
				l_find_solution_failed_facets.push_front(result[i]);
			}
		}
		delete[] result;
		it=l_find_solution_failed_facets.erase(it); // it points to the next element
	}

	// Choose where to store the new cell
	if (new_cell->empty_facets())
		l_empty_facets.push_back(new_cell);
	else
		l.push_back(new_cell);
}

bool Cont::is_valid_cell_1_old(const IntervalVector& box_existence, const VarSet& vars, const VarSet& forced_params) {
	// We check the rows of the jacobian matrix of the implicit function.
	// The rows that correspond to variables that "should be parameters"
	// must have no component with 0.

	IntervalMatrix Jp(m,n-m); // Jacobian % parameters
	IntervalMatrix Jx(m,m);   // Jacobian % variables
	f.jacobian(box_existence,Jx,Jp,vars);

	Matrix Jx_mid_inv(m,m);
	real_inverse(Jx.mid(),Jx_mid_inv);

	// Approximate jacobian of the implicit function
	// TODO: make it rigorous!
	IntervalMatrix J_implicit=Jx_mid_inv*Jp;

	int v=0;  // index of the ith variable
	for (int i=0; i<n; i++) {
		if (vars.is_var[i]) {
			if (!forced_params.is_var[i]) {
				for (int j=0; j<n-m; j++) {
					if (J_implicit[v][j].contains(0)) return false;
				}
			}
			v++;
		}
	}
	return true;
}


bool Cont::is_valid_cell_1(const IntervalVector& box_existence, const VarSet& vars, const vector<pair<int,bool> >& wrong_vars) {

	// ========= calculate the Jacobian of the implicit function ===========
	IntervalMatrix Jp(m,n-m); // Jacobian % parameters
	IntervalMatrix Jx(m,m);   // Jacobian % variables
	f.jacobian(box_existence,Jx,Jp,vars);

	Matrix Jx_mid_inv(m,m);
	real_inverse(Jx.mid(),Jx_mid_inv);

	// Approximate jacobian of the implicit function
	// TODO: make it rigorous!
	IntervalMatrix J_implicit=-Jx_mid_inv*Jp;
	// =====================================================================

	// ======= Get the submatrix corresponding to "wrong variables" ========
    IntervalMatrix J_implicit_wrong(wrong_vars.size(),n-m);
    for(unsigned int i=0; i<wrong_vars.size(); i++)
        J_implicit_wrong.set_row(i,J_implicit.row(wrong_vars[i].first));

    // ======= Check that this matrix is full rank =======
    if (!full_rank(J_implicit_wrong)) return false;


//                cout << "x=" << box_existence << endl;
//                cout << "wrong vars: ";
//                if(wrong_vars.size()>=1) cout << wrong_vars[0] << " ";
//                if(wrong_vars.size()>=2) cout << wrong_vars[1] << " ";
//                if(wrong_vars.size()>=3) cout << wrong_vars[2] << " ";
//                cout << endl;
//                cout << J_implicit << endl;
//                cout << endl;
//                cout << J_implicit_wrong << endl;
//                cout << "==================" << endl;


    // ==================================================================
    //     Calculate g(p_inf), the value of the implicit
    //     function at the lower-left corner of the parameter box
    // ==================================================================
    IntervalVector p=vars.param_box(box_existence);
    IntervalVector x_p_inf=vars.full_box(vars.var_box(box_existence),p.lb());
    IntervalVector ginf_existence(m);
    IntervalVector ginf_unicity(m); // will be ignored

    // Calculate g(p_inf) -> solve the system
	if (!inflating_newton(f,vars,x_p_inf,ginf_existence,ginf_unicity)) return false;
    
	// ======= Get the subvector corresponding to "wrong variables" ========
	IntervalVector ginf_wrong(wrong_vars.size());

    for(unsigned int i=0; i<wrong_vars.size(); i++) {
    	int v=wrong_vars[i].first;
    	ginf_wrong[i]=ginf_existence[vars.var(v)];
    	if (wrong_vars[i].second)
    		ginf_wrong[i] -= domain[vars.var(v)].ub();
    	else
    		ginf_wrong[i] -= domain[vars.var(v)].lb();
    }

    // ==================================================================
    //     Check that the rigorous linearization of the
    //     constraint g_wrong<=0 in p is homeomorph to a half-ball,
    //     that is, the implicit function divides the box p in 2^k parts
    //     where k is the number of "wrong variables". Each part
    //     corresponds to a fixed signed (+/-) of a component
    //     of the implicit function.
    bool flag=is_homeomorph_half_ball(ginf_wrong, J_implicit_wrong, p);

//    if (flag==false) {
//    	cout << "x=" << box_existence << endl;
//    	cout << "vars=" << vars << endl;
//    	cout << "wrong vars: ";
//    	if (wrong_vars.size()>=1) cout << wrong_vars[0].first << " (" << wrong_vars[0].second << ") ";
//    	if (wrong_vars.size()>=2) cout << wrong_vars[1].first << " (" << wrong_vars[1].second << ") ";
//    	if (wrong_vars.size()>=3) cout << wrong_vars[2].first << " (" << wrong_vars[2].second << ") ";
//    	cout << endl;
//    	cout << J_implicit << endl;
//    	cout << endl;
//    	cout << J_implicit_wrong << endl;
//    	cout << "==================" << endl;
//    }
    return flag;
}

bool Cont::is_valid_cell_2(const IntervalVector& box_existence, const VarSet& vars, const VarSet& forced_params) {

	bool valid_cell=true;

	IntervalVector box_existence2(n);
	IntervalVector box_unicity2(n);   // ignored

	try {
		// we fix the dimension of the variables that exceed
		// their domain to the "crossed" extremity
		for (int i=0; i<n; i++) {

			if (!box_existence[i].is_subset(domain[i])) {

				IntervalVector box=box_existence & domain;

				if (box_existence[i].lb()<domain[i].lb())
					box[i]=domain[i].lb();
				else
					box[i]=domain[i].ub();

				VarSet tmpvars=get_newton_vars(f, box.mid(), forced_params);

				valid_cell=inflating_newton(f,tmpvars,box,box_existence2,box_unicity2);

				for (int j=0; valid_cell && j<n; j++) {
					valid_cell &= (!tmpvars.is_var[j] || box_existence2[j].is_subset(domain[j]));
				}

				if (!valid_cell) break;
			}
		}
	} catch(SingularMatrixException&) {
		valid_cell=false;
		// May happen if we are near a corner of the bounding domain:
		// some dimension are forced to be parameter erroneously. In
		// this case, we have to decrease h...
	}

	return valid_cell;
}

ContCell* Cont::choose(const ContCell::Facet* x_facet, const IntervalVector& x, double h) {
	IntervalVector x_box(m);
	IntervalVector p_box(n-m);
	IntervalVector p_box_h(n-m);
	IntervalVector box(n);
	IntervalVector box_unicity(n);
	IntervalVector box_existence(n);
	VarSet vars=get_newton_vars(f, x.mid(), VarSet(n,BitSet::empty(n),false));

	x_box=vars.var_box(x);
	p_box=vars.param_box(x);

	// IntervalVector g_box(g? g->image_dim() : 1 /* unused if g==NULL*/);

	// Have we found a new cell?
	bool success;

	// Is this cell valid?
	// If the cell is inside the domain, it is valid. If the cell crosses the
	// boundary of the domain, the manifold inside the intersection of the cell
	// and the domain must be homeomorph to a half-ball.
	bool valid_cell;

	do {

		valid_cell=true; // by default

		// We need to recalculate variables even when success==false
		// because the existence box may cross the boundary
		// of the domain but the manifold may not.

		try {

			IntervalVector shift(vars.nb_param,Interval(-h,h));

			// minimize overlapping with the x-cell by shifting
//			if (x_facet!=NULL)
//				shift[x_facet->p] = h*(x_facet->sign? Interval(-0.01,1.99) : Interval(-1.99,0.01));

			p_box_h = p_box + shift;
			box=vars.full_box(x_box, p_box_h);

			success=inflating_newton(f,vars,box,box_existence,box_unicity);

		} catch(SingularMatrixException&) {
			// should not happen
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

		// Dimensions that should be parameters (by default: none).
		BitSet forced_params(BitSet::empty(n));

		// The indices of variables (among the variables, not all the dimensions)
        // that violate a bound of the domain + a boolean which indicates the bound
		// (false=lower bound, true=upper bound)
		vector<pair<int,bool> > wrong_vars;

		if(success) { // check if the cell is valid

			// Check if the cell crosses the domain only
			// in the dimension of parameters.

			// Find dimensions along with the box exceeds domain
			// and mark them has "should be parameters"
           int var_number=0;
			for (int i=0; i<n; i++) {
				if (!box_existence[i].is_subset(domain[i])) {
                    forced_params.add(i);
                    if (vars.is_var[i]) {
                    	if (box_existence[i].lb() < domain[i].lb()) {
                    		if (box_existence[i].ub() > domain[i].ub()) {
                    			success=false; // two bounds violated for the same variable => abort
                    			break;
                    		} else {
                    			valid_cell=false;
                    			wrong_vars.push_back(make_pair(var_number,false));
                    		}
                    	} else if (box_existence[i].ub() > domain[i].ub()) {
                    		valid_cell=false;
                    		wrong_vars.push_back(make_pair(var_number,true));
                    	}
                    }
				}
                if (vars.is_var[i]) var_number++;
			}
		}

//		if (success && !valid_cell) {
//			valid_cell = is_valid_cell_1_old(box_existence,vars,forced_params);
//        }

		if (success && !valid_cell) {
			valid_cell = is_valid_cell_1(box_existence,vars,wrong_vars);
        }

//		if (success && !valid_cell) {
//			valid_cell = is_valid_cell_2(box_existence,vars,forced_params);
//        }
		//============================================================
        
		if (success && valid_cell) {
			ContCell* cell = new ContCell(box_existence,box_unicity,domain,vars);

			// Contract each facet separately
			CtcParamNewton ctc(f,vars);
			cell->contract_facets(ctc);

			return cell;
		} else {
			h = alpha*h;
		}

	} while(h>=h_min);

	throw ChooseFail();
}

pair<ContCell*,ContCell::Facet*> Cont::find_solution_in_cells(IntervalVector& x) {
	x.set_empty(); // by default

	int i=0; // count the cells (for debug)

	while (x.is_empty() && !l.empty()) {

		//cout << "[find_solution] lookup in cell n°" << i << endl;

		ContCell* cell=next_cell();

		try {

			if (full_diff)
				cell->find_solution_in_facets(f,x);
			else
				cell->find_solution_in_facets_not_in(f,x,neighborhood[cell]);

			// No solution in all the remaining facets => remove the cell
			if (x.is_empty()) {
				l_empty_facets.push_back(cell);
				remove_next_cell();
			} else {
				return pair<ContCell*,ContCell::Facet*>(cell,&cell->facets.front());
			}
		} catch(FindSolutionFail&) {
			// Try to remove the facet using inflating Newton
			IntervalVector facet2(x.size());
			IntervalVector _ignore_(x.size());
			bool success=inflating_newton(f,cell->vars,cell->facets.front().facet,facet2,_ignore_);

			if (success) {

				cell->facets.front().facet &= facet2;

				if (cell->facets.front().facet.is_empty()) {
					cell->pop_front_facet();
					continue;
				}
			}

			move_facet_to_fails(false);

		}
		i++;
	}

	return pair<ContCell*,ContCell::Facet*>(NULL,NULL);
}

void Cont::move_facet_to_fails(bool choose) {
	// only happens if the initial point is not feasible
	if (l.empty()) {
		cerr << "Initial point infeasible" << endl;
		exit(1);
	}

	ContCell* cell=next_cell();

	if (choose)
		l_choose_failed_facets.push_back(cell->pop_front_facet());
	else
		l_find_solution_failed_facets.push_back(cell->pop_front_facet());

	if (cell->empty_facets()) {
		l_empty_facets.push_back(cell);
		remove_next_cell();
	}
}

void Cont::check_no_facet_contains(const IntervalVector& x) {
	for (list<ContCell*>::iterator it=l.begin(); it!=l.end(); it++) {
		(*it)->check_no_facet_contains(x);
	}
}

void Cont::cells_to_mathematica(const list<ContCell*>& l, const string& filename) {
    ofstream file (filename.c_str());
    file << '{';
    int count=0;
    for (list<ContCell*>::const_iterator it=l.begin(); it!=l.end(); it++) {
        if (it!=l.begin()) file << ',';
        file << '{';
        const IntervalVector& box=(*it)->unicity_box;
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

void Cont::boxes_to_mathematica(const list<IntervalVector>& l, const string& filename) {
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

void Cont::to_mathematica(const string& basename) const {
    cells_to_mathematica(l_empty_facets,basename+".txt");
    boxes_to_mathematica(l_choose_failed_facets,basename+"-choose-failed.txt");
    boxes_to_mathematica(l_find_solution_failed_facets,basename+"-find-failed.txt");
    cells_to_mathematica(l,basename+"-todo.txt");

    list<IntervalVector> todo_facets;
    for (list<ContCell*>::const_iterator it=l.begin(); it!=l.end(); it++) {
    	for (list<ContCell::Facet>::const_iterator it2=(*it)->facets.begin(); it2!=(*it)->facets.end(); it2++) {
    		todo_facets.push_back(it2->facet); //(todo_facets.end(), it->facets.begin(), it->facets.end());
    	}
    }

    boxes_to_mathematica(todo_facets,basename+"-todo-facets.txt");
}

} // namespace ibex
