//============================================================================
//                                  I B E X
// File        : Mohc Contractor
// Author      : Ignacio Araya, Gilles Trombettoni, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Ene 8, 2013
// Last Update : Ene 8, 2013
//============================================================================

#include "ibex_CtcMohc.h"
#include "ibex_Eval.h"
#include "ibex_CtcFwdBwd.h"

using namespace std;

namespace ibex {

namespace {
class MohcEmptyBoxException { };
}

const double CtcMohc::ADAPTIVE = -1.0;

const double CtcMohc::default_tau_mohc=0.9;
const double CtcMohc::default_epsilon=0.1;
const double CtcMohc::default_univ_newton_min_width=1e-8;


/*********** amohc ******/
//parameters
const double CtcMohc::RHO_INTERESTING=0.65;
const double CtcMohc::TAU_FREQ=0.10;

//auxiliary variables
int CtcMohc::nb_calls=0;
int CtcMohc::nb_interesting=0;
/*************************/


/**for analysis purposes only**/
bool CtcMohcRevise::_minmax=true;
bool CtcMohcRevise::_monobox=true;
bool CtcMohcRevise::_opt=true;
bool CtcMohcRevise::_og=true;
bool CtcMohcRevise::_mohc2=true;
/*****************************/

CtcMohcRevise::CtcMohcRevise(const NumConstraint& c, double epsilon, double univ_newton_min_width,
		double tau_mohc, bool amohc) :
			   Ctc(c.f.nb_var()), ctr(c.f,c.op), fog(c.f), active_mono_proc(1),
			   LB(c.f.nb_var()), RB(c.f.nb_var()), box(c.f.nb_var()),
			   tau_mohc(tau_mohc), epsilon(epsilon),
			   univ_newton_min_width(univ_newton_min_width), amohc(amohc)	{

	input = new BitSet(0,nb_var-1,BitSet::empt);
	output = new BitSet(0,nb_var-1,BitSet::empt);

	for (int v=0; v<ctr.f.nb_var(); v++) {
		if (ctr.f.used(v)) {
			output->add(v);
			input->add(v);
		}
	}

	ApplyFmin=new _3vl[nb_var];
	ApplyFmax=new _3vl[nb_var];

}


bool CtcMohcRevise::hasMultOcc(Function &f) {
	for(int i=0; i<nb_var; i++)
		if (fog.occ[i].size()>1) return true;
	return false;
}

void CtcMohcRevise::update_active_mono_proc(Interval& z) {
	Interval ev_mono(0,0);
	if (ctr.op==EQ || ctr.op==LEQ || ctr.op==LT)
		ev_mono|=zmin;
	else
		ev_mono|=fog.eval(box, false);
	if (ctr.op==EQ || ctr.op==GEQ || ctr.op==GT)
		ev_mono|=zmax;
	else
		ev_mono|=fog.eval(box, true);

	double diam_mono=ev_mono.diam();
	double diam=z.diam();


	//  diam = std::min(diam,fog.taylor_error); //si se usa un contractor de relajacion lineal?

	double rho;
	if (diam>0)
		rho=diam_mono/diam;
	else
		rho=1.01;

	if (amohc) {
		CtcMohc::nb_calls++;
		if (rho < CtcMohc::RHO_INTERESTING)
			CtcMohc::nb_interesting++;
		if (CtcMohc::nb_calls > 50 && (double)CtcMohc::nb_interesting/(double)CtcMohc::nb_calls < CtcMohc::TAU_FREQ)
			tau_mohc=0.5;
		else
			tau_mohc=0.9999;
	}


	if (rho<tau_mohc) {active_mono_proc=1;}
	else active_mono_proc=0;
}

void CtcMohcRevise::contract(IntervalVector& b) {

	if (b.is_empty()) { //no deberia ocurrir
		set_flag(INACTIVE);
		set_flag(FIXPOINT);
		return;
	}
	const Dim& d=ctr.f.expr().dim;
	Domain root_label(d);
	Interval right_cst;


	switch (ctr.op) {
	case LT :
	case LEQ : right_cst=Interval(NEG_INFINITY, 0); break;
	case EQ  : right_cst=Interval(0,0); break;
	case GEQ :
	case GT : right_cst=Interval(0,POS_INFINITY); break;
	}

	assert(d.type()==Dim::SCALAR);
	root_label.i()=right_cst;
	Interval z;

	ctr.f.backward(root_label,b);

	if (b.is_empty()) {
		set_flag(FIXPOINT);
		active_mono_proc=0;
		return;
	}

	//box is feasible
	z=ctr.f.expr().deco.d->i();

	if (b.is_empty()) return;

	//if (ctr.op!=EQ) z&=right_cst;


	if (!hasMultOcc(ctr.f))
		active_mono_proc=0;


	box=b;

	bool flag=false;
	IntervalVector initbox=box;
	try{

		if (active_mono_proc != 0) { //monotonic procedures

			bool y_set=_minmax;//the Y set is created only if minmax is used

			if (!fog.occurrence_grouping(box, y_set, _og)) {
				if (active_mono_proc==-1) active_mono_proc=0;
				b=box;
				return;
			}

			initialize_apply(); //initialize applyfmin and applyfmax arrays

			zmin.set_empty ();
			zmax.set_empty ();

			if (ctr.op==EQ || ctr.op==LEQ || ctr.op==LT) {
				if (_minmax) {

					zmin=fog.revise(box,true);

				} else {
					zmin=fog.eval(box, true);

					zmin &= Interval(NEG_INFINITY,0);
					if (zmin.is_empty ()) { b.set_empty(); throw MohcEmptyBoxException();}
				}
			} else
				apply_fmin_to_false_except(-1);


			if (ctr.op==EQ || ctr.op==GEQ || ctr.op==GT) {
				if (_minmax) {

					zmax=fog.revise(box,false); // contract Y, W

				} else {
					// only the existence test
					zmax=fog.eval(box, false); //og

					zmax &= Interval(0, POS_INFINITY);
					if (zmax.is_empty ()) { b.set_empty(); throw MohcEmptyBoxException();}
				}
			} else
				apply_fmax_to_false_except(-1);

			if (active_mono_proc==-1) { //compute_rho_mohc

				update_active_mono_proc(z);
			}

			if (epsilon>0 && _monobox)
				MonoBoxNarrow();

		}
	} catch (MohcEmptyBoxException& e) {
		b.set_empty();
		set_flag(FIXPOINT);
		return;
	}

	b=box;
	//if (flag) cout << "box____:" << b << endl;
}

void CtcMohcRevise::MonoBoxNarrow() {
	for(int i=0; i<nb_var; i++) {
		// if (fog.nb_occ[i]==0 || (fog.nb_occ[i]==1 && _minmax) || box[i].diam() < 1e-8) continue;
		LB[i]=box[i];
		RB[i]=box[i];
	}
	if (_opt)
		LazyNarrow();

	if (_opt) {
		if (zmin.contains(0)) apply_fmax_to_false_except(-1);
		if (zmax.contains(0)) apply_fmin_to_false_except(-1);
	}

	for(int i=0; i<nb_var; i++) {
		if (fog.occ[i].size()==0 || (fog.occ[i].size()==1 && _minmax) || box[i].diam() < 1e-8) continue;
		MonoBoxNarrow(i);
	}

	for(int i=0; i<nb_var; i++) {
		//  if (fog.nb_occ[i]==0 || (fog.nb_occ[i]==1 && _minmax) || box[i].diam() < 1e-8) continue;
		box[i]=Interval(LB[i].lb(), RB[i].ub());
	}

}

void CtcMohcRevise::MonoBoxNarrow(int i) {
	//Si la variable no ha sido tratado completamente por MinMaxRevise
	//Proyeccion usando MinMaxRevise.

	if (_mohc2 && fog.g[i].lb()<0 && fog.g[i].ub()>0) {
		if (ApplyFmax[i]!=NO || ApplyFmin[i]!=NO) {
			bool og_treated=false;

			for(unsigned int j=0; j<fog.occ[i].size(); j++) {
				if (fog.r_c[fog.occ[i][j]].ub() < 1.0) {og_treated=true; break;}
			}
			//~ for(int occ=fog.first_occ[i];occ<fog.first_occ[i+1];occ++)
			//~ if (fog.r_c[occ].ub() < 1.0) {og_treated=true; break;}

			if (og_treated) {
				LeftNarrow(i);
				RightNarrow(i);
				if (LB[i].lb()>RB[i].ub()) throw MohcEmptyBoxException();
			}
		}
		return;
	}

	if (fog.g[i].lb()>=0) { //x is increasing
		if (ApplyFmax[i]!=NO)
			LeftNarrowFmax(i);

		if (ApplyFmin[i]!=NO)
			RightNarrowFmin(i);
		return;
	}

	if (fog.g[i].ub()<=0) { //x is decreasing
		if (ApplyFmin[i]!=NO)
			LeftNarrowFmin(i);

		if (ApplyFmax[i]!=NO)
			RightNarrowFmax(i);
		return;
	}
}

bool CtcMohcRevise::_existence_test(int i) {

	if (ApplyFmax[i]!=NO) {
		zmax=fog.eval(box,false); //max eval

		if (_opt && zmax.lb()<=0)  //the lower bound of x_i is solution or will be.
			apply_fmin_to_false_except(i);
	}

	if (ApplyFmin[i]!=NO) {
		zmin=fog.eval(box,true); //min eval

		if (_opt && zmin.ub()>=0)  //the lower bound of x_i is solution or will be.
			apply_fmax_to_false_except(i);
	}

	if ((zmax.is_empty() || zmax.ub()>=0) && (zmin.is_empty() || zmin.lb()<=0))
		return false;

	return true;
}

void CtcMohcRevise::RightNarrow(int i) {

	Interval ini(box[i]);
	box[i]=RB[i].ub();

	/**********existence test*************/
	if (!_existence_test(i)) {box[i]=ini; return; }
	/************************************/

	Interval right_cst;

	switch (ctr.op) {
	case LT :
	case LEQ : right_cst=Interval(NEG_INFINITY, 0); break;
	case EQ  : right_cst=Interval(0,0); break;
	case GEQ :
	case GT : right_cst=Interval(0,POS_INFINITY); break;
	}

	double w=epsilon*ini.diam();
	RB[i]=Interval(RB[i].mid(),RB[i].ub());
	while(2*RB[i].diam()>w && 2*RB[i].diam()>univ_newton_min_width) {
		box[i]=RB[i];
		Interval ev=fog.eval(box); //se puede ahorrar una llamada en caso de inecuacion
		ev&=right_cst;

		if (!ev.is_empty())
			RB[i]=Interval(RB[i].mid(),RB[i].ub());
		else
			RB[i]=Interval(RB[i].lb()-RB[i].diam()/2,RB[i].lb());
	}

	RB[i]&=ini;
	if (RB[i].is_empty()) throw MohcEmptyBoxException();

	box[i]=ini;
}

//LeftNarrow of a variable not completely monotone!
void CtcMohcRevise::LeftNarrow(int i) {
	Interval ini(box[i]);
	box[i]=LB[i].ub();

	/**********existence test*************/

	if (!_existence_test(i)) {box[i]=ini; return; }

	/************************************/

	Interval right_cst;

	switch (ctr.op) {
	case LT :
	case LEQ : right_cst=Interval(NEG_INFINITY, 0); break;
	case EQ  : right_cst=Interval(0,0); break;
	case GEQ :
	case GT : right_cst=Interval(0,POS_INFINITY); break;
	}

	double w=epsilon*ini.diam();
	LB[i]=Interval(LB[i].lb(),LB[i].mid());
	while(2*LB[i].diam()>w && 2*LB[i].diam()>univ_newton_min_width) {
		box[i]=LB[i];
		Interval ev=fog.eval(box);
		ev&=right_cst;
		if (!ev.is_empty())
			LB[i]=Interval(LB[i].lb(), LB[i].mid());
		else
			LB[i]=Interval(LB[i].ub(), LB[i].ub()+LB[i].diam()/2);
	}

	LB[i]&=ini;
	if (LB[i].is_empty()) throw MohcEmptyBoxException();

	box[i]=ini;

}

void CtcMohcRevise::LeftNarrowFmax(int i) {
	Interval ini(box[i]);
	if (ApplyFmax[i]==MAYBE) { //the first free Newton didn't contract the variable i.
		//existence test
		box[i]=LB[i].lb(); //point

		zmax=fog.eval(box,false); //max eval
		if (_opt && zmax.lb()<=0)  //the lower bound of x_i is solution or will be.
			apply_fmin_to_false_except(i);

		if (zmax.ub()<0) { //there are a solution for L in the Interval
			//second free Newton call
			if (_opt) LB[i]=fog.Newton_it_cert(LB[i],LB[i].lb(),zmax.ub(),i);
		} else {
			//ApplyFmax[i]=NO;
			box[i]=ini;
			return;
		}
	}

	double w=epsilon*ini.diam(); //should be here or before LazyNarrow? or before Hc4Revise?

	fog.MonoUnivNewton(box, i, LB[i], false, w, univ_newton_min_width);
	box[i]=ini;
}

void CtcMohcRevise::LeftNarrowFmin(int i) {
	Interval ini(box[i]);
	if (ApplyFmin[i]==MAYBE) { //the first free Newton didn't contract the variable i.
		//existence test
		box[i]=LB[i].lb(); //point A

		zmin=fog.eval(box,true);  //min eval
		if (_opt && zmin.ub()>=0)  //the lower bound of x_i is solution or will be.
			apply_fmax_to_false_except(i);

		if (zmin.lb()>0) { //there are a solution for L in the Interval
			//second free Newton call
			if (_opt) LB[i]=fog.Newton_it_cert(LB[i],LB[i].lb(),zmin.lb(),i);

		} else {
			//ApplyFmax[i]=NO;
			box[i]=ini;
			return;
		}
	}

	double w=epsilon*ini.diam(); //should be here or before LazyNarrow? or before Hc4Revise?

	fog.MonoUnivNewton(box, i, LB[i], true, w, univ_newton_min_width);
	box[i]=ini;
}

void CtcMohcRevise::RightNarrowFmin(int i) {
	Interval ini(box[i]);
	if (ApplyFmin[i]==MAYBE) { //the first free Newton didn't contract the variable i.
		//existence test
		box[i]=RB[i].ub(); //point A
		zmin=fog.eval(box,true);  //min eval
		if (_opt && zmin.ub()>=0)  //the lower bound of x_i is solution or will be.
			apply_fmax_to_false_except(i);

		if (zmin.lb()>0) { //there are a solution for L in the Interval
			//second free Newton call
			if (_opt)  RB[i]=fog.Newton_it_cert(RB[i],RB[i].ub(),zmin.lb(),i);
		} else {
			//ApplyFmax[var]=NO;
			box[i]=ini;
			return;
		}
	}

	double w=epsilon*ini.diam(); //should be here or before LazyNarrow? or before Hc4Revise?
	fog.MonoUnivNewton(box, i, RB[i], true, w, univ_newton_min_width);
	box[i]=ini;
	//      x=Interval(ini.lb(), RB[var].ub());
}

void CtcMohcRevise::RightNarrowFmax(int i) {
	Interval ini(box[i]);
	if (ApplyFmax[i]==MAYBE) { //the first free Newton didn't contract the variable i.
		//existence test
		box[i]=RB[i].ub(); //point A
		zmax=fog.eval(box,false);  //max eval
		if (_opt && zmax.lb()<=0)  //the lower bound of x_i is solution or will be.
			apply_fmin_to_false_except(i);

		if (zmax.ub()<0) { //there are a solution for L in the Interval
			//second free Newton call
			if (_opt)  RB[i]=fog.Newton_it_cert(RB[i],RB[i].ub(),zmax.ub(),i);
		} else {
			//ApplyFmax[i]=NO;
			box[i]=ini;
			return;
		}
	}

	double w=epsilon*ini.diam(); //should be here or before LazyNarrow? or before Hc4Revise?

	fog.MonoUnivNewton(box, i, RB[i], false, w, univ_newton_min_width);
	box[i]=ini;
	//      x=Interval(ini.lb(), RB[var].ub());
}

void CtcMohcRevise::LazyNarrow() {
	for(int i=0; i<nb_var; i++) {
		if (fog.occ[i].size()==0 || (fog.occ[i].size()==1 && _minmax) ) continue;


		Interval ini(box[i]);
		if (fog.g[i].lb()>=0.0) {
			if (ApplyFmax[i]!=NO)
				initLeftNarrowFmax(i);
			if (ApplyFmin[i]!=NO)
				initRightNarrowFmin(i);
		} else  if (fog.g[i].ub()<=0.0) {
			if (ApplyFmin[i]!=NO)
				initLeftNarrowFmin(i);
			if (ApplyFmax[i]!=NO)
				initRightNarrowFmax(i);
		} else if (_og) {
			initLeftNarrow(i);
			initRightNarrow(i);
		}

		box[i]=ini;
	}

}


void CtcMohcRevise::initLeftNarrow(int i) {
	Interval t_a=LB[i];
	Interval t_b=LB[i];
	if (ApplyFmax[i]!=NO)
		t_a=fog.Newton_it(LB[i],LB[i].ub(),zmax.ub(),i,true); //x_a (increasing=true)
	if (ApplyFmin[i]!=NO)
		t_b=fog.Newton_it(LB[i],LB[i].ub(),zmin.lb(),i,false); //x_b


	if (!t_a.is_empty() && (t_a.lb() > LB[i].lb())) {
		apply_fmin_to_false_except(i);
		ApplyFmax[i]=YES;
	}

	if (!t_b.is_empty() && (t_b.lb() > LB[i].lb())) {
		apply_fmax_to_false_except(i);
		ApplyFmin[i]=YES;
	}
	if (t_a.is_empty()) ApplyFmax[i]=NO;
	if (t_b.is_empty()) ApplyFmin[i]=NO;

	if (!t_a.is_empty() || !t_b.is_empty())
	{
		if (!t_a.is_empty()) LB[i]&=t_a;
		if (!t_b.is_empty()) LB[i]=Interval(std::max(LB[i].lb(),t_b.lb()) , std::min(t_b.ub(),LB[i].ub()) ) ;

		//        if (Diam(LB[var])>0) {
			//          double deriv_a=ctr_mohc.zmax.ub()/Diam(LB[var]);
		//          double deriv_b=ctr_mohc.zmin.lb()/Diam(LB[var]);
		//          if (ctr_mohc.deriv_a[var].ub()>0 && deriv_a/ctr_mohc.deriv_a[var].ub() < tau_mono) ApplyFmax[var]=NO;
		//          if (ctr_mohc.deriv_b[var].lb()<0 && deriv_b/ctr_mohc.deriv_b[var].lb() < tau_mono) ApplyFmin[var]=NO;
		//        }
	}
}

void CtcMohcRevise::initRightNarrow(int i) {
	Interval t_a=RB[i];
	Interval t_b=RB[i];
	if (ApplyFmin[i]!=NO)
		t_a=fog.Newton_it(RB[i],RB[i].lb(),zmin.lb(),i,true);  //x_a
		if (ApplyFmax[i]!=NO)
			t_b=fog.Newton_it(RB[i],RB[i].lb(),zmax.ub(),i,false); //x_b

		if (t_a==RB[i] && t_b==RB[i]) return;

		if (!t_a.is_empty() && (t_a.ub() < RB[i].ub())) {
			//apply_fmax_to_false_except(i);
			ApplyFmin[i]=YES;
		}

		if (!t_b.is_empty() && (t_b.ub() < RB[i].ub())) {
			//apply_fmin_to_false_except(i);
			ApplyFmax[i]=YES;
		}


		if (t_a.is_empty()) ApplyFmin[i]=NO;
		if (t_b.is_empty()) ApplyFmax[i]=NO;
		if (!t_a.is_empty() || !t_b.is_empty())
		{
			if (!t_a.is_empty()) RB[i]&=t_a;
			if (!t_b.is_empty()) RB[i]=Interval(std::max(RB[i].lb(),t_b.lb()) , std::min(t_b.ub(),RB[i].ub()) ) ;

			//        if (Diam(RB[i])>0) {
			//           double deriv_a=-ctr_mohc.zmin.lb()/Diam(RB[i]);
			//           double deriv_b=-ctr_mohc.zmax.ub()/Diam(RB[i]);
			//           if (ctr_mohc.deriv_a[i].ub()>0 && deriv_a/ctr_mohc.deriv_a[i].ub() < tau_mono) ApplyFmin[i]=NO;
			//           if (ctr_mohc.deriv_b[i].lb()<0 && deriv_b/ctr_mohc.deriv_b[i].lb() < tau_mono) ApplyFmax[i]=NO;
			//        }

		}
}


//initialize LB projecting fmax (or fmin) in X[i] using deriv[i].
//contract the left bound of X[i] using the new value LB.lb().
void CtcMohcRevise::initLeftNarrowFmin(int i) {
	Interval t=fog.Newton_it(LB[i],LB[i].ub(),zmin.lb(),i);
	if (!t.is_empty() && (t.lb() > LB[i].lb())) {
		apply_fmax_to_false_except(i);
		ApplyFmin[i]=YES;
		LB[i]&=t;
	} else if (t.is_empty()) { //no contraction is possible
		ApplyFmin[i]=NO;
	} else {
		LB[i]&=t;
		//         if (Diam(LB[i])>0 && ctr_mohc.deriv_b[i].lb()<0) {
		//            double deriv_b=ctr_mohc.zmin.lb()/Diam(LB[i]);
		//            if (deriv_b/ctr_mohc.deriv_b[i].lb() < tau_mono) ApplyFmin[i]=NO;
		//        }
	}
}

void CtcMohcRevise::initLeftNarrowFmax(int i) {
	Interval t=fog.Newton_it(LB[i],LB[i].ub(),zmax.ub(),i);
	if (!t.is_empty() && (t.lb() > LB[i].lb())) {
		apply_fmin_to_false_except(i);
		ApplyFmax[i]=YES;
		LB[i]&=t;
	} else if (t.is_empty()) { //no contraction is possible
		ApplyFmax[i]=NO;
	} else {
		LB[i]&=t;
		//        if (Diam(LB[i])>0 && ctr_mohc.deriv_a[i].ub()>0) {
		//          double deriv_a=ctr_mohc.zmax.ub()/Diam(LB[i]);
		//          if (deriv_a/ctr_mohc.deriv_a[i].ub() < tau_mono) ApplyFmax[i]=NO;
		//        }
	}
}

void CtcMohcRevise::initRightNarrowFmax(int i) {
	Interval t=fog.Newton_it(RB[i],RB[i].lb(),zmax.ub(),i);
	if (!t.is_empty() && (t.ub() < RB[i].ub())) {
		apply_fmin_to_false_except(i);
		ApplyFmax[i]=YES;
		RB[i]&=t;
	} else if (t.is_empty()) { //no contraction is possible
		ApplyFmax[i]=NO;
	} else {
		RB[i]&=t;
		//        if (Diam(RB[i])>0 && ctr_mohc.deriv_b[i].lb()<0) {
		//           double deriv_b=-ctr_mohc.zmax.ub()/Diam(RB[i]);
		//           if (deriv_b/ctr_mohc.deriv_b[i].lb() < tau_mono) ApplyFmax[i]=NO;
		//        }
	}


}

void CtcMohcRevise::initRightNarrowFmin(int i) {
	Interval t=fog.Newton_it(RB[i],RB[i].lb(),zmin.lb(),i);
	if (!t.is_empty() && (t.ub() < RB[i].ub())) {
		apply_fmax_to_false_except(i);
		ApplyFmin[i]=YES;
		RB[i]&=t;
	} else if (t.is_empty()) {
		ApplyFmin[i]=NO;
	} else {
		RB[i]&=t;
		//        if (Diam(RB[i])>0 && ctr_mohc.deriv_a[i].ub()>0) {
		//           double deriv_a=-ctr_mohc.zmin.lb()/Diam(RB[i]);
		//           if (deriv_a/ctr_mohc.deriv_a[i].ub() < tau_mono) ApplyFmin[i]=NO;
		//        }
	}
}

void CtcMohcRevise::initialize_apply() {
	for (int i=0; i<nb_var; i++) {
		ApplyFmin[i]=MAYBE;
		ApplyFmax[i]=MAYBE;
	}
}

void CtcMohcRevise::apply_fmax_to_false_except(int i) {
	for (int ii=0; ii<nb_var; ii++)
		if (ii!=i)
			ApplyFmax[ii]=NO;
}

void CtcMohcRevise::apply_fmin_to_false_except(int i) {
	for (int ii=0; ii<nb_var; ii++)
		if (ii!=i)
			ApplyFmin[ii]=NO;
}



/* Perform a Newton iteration */
Interval Function_OG::Newton_it(Interval b, double x_m, double f_m, int i) {
	Interval nwt_proj=Interval(x_m)-Interval(f_m)/g[i];
	b &= nwt_proj;
	return b;
}

/* Perform a Newton iteration for iiables x_a(increasing=true) and x_b(increasing=false))*/
Interval Function_OG::Newton_it(Interval b, double x_m, double f_m, int i, bool increasing) {
	if (ga[i]==0.0&& gb[i]==0.0) return b;
	Interval deriv=(increasing)? ga[i]:gb[i];
	Interval nwt_proj=Interval(x_m)-Interval(f_m)/deriv ;
	b &= nwt_proj;
	return b;
}

/* Perform a Newton iteration */
Interval Function_OG::Newton_it_cert(Interval b, double x_m, double f_m, int i) {
	Interval b_t=b;
	Interval nwt_proj=Interval(x_m)-Interval(f_m)/g[i];
	b &= nwt_proj;
	if (b.is_empty()) {
		if (b_t.ub()>nwt_proj.ub())
			return b_t.lb();
		else
			return b_t.ub();
	}
	return b;
}

void Function_OG::MonoUnivNewton(IntervalVector& box, int i, Interval& b, bool minEval, double w, double min_width) {
	while(b.diam()>w && b.diam()>min_width) {
		box[i]=b.mid();
		Interval z=eval(box,minEval);
		b=Newton_it_cert(b,b.mid(),(minEval)? z.lb():z.ub(),i);
	}
}


void Function_OG::OG_case1(int i) {
	if (g[i].lb()>=0) {
		set_ra(i,1);
		ga[i]=g[i];
	} else if (g[i].ub()<=0) {
		set_rb(i,1);
		gb[i]=g[i];
	}
	set_rc(i,0);
}

void Function_OG::OG_case2(int i, Interval inf_G_Xa, Interval inf_G_Xb, Interval sup_G_Xa, Interval sup_G_Xb) {
	Interval m1=( inf_G_Xa*sup_G_Xb + sup_G_Xb*inf_G_Xb )/
			( inf_G_Xa*sup_G_Xb - inf_G_Xb*sup_G_Xa );

	Interval m2=( inf_G_Xa*sup_G_Xa + sup_G_Xb*inf_G_Xa )/
			( inf_G_Xa*sup_G_Xb - inf_G_Xb*sup_G_Xa );

	for(unsigned int j=0; j<occ[i].size(); j++) {
		//for(int occ=first_occ[i];occ<first_occ[i+1];occ++) {
			if (_g[occ[i][j]].lb()>=0) {
				r_a[occ[i][j]]=1-m1;
				r_b[occ[i][j]]=m1;
				r_c[occ[i][j]]=0;
			} else if (_g[occ[i][j]].ub()<=0) {
				r_a[occ[i][j]]=m2;
				r_b[occ[i][j]]=1-m2;
				r_c[occ[i][j]]=0;
			}
	}
}

//hash_map<int, double> ConstraintOG::h;

/******* For performing the sorting in OG case 3 ********/
struct Isort {
	bool operator() (const pair<int, double> i1, const pair<int, double> i2)
	{
		return (i1.second<=i2.second);
	};
};
/********************************************************/

void Function_OG::OG_case3(list<int>& X_m, list<int>& X_nm, Interval& G_m) {

	list< pair<int, double> > X_nm_sort;
	if (G_m.ub()>=0) {
		for (list<int>::iterator occ = X_nm.begin(); occ != X_nm.end(); occ++)
			X_nm_sort.push_back(make_pair(*occ, -_g[*occ].ub()/_g[*occ].lb()));
		//h[*occ]=-g[*occ].ub()/g[*occ].lb();
	} else if (G_m.ub()<=0) {
		for (list<int>::iterator occ = X_nm.begin(); occ != X_nm.end(); occ++)
			X_nm_sort.push_back(make_pair(*occ, -_g[*occ].lb()/_g[*occ].ub()));
		//h[*occ]=-g[*occ].lb()/g[*occ].ub();
	}
	X_nm_sort.sort(Isort());

	//the monotone occurrences are grouped in xa or xb
	if (G_m.lb()>=0) {
		for (list<int>::iterator occ = X_m.begin(); occ != X_m.end(); occ++) {
			r_a[*occ]=1;
			r_c[*occ]=0;
		}
	} else {
		for (list<int>::iterator occ = X_m.begin(); occ != X_m.end(); occ++) {
			r_b[*occ]=1;
			r_c[*occ]=0;
		}
	}

	bool sat=true;

	//the non monotone occurrences are put in xa or xb, until the constraint is unsatisfied
	if (G_m.lb()>=0) {
		for (list< pair<int, double> >::iterator occ = X_nm_sort.begin(); occ != X_nm_sort.end(); occ++) {
			Interval m=(sat)? 1:0;
			if (sat && G_m.lb()+_g[occ->first].lb()<0) {
				m=-Interval(G_m.lb())/Interval(_g[occ->first].lb());
				sat=false;
			}

			r_a[occ->first]=m;
			r_c[occ->first]=1-m;
			G_m+=m*_g[occ->first];
		}
	} else {
		for (list< pair<int, double> >::iterator occ = X_nm_sort.begin(); occ != X_nm_sort.end(); occ++) {
			Interval m=(sat)? 1:0;
			if (sat && G_m.ub()+_g[occ->first].ub()>0) {
				m=-Interval(G_m.ub())/Interval(_g[occ->first].ub());
				sat=false;
			}


			r_b[occ->first]=m;
			r_c[occ->first]=1-m;
			G_m+=m*_g[occ->first];
		}
	}
}


bool Function_OG::occurrence_grouping(IntervalVector& box, bool y_set, bool _og) {
	if (!gradient(box)) return false;

	bool worked=false;
	for(int i=0; i<box.size();i++) {
		set_ra(i,0.0);
		set_rb(i,0.0);
		set_rc(i,1.0);
		ga[i]=0.0;  gb[i]=0.0;

		if (occ[i].size()>0 && (!y_set || occ[i].size()>1)) {
			worked |= occurrence_grouping(i,_og);
		}
	}
	return worked;
}

bool Function_OG::occurrence_grouping(int i, bool _og) {
	//only variables with multiple occurrences are treated


	if (g[i].lb() >= 0 || g[i].ub() <= 0) { //G_0 does not contain 0
		OG_case1(i);
		return true;
	}

	if (!_og)
		return false;


	Interval G_plus=0, G_minus=0;

	list<int> X_m, X_nm;

	for(unsigned int j=0; j<occ[i].size(); j++) {
		//for(int occ=first_occ[i];occ<first_occ[i+1];occ++) { //FOR EACH occurrence j

		if (_g[occ[i][j]].lb()>=0) {
			X_m.push_back(occ[i][j]);
			G_plus += _g[occ[i][j]];
		} else if (_g[occ[i][j]].ub()<=0) {
			X_m.push_back(occ[i][j]);
			G_minus += _g[occ[i][j]];
		} else {
			X_nm.push_back(occ[i][j]);
		}
	}

	Interval G_m=G_plus + G_minus;

	if (G_m==0.0) return false;

	if (G_m.lb()<0 && G_m.ub()>0) { //G_m contains 0
		OG_case2(i, G_plus.lb(), G_minus.lb(), G_plus.ub(), G_minus.ub());
		ga[i]=Interval(0,G_m.ub());
		gb[i]=Interval(G_m.lb(),0);
	} else {
		OG_case3(X_m, X_nm, G_m);
		if (G_m.ub()>0) ga[i]=G_m;
		else gb[i]=G_m;
	}

	return true;

}

bool Function_OG::gradient(IntervalVector& box) {
	_setbox(box);
	_f.gradient(_box,_g);


	for (int i=0;i < _g.size();i++) {
		if (_g[i].mag() == POS_INFINITY) {
			return false;
		}
	}


	for(int i=0; i<g.size(); i++) {
		g[i]=0.0;
		for(unsigned int j=0; j<occ[i].size(); j++) {
			//~ for(int occ=first_occ[i];occ<first_occ[i+1];occ++)
			g[i] += _g[occ[i][j]];
		}
	}

	//taylor_error=0.0;
	//for(int i=0; i<g.size(); i++)
	//    taylor_error+=box[i].diam()*g[i].diam();
	return true;
}

void Function_OG::_eval_leaves(IntervalVector& box, bool minrevise) {

	for(int i=0; i<g.size();i++) {
		if (occ[i].size()==0) {/*_box[i] = box[i];*/ continue; }
		for(unsigned int j=0; j<occ[i].size(); j++) {

			if (minrevise) {
				aux[occ[i][j]] = r_a[occ[i][j]]*box[i].lb() + r_b[occ[i][j]]*box[i].ub();
			} else {
				aux[occ[i][j]] = r_a[occ[i][j]]*box[i].ub() + r_b[occ[i][j]]*box[i].lb();
			}

			_box[occ[i][j]] = aux[occ[i][j]] + r_c[occ[i][j]]*box[i];

			r_a[occ[i][j]].mid();
			// ia: this line fix the problem with dipigri. I don't know why
		}
	}
	//cout << "b1:" << box << endl;
	//cout << "b2:" << _box << endl;
}

void Function_OG::_proj_leaves(IntervalVector& box) {
	//projection over the variable intervals

	for(int i=0; i<g.size();i++) {
		if (occ[i].size()==0) { continue; }

		for(unsigned int j=0; j<occ[i].size(); j++) {
			if (r_c[occ[i][j]]==0.0) continue; //occurrence without projection

			box[i] &= (_box[occ[i][j]] - aux[occ[i][j]]) / r_c[occ[i][j]] ;

			if (box[i].is_empty()) { throw MohcEmptyBoxException(); }
		}
	}
}



Interval Function_OG::eval(IntervalVector& box) {
	Interval a=eval(box, true);
	return a|=eval(box, false);
}

Interval Function_OG::eval(IntervalVector& box, bool minrevise) {
	_eval_leaves(box, minrevise);
	Eval().eval(_f,_box);
	return _f.expr().deco.d->i();
}

Interval Function_OG::revise(IntervalVector& box, bool minrevise) {
	const Dim& d=_f.expr().dim;
	Domain root_label(d);
	root_label.i()=(minrevise)? Interval(NEG_INFINITY, 0) : Interval(0, POS_INFINITY);
	_eval_leaves(box, minrevise);

	Interval ev(0.0);

	_f.backward(root_label,_box); //evaluacion se puede obtener a traves de f?

	_proj_leaves(box);

	return _f.expr().deco.d->i();

}

Function_OG::Function_OG(const Function& ff) :
						 eso(ff.args(),ff.expr()), _f(eso.get_x(),eso.get_y()),
						 r_a(_f.nb_var()), r_b(_f.nb_var()), r_c(_f.nb_var()),
						 _box(_f.nb_var()), _g(_f.nb_var()), g(ff.nb_var()), ga(_f.nb_var()),
						 gb(_f.nb_var()), aux(_f.nb_var())	{

	for(int o=0; o<_f.nb_var(); o++)
		r_c[o]=1.0;

	occ = new vector<int>[ff.nb_var()];
	//occ[i][j] -> var in _f

	int* var;
	eso.var_map(var);

	for(int o=0; o<_f.nb_var(); o++) {
		if (ff.used(var[o]))
			occ[var[o]].push_back(o);

	}

	delete[] var;

}


void Function_OG::set_ra(int i, Interval val) {
	for(unsigned int j=0; j<occ[i].size(); j++)
		//~ for(int occ=first_occ[i];occ<first_occ[i+1];occ++) //FOR EACH occurrence occ
		r_a[occ[i][j]]=val;
}

void Function_OG::set_rb(int i, Interval val) {
	for(unsigned int j=0; j<occ[i].size(); j++)
		//~ for(int occ=first_occ[i];occ<first_occ[i+1];occ++) //FOR EACH occurrence occ
		r_b[occ[i][j]]=val;
}

void Function_OG::set_rc(int i, Interval val) {
	for(unsigned int j=0; j<occ[i].size(); j++)
		//~ for(int occ=first_occ[i];occ<first_occ[i+1];occ++) //FOR EACH occurrence occ
		r_c[occ[i][j]]=val;
}

//initialize the occurrence-based box using a normal box variable-based
void Function_OG::_setbox(IntervalVector& box) {
	int j=0;
	for(int i=0; i<box.size();i++) {
		if (occ[i].size()!=0) // _box[first_occ[i]]=box[i];
			//else
			for(unsigned int j=0; j<occ[i].size(); j++)
				//for(int occ=first_occ[i];occ<first_occ[i+1];occ++)
				_box[occ[i][j]]=box[i];

	}
}

namespace {
Array<Ctc> convert(const Array<NumConstraint>& csp, double epsilon, double univ_newton_min_width, double tau_mohc, bool amohc) {
	std::vector<Ctc*> vec;
	for (int i=0; i<csp.size(); i++) {
		vec.push_back(new CtcMohcRevise(csp[i], epsilon, univ_newton_min_width, tau_mohc, amohc));
	}
	return vec;
}
}

CtcMohc::CtcMohc(const Array<NumConstraint>& csp, double ratio, bool incremental,  double epsilon,
		double univ_newton_min_width, double tau_mohc) :
				CtcPropag(convert(csp,epsilon, univ_newton_min_width, tau_mohc, (tau_mohc==ADAPTIVE)), ratio, incremental) {

	active_mono_proc=new int[csp.size()];

	update_active_mono_proc = (tau_mohc <= 1.0);

	for(int i=0;i<csp.size();i++) active_mono_proc[i]=1;
	//  flags[FIXPOINT]=false;

}

CtcMohc::CtcMohc(const Array<NumConstraint>& csp, int* active_mono_proc, double ratio, bool incremental,  double epsilon,
		double univ_newton_min_width) :
				CtcPropag(convert(csp,epsilon, univ_newton_min_width, 1.0, false), ratio, incremental),
				active_mono_proc(active_mono_proc), update_active_mono_proc(false) {

	if (!active_mono_proc) {
		active_mono_proc=new int[csp.size()];
		for(int i=0;i<csp.size();i++) active_mono_proc[i]=1;
	}
	// flags[FIXPOINT]=false;
}

CtcMohc::~CtcMohc() {
	for (int i=0; i<list.size(); i++)
		delete &list[i];
}

}


