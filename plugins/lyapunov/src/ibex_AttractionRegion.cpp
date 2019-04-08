//============================================================================
//                                  I B E X
// File        : ibex_AttrationRegion.h
// Authors     : Gilles Chabert, Alexandre Goldsztejn
// Copyright   : IMT Atlantique / CNRS
// License     : See the LICENSE file
// Created     : December 2016
//============================================================================

#include "ibex_AttractionRegion.h"

#include "ibex.h"

#include <sstream>
#include <utility>
#include <vector>
#include <fstream>
#include <cassert>

#define DEFAULT_TIMEOUT 600
#define RELEASE_2_5

using namespace std;

namespace ibex {

Matrix QuadraticFunction::check_quadratic(const Function& dv) {
	int n=dv.nb_var();
	IntervalMatrix J=dv.jacobian(IntervalVector(n));
	if (J.diam()==Matrix::zeros(n,n)) return 0.5*J.mid();
	else throw NotQuadratic();
}

QuadraticFunction::QuadraticFunction(const Function& dv) : n(dv.nb_var()), P(check_quadratic(dv)), invP(n,n) {
	//cout << "P=" << P << endl;

	neumaier_inverse(P,invP);
	//cout << "P^-1=" << invP << endl;
}

IntervalVector QuadraticFunction::ellipse_hull(double c) const {
	IntervalVector d(n);
	double r=sqrt(Interval(c,c)).ub();
	for (int i=0; i<n; i++) d[i]=sqrt(invP[i][i]);
	return (Interval(-1,1)*r)*d;
}


// ===============================================================================

AttractionRegion::AttractionRegion(Function& f, Function& v, Function& vminor,
		const Vector& xhat, const IntervalVector& theta, const Array<NumConstraint>& theta_ctr) :
		timeout(DEFAULT_TIMEOUT),
		n(f.args()[0].dim.vec_size()), p(f.args().size()>1? f.args()[1].dim.vec_size() : 0), f(f),
		v(v), dv(v,Function::DIFF), vdot_xstar(), vdot_xhat(), vddot(),
		vmin(&v==&vminor? dv : Function(vminor,Function::DIFF) /* only differential vminor if different from v */),
		xstar(n), xhat(xhat),
		is_quadratic(false), theta(theta), varset(f,f.args()[0]),
		theta_ctr(theta_ctr) {

//	cout << f << endl;
//	cout << v << endl;
//	cout << dv << endl;

	init_xstar(xhat);
//	cout << "[x*]=" << xstar << endl;

}

AttractionRegion::~AttractionRegion() {

}

void AttractionRegion::init_xstar(const Vector& xstar_approx) {

	assert(xstar_approx.size()==n);

	if (p>0) {
		IntervalVector xstar_approx_theta=cart_prod(xstar_approx,theta);
		IntervalVector xstar_theta(n+p);
		IntervalVector unicity_box(n+p); // ignored

		if (!inflating_newton(f,varset,xstar_approx_theta,xstar_theta,unicity_box)) {
			ibex_error("[Lyapunov]: cannot build an enclosure of the equilibrium");
		}

		xstar = xstar_theta.subvector(0,n-1);

	} else {

		IntervalVector unicity_box(n); // ignored

		if (!inflating_newton(f,xstar_approx,xstar,unicity_box)) {
			ibex_error("[Lyapunov]: cannot build an enclosure of the equilibrium");
		}
	}

}

void AttractionRegion::init_vdot(Function& vdot, const IntervalVector& center) {

	Array<const ExprSymbol> args(f.args().size());
	varcopy(f.args(), args);
	const ExprSymbol& x=args[0];

	 vdot.init(args,(dv(x-center)*f((const Array<const ExprNode>&) args)).simplify(),"vdot");
}

void AttractionRegion::init_vddot() {
	// we differentiate with respect to x and theta:
	Function dvdot_xtheta(vdot_xstar,Function::DIFF);

	Array<const ExprSymbol> args(f.args().size());
	varcopy(f.args(), args);

	// we retrieve all the derivatives w.r.t to x
	const ExprNode& e=dvdot_xtheta((Array<const ExprNode>&) args);
	Array<const ExprNode> a(n);
	for (int i=0; i<n; i++) {
		a.set_ref(i,e[varset.var(i)]);
	}
	const ExprVector& v=ExprVector::new_col(a);

	vddot.init(args,v.simplify());
}

bool AttractionRegion::is_pos_def(const IntervalMatrix& Q) {
	// try fast PD test: diagonal dominancy.
	if (is_diagonal_dominant(Q)) {
		// check sign of the diagonal
		for (int i=0; i<Q.nb_rows(); i++) {
			if (Q[i][i].lb()<0) return false;
		}
		return true;
	}

	// Then:
	//return is_posdef_rohn(Q);    // warning: exponential complexity
	return is_posdef_sylvester(Q); // polynomial :)
}

bool AttractionRegion::is_lyapunov_deriv_hansen(ellipse_test_type test, const IntervalVector& u) {

	IntervalMatrix J(n,n);
	IntervalVector x=xstar+u;

	if (p>0) {
		IntervalMatrix Jtheta(n,p);
		if (test==DERIVATIVE)
			f.jacobian(cart_prod(x,theta),J,Jtheta,varset);
		else
			f.hansen_matrix(cart_prod(x,theta),xstar,J,Jtheta,varset);
	} else {
		if (test==DERIVATIVE)
			f.jacobian(x,J);
		else {
			f.hansen_matrix(x,xstar,J);
		}
	}

	IntervalMatrix P(n,n);

	if (is_quadratic) {
		P=vmin.matrix();
	} else {
		// The Hansen matrix must be a slope matrix for exactly 0.
		if (u.mid()!=Vector::zeros(n)) {
			cerr << "warning: [u] not centered on zero!" << endl;
			dv.hansen_matrix(u,Vector::zeros(n),P);
		} else
			dv.hansen_matrix(u,P);
	}

	IntervalMatrix Q=-(J.transpose()*P + P*J);

	return is_pos_def(Q);
}

bool AttractionRegion::is_lyapunov_hessian(const IntervalVector& u) {
	IntervalMatrix J(n,n);
	IntervalVector x=xstar+u;

	if (vddot.name==NULL) {
		init_vdot(vdot_xstar, xstar);
		init_vddot();
	//	cout << vdot_xstar << endl;
	//	cout << *vddot << endl;
	}

	if (p>0) {
		IntervalMatrix Jtheta(n,p);
		vddot.jacobian(cart_prod(x,theta),J,Jtheta,varset);
	} else {
		vddot.jacobian(x,J);
	}
	return is_pos_def(-J);

}

Interval AttractionRegion::find_level_set_dichotomy(ellipse_test_type test, double& time, double cmax, double prec) {
	double cl=0;
	double cu=cmax;
	bool res;

	double c;
	IntervalVector u(n);
	Timer timer;
	timer.start();
	time=0;

	do {
		c=0.5*(cl+cu);

		u = vmin.ellipse_hull(c);

		if (test==DERIVATIVE || test==HANSEN)
			res=is_lyapunov_deriv_hansen(test,u);
		else
			res=is_lyapunov_hessian(u);

		if (res) cl=c; else cu=c;

		timer.stop();
		time += timer.get_time();
		timer.start();
		if (time >=timeout) break;

	} while (cu-cl>prec*cu);

	double Delta;
	IntervalVector ustar=xstar-xhat;

//	if (is_quadratic) {
//		Delta=sqrt(ustar*P*ustar).ub();
//	} else {
		IntervalMatrix P(n,n);
		if (is_quadratic)
			P=vmin.matrix();
		else {
			// The Hansen matrix must be a slope matrix for exactly 0.
			if (u.mid()!=Vector::zeros(n)) {
				cerr << "warning: [u] not centered on zero!" << endl;
				dv.hansen_matrix(u,Vector::zeros(n),P);
			} else
				dv.hansen_matrix(u,P);
		}

		IntervalVector Pu=P*u;
		Interval L=sqrt(Pu*Pu);
		Delta=(L*sqrt(ustar*ustar)).ub();
//	}

	timer.stop();
	time += timer.get_time();

	if (cu<Delta) return Interval::ZERO;
	else if (cl<Delta) return Interval(0, cu-Delta);
	else return Interval(cl-Delta, cu-Delta);
}

namespace {


/**
 * Contractor w.r.t an ellipse
 *  (used by the optimizer)
 */
class EllipseHull : public Ctc {
public:

