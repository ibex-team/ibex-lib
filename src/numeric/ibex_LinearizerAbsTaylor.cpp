/* ============================================================================
 * I B E X - AbsTaylor linearizer
 * ============================================================================
 *
 * Author(s)   : Ignacio Araya, Victor Reyes
 * Created     : May 2021
 * Updated     : May 2021
 * ---------------------------------------------------------------------------- */

#include "ibex_LinearizerAbsTaylor.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_Random.h"
#include "ibex_Exception.h"
#include "ibex_NormalizedSystem.h"

#include <vector>

using namespace std;

namespace ibex {

namespace {
	class Unsatisfiability : public Exception { };
	class NoExpansionPoint : public Exception { };
}

LinearizerAbsTaylor::LinearizerAbsTaylor(const System& _sys, point_policy point):
		Linearizer(_sys.nb_var), sys(_sys),
			m(sys.f_ctrs.image_dim()), goal_ctr(-1 /*tmp*/),
			lp_solver(NULL), point(point) {

	if (dynamic_cast<const ExtendedSystem*>(&sys)) {
		((int&) goal_ctr)=((const ExtendedSystem&) sys).goal_ctr();
	}

}

LinearizerAbsTaylor::~LinearizerAbsTaylor() {

}

int LinearizerAbsTaylor::linearize(const IntervalVector& box, LPSolver& _lp_solver)  {
	lp_solver = &_lp_solver;
	IntervalVector box2(n*2);
	for(int i=0;i<n;i++)
		box2[i]=box[i];
	//initialize auxiliary variables u_i
	for(int i=0;i<n;i++)
		box2[n+i]=Interval(-box2[i].mag(), box2[i].mag());
	lp_solver->set_bounds(box2);
	return linear_restrict(box);
}

int LinearizerAbsTaylor::linear_restrict(const IntervalVector& box) {

	// expansion point
	Vector exp_point(box.size());
	if (point == MID)
		exp_point = box.mid();
	else if (point == RANDOM){
		for (int i = 0 ; i < box.size() ; i++)
			exp_point[i] = RNG::rand(box[i].lb(),box[i].ub());
	}

	BitSet active=sys.active_ctrs(box);
	if (active.empty()) return 0;

	try {

		IntervalMatrix J=sys.f_ctrs.jacobian(box,active);
		//IntervalMatrix J=sys.active_ctrs_jacobian(box);  // --> better with SystemBox

		if (J.is_empty()) return -1; // note: no way to inform that the box is actually infeasible

		IntervalVector g_mid(sys.f_ctrs.eval_vector(exp_point,active));
		if (g_mid.is_empty()) return -1;

		// total number of added constraint
		// may be less than active.size() if
		// a constraint was not detected as inactive
		int count=0;
		int c; // constraint number

		for (int i=0; i<active.size(); i++) {
			c=(i==0? active.min() : active.next(c));

			try {
				if (sys.ops[c]==EQ && c!=goal_ctr)
					// in principle we could deal with linear constraints
					return -1;
				else if (c==goal_ctr || sys.ops[c]==LEQ || sys.ops[c]==LT)
					count += linearize_leq_mid(box,exp_point, J[i],g_mid[i]);
				else
					count += linearize_leq_mid(box, exp_point, -J[i],-g_mid[i]);
			} catch (LPException&) {
				return -1;
			} catch (Unsatisfiability&) {
				return -1;
			}
		}

		//adding constraints: -(xi - xi_mid) <= Ui, (xi - xi_mid) <= Ui
		for(int i=0; i<n; i++){
			Vector a=Vector::zeros(2*n);
			a[i]=-1.0;
			a[n+i]=-1.0;

			Vector a2=Vector::zeros(2*n);
			a2[i]=1.0;
			a2[n+i]=-1.0;


			lp_solver->add_constraint(a, LEQ, -exp_point[i] );
			lp_solver->add_constraint(a2, LEQ, exp_point[i] );
			count +=2;
		}

		return count;
	} catch(NoExpansionPoint&) {
		return -1;
	}

}


int LinearizerAbsTaylor::linearize_leq_mid(const IntervalVector& box, const Vector& point, const IntervalVector& dg_box, const Interval& g_mid) {
	Vector a(2*n); // vector of coefficients

	// ========= compute matrix of coefficients ===========
	// Fix each coefficient to the lower/upper bound of the
	// constraint gradient, depending on the position of the
	// corresponding component of the corner and the
	// linearization mode.
	for (int j=0; j<n; j++){
		a[j]=dg_box[j].mid();
	}

	for (int j=n; j<2*n; j++){
		a[j]=(dg_box[j-n]-a[j-n]).mag();
	}

	// =====================================================
    Vector aa=a;
	aa.resize(box.size());
	Interval rhs = -g_mid + aa*point - lp_solver->tolerance();

	double b = rhs.lb() ;

	// may throw Unsatisfiability and LPException
	return check_and_add_constraint(box,a,b);
}

int LinearizerAbsTaylor::check_and_add_constraint(const IntervalVector& box, const Vector& a, double b) {

	Interval ax=a*box; // for fast (in)feasibility check

	// ======= Quick (in)feasibility checks
	//                 a*[x] <= rhs ?
	if (ax.lb()>b)
		// the constraint is not satisfied
		throw Unsatisfiability();
	else if (ax.ub()<=b) {
		// the (linear) constraint is satisfied for any point in the box
		return 0;
	} else {
		//cout << "add constraint " << a << "*x<=" << b << endl;
		lp_solver->add_constraint(a, LEQ, b);
		return 1;
	}
}

} // end namespace