	EllipseHull(int n, int p, const QuadraticFunction& q, const IntervalVector& xstar) :
		Ctc(n+p+2), n(n), p(p), q(q), xstar(xstar) { }

	void contract(IntervalVector& box) {
		IntervalVector x=box.subvector(0,n-1);
		double c=box[n+p+1].ub();
		if (c<POS_INFINITY) {
			x &= (xstar+q.ellipse_hull(c));
			if (x.is_empty()) box.set_empty();
			else box.put(0,x);
		}
	}

	int n,p;
	const QuadraticFunction& q;
	const IntervalVector& xstar;
};

} // end namespace

Interval AttractionRegion::find_level_set_optim(double cmin, double cmax, double prec, Vector& feasible, double& time, int& nb_cells) {
	SystemFactory fac;

	if (vdot_xhat.name==NULL) {
		init_vdot(vdot_xhat, xhat);
	}

	Array<const ExprSymbol> xtheta(f.args().size());
	varcopy(f.args(), xtheta);
	const ExprSymbol& x=xtheta[0];

	// The square of the radius
	const ExprSymbol& c=ExprSymbol::new_("c");

	Array<const ExprNode> _u(n);
	for (int i=0; i<n; i++) {
		_u.set_ref(i,x[i]-xhat[i]);
	}
	const ExprVector& u=ExprVector::new_col(_u);

	fac.add_var(xtheta);

	fac.add_var(c);

	fac.add_goal(c);

	fac.add_ctr(vdot_xhat((Array<const ExprNode>&) xtheta)>=0);

	fac.add_ctr(c=v(u).simplify());
	// warning: previous "simplify" may have destroyed nodes of u!
	// if you have to use u again, build it again...

	for (int i=0; i<theta_ctr.size(); i++)
		fac.add_ctr(ExprCtr(theta_ctr[i].f((Array<const ExprNode>&) xtheta),theta_ctr[i].op));

	//note: c>=vminor(u) will be enforced by the Ellipse contractor

	System sys(fac);

	//cout << "sys=" << sys << endl;

	sys.box=IntervalVector(n+p+1); //Interval(-1e8,1e8));

	// Can also be used in place of the contractor Ellipse:
	//sys.box.put(0,xstar+ellipse_hull(cmax));

	if (p>0)
		sys.box.put(n,theta);

	sys.box[n+p]=Interval(cmin,POS_INFINITY);

	// Extended system (the objective is transformed to a constraint y=f(x))
	
#ifdef RELEASE_2_5
	ExtendedSystem ext_sys(sys, NormalizedSystem::default_eps_h);
#else
	ExtendedSystem ext_sys(sys);  // --> is actually wrong! we have to use eps_h
#endif

	// ===========================================================================
	// ============================ building contractors ========================= 
	CtcHC4 hc4(ext_sys,0.01);

	CtcHC4 hc4_2(ext_sys,0.1,true);

	CtcAcid acid(ext_sys, hc4_2, true);

#ifdef RELEASE_2_5
	LinearizerCombo linear_relax(ext_sys,LinearizerCombo::XNEWTON);
	CtcPolytopeHull polytope(linear_relax);
#else
	LinearRelaxCombo linear_relax(ext_sys,LinearRelaxCombo::XNEWTON);
	CtcPolytopeHull polytope(linear_relax,CtcPolytopeHull::ALL_BOX);
#endif
	
	CtcCompo polytope_hc4(polytope, hc4);

	CtcFixPoint fixpoint(polytope_hc4, 0.2);

	EllipseHull hull(n,p,vmin,xstar);

	//CtcCompo compo(hc4,acid,fixpoint);
	CtcCompo compo(hull,hc4,acid,fixpoint);
	// ===========================================================================

	// Create a bisection heuristic.
	//SmearSumRelative bisector(ext_sys, prec);
	//RoundRobin bisector(prec);
	//LargestFirst bisector(0); //prec
	LargestFirst bisector(prec);

	// Create a solver with the previous objects

#ifdef RELEASE_2_5
	NormalizedSystem norm_sys(sys, NormalizedSystem::default_eps_h);
	LoupFinderDefault loup_finder(norm_sys, false); // false=no inHC4
	CellDoubleHeap buffer(ext_sys); 
	Optimizer o(n+p+1, compo, bisector, loup_finder, buffer, ext_sys.goal_var(), 0, prec, 0);
#else
	Optimizer o(sys, compo, bisector, 0, prec, 0, Optimizer::default_sample_size, Optimizer::default_equ_eps, false);
	o.in_HC4_flag=false;
#endif

	o.timeout=timeout;
	o.trace=0;
	
	o.optimize(sys.box,cmax);
	
	//o.report();

#ifdef RELEASE_2_5
	feasible = o.get_loup_point().mid();
	time = o.get_time();
	nb_cells = o.get_nb_cells();
	return Interval(o.get_uplo(),o.get_loup());
#else
	feasible = o.loup_point;
	time = o.time;
	nb_cells = o.nb_cells;
	return Interval(o.uplo,o.loup);
#endif

	//cout << " v(x*)=" << v.eval(o.loup_point) << endl;
}

} // end namespace ibex

