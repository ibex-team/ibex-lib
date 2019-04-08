//============================================================================
//                                  I B E X                                   
// File        : Inner arithmetic functions
// Author      : Gilles Chabert, Ignacio Araya
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 2, 2012
// Last Update : Jul 2, 2012
//============================================================================

#include "ibex_InnerArith.h"
#include "ibex_Random.h"

//#include <stdlib.h>
#include <cassert>
#include <iomanip>
#include <limits>

using namespace std;

namespace ibex {

namespace {

enum { ADD, SUB, MUL, DIV};

//std::string op_str(int i) {
//  switch(i) {
//    case ADD: return " + ";
//    case SUB: return " - ";
//    case MUL: return " * ";
//    default : return " / ";
//  }
//}

double max(const double& x, const double& y) {
	return x>y? x : y;
}

double min(const double& x, const double& y) {
	return x<y? x : y;
}

/* [gch] NOTE: Perhaps all the functions UPxx and LOxx below
 * could be implemented more efficiently by using fpu_round_down()
 * and fpu_round_up(). But it not sure (time to change processor
 * rounding mode has to be considered).
 */

double UP(Interval (*F)(const Interval&), const double& _real_) {
	return F(Interval(_real_,_real_)).ub();
}

double LO(Interval (*F)(const Interval&), const double& _real_) {
	return F(Interval(_real_,_real_)).lb();
}

double UP2(Interval (*F)(const Interval&, const Interval&), const double& _real1_, const double& _real2_) {
	return F(Interval(_real1_,_real1_),Interval(_real2_,_real2_)).ub();
}

double LO2(Interval (*F)(const Interval&, const Interval&), const double& _real1_, const double& _real2_) {
	return F(Interval(_real1_,_real1_),Interval(_real2_,_real2_)).lb();
}

double UP_root(const double& _real_, int expon) {
	return root(Interval(_real_,_real_), expon).ub();
}

double LO_root(const double& _real_, int expon) {
	return root(Interval(_real_,_real_), expon).lb();
}

double projx(double z, double y, int op, bool round_up) {
  (round_up)? fpu_round_up() : fpu_round_down();
  switch(op) {
    case ADD: return z-y;
    case SUB: return z+y;
    case MUL: return (y==0)? POS_INFINITY:z/y;
    default:  return z*y;
  }
  //fpu_round_near(); // unreachable!
}

double projy(double z, double x, int op, bool round_up) {
  (round_up)? fpu_round_up() : fpu_round_down();
  switch(op) {
    case ADD: return z-x;
    case SUB: return x-z;
    case MUL:
    	//assert(z!=0); // z==0 should not appear
    	assert(x!=0); // x==0 should not appear
    	return z/x;
    default: return (z==0)? POS_INFINITY:x/z;
  }
  //fpu_round_near(); // unreachable!
}


/**
 * Contract x and y so that the resulting box is inner w.r.t.
 *                      (x op y)>=z_inf or (x op y)<=z
 * where op is a monotonous function wrt x and y. The first case corresponds to geq==true.
 *
 * If xin and yin are non-empty, the returned box (x,y) is guaranteed to be
 * a superset of (xin,yin). Otherwise, xin and yin are ignored.
 *
 * Requirements:
 * 1- the function op(x,y) is monotonous wrt x and y in [x] x [y].
 *    In particular, if op==MUL, [x]x[y] must be included in a single quadrant.
 * 2- [xin]x[yin] is contained in [x]x[y]
 * 3- and [op]([xin],[yin]) is contained in [z]
 * 4- if op==MUL the following cases are not handled here: z=0 or x=[0,0] or y=[0,0].
 *
 * \param inc_var1 : true<=>op is increasing wrt x (false<=>decreasing)
 * \param inc_var2 : true<=>op is increasing wrt y (false<=>decreasing)

 * \pre xin.is_empty()<=>yin.is_empty()
 */
bool ibwd_cmp_mono_op(bool geq, double z, Interval& x, Interval& y, const Interval& xin, const Interval& yin, int op, bool inc_var1, bool inc_var2) {
	/*volatile?*/ double xmin, xmax;
	/*volatile?*/ double x0,y0;
	/*volatile?*/ double y1,y2;
	bool inflate=!xin.is_empty();

	assert(xin.is_subset(x));
	assert(yin.is_subset(y));
	//assert(!inflate || eval(xin,yin,op).lb()>=z_inf); // does this condition should really hold?
	//pb: this function is used for <= with add/sub
	//cout << "----------------------------------------------------------" << endl;
	//cout << "  cmp_mono_op " << op_str(op) << " " << (geq? "GEQ" : "LEQ") << endl;
	//cout << "  x=" << x << " y=" << y << " z=" << z << endl;
	//cout << "  inc_var1=" << inc_var1 << " inc_var2=" << inc_var2 << endl;
	if (inflate) { 
		//cout << "  inflating xin=" << xin << " yin=" << yin << endl;
	}
	if (x.is_empty() || y.is_empty() || (geq && z==POS_INFINITY) || (!geq && z==NEG_INFINITY)) {
		//cout << "  result: x=" << x << " y=" << y << endl;
		//cout << "----------------------------------------------------------" << endl;
		return false;
	}
	else if ((geq && z==NEG_INFINITY) || (!geq && z==POS_INFINITY)) {
		//cout << "  result: x=" << x << " y=" << y << endl;
		//cout << "----------------------------------------------------------" << endl;
		return true; // note: we also know that (x,y) is not empty.
	}

	// we bring back to the GEQ case
	// by simply reverting the directions
	// (but this is not very clean, see remark below)
	if (!geq) {
		inc_var1 = !inc_var1;
		inc_var2 = !inc_var2;
	}

	if (inc_var1) {
		if (inc_var2) {
			y1=inflate ? yin.lb() : y.ub();
			y2=y.lb();
		} else {
			y1=inflate? yin.ub() : y.lb();
			y2=y.ub();
		}
	}
	else {
		if (inc_var2) {
			y1=y.lb();
			y2=inflate? yin.lb() : y.ub();
		} else {
			y1=y.ub();
			y2=inflate? yin.ub() : y.lb();
		}
	}

	//cout << "  y1=" << y1 << " y2=" << y2 << endl;

	xmin=projx(z, y1, op, inc_var1);
	// remark: to avoid the following test, we cannot
	// return -oo in projx if round_up==false
	// because we want sometimes to get +oo "rounded by below"
	// (ex: op==ADD, x,y>0, z_sup=+oo inc_var and inc_var are both false (geq==false))
	if (op==MUL && xmin==POS_INFINITY) xmin=NEG_INFINITY;
	xmax=projx(z, y2, op, inc_var1); //true->ROUND_UP, false->ROUND_DOWN

	//cout << "  xmin=" << xmin << " xmax=" << xmax << endl;

	if (xmax==POS_INFINITY) {
		xmax=x.ub();
		//cout << "  xmax=" << xmax << endl;
	}

	if (xmin==NEG_INFINITY) {
		xmin=x.lb();
		//cout << "  xmin=" << xmin << endl;
	}


	if ((inc_var1 && xmin > x.ub()) || (!inc_var1 && xmax < x.lb())) {
		// this may happen including with inflate mode.
		// e.g.: x=<1,1>, y=[0,eps] and z=1. then xmax<1.
		if (!inc_var1) fpu_round_up(); // default mode. TODO: valid for gaol and... ?
				if (inflate) {x=xin; y=yin; return true;}
		else {
		x.set_empty();
		y.set_empty();
		return false;
		}
		//cout << "  result: x=" << x << " y=" << y << endl;
		//cout << "----------------------------------------------------------" << endl;
		//return false;
	} else if((inc_var1 && xmax < x.lb()) || (!inc_var1 && xmin > x.ub())) {
		// all the box is inner
		x0=(inc_var1)? x.lb():x.ub();
		//cout << "  x0 =" << x0 << endl;
	} else {
		if (inflate) {
			if (inc_var1) { if (xmax>xin.lb()) xmax=xin.lb(); }
			else          { if (xmin<xin.ub()) xmin=xin.ub(); }
			if (xmin>xmax) {
				if (!inc_var1) fpu_round_up(); // default mode. TODO: valid for gaol and... ?
				x=xin;
				y=yin;
				return true;
			}
		}
		//cout << "  xmin=" << xmin << " xmax=" << xmax << endl;
		Interval xx= x & Interval(xmin,xmax);

		//x0= xx.lb() + (double)RNG::rand()/(double)RAND_MAX*xx.diam();
		x0= RNG::rand(xx.lb(),xx.ub());

		//cout << "  x0 (random) =" << x0 << endl;
		if (!xx.contains(x0)) x0= (x0 < xx.lb())? xx.lb():xx.ub();
	}

	y0=projy(z,x0,op,inc_var2);
	//cout << "  y0=" << y0 << endl;

	if (y0!=POS_INFINITY) {
		if(y0>y.ub()) y0=y.ub();
		else if(y0<y.lb()) y0=y.lb();

		y = (inc_var2)? Interval(y0,y.ub()): Interval(y.lb(),y0);
	}

	x = (inc_var1)? Interval(x0,x.ub()):Interval(x.lb(),x0);

	if (!inc_var1 || !inc_var2) fpu_round_up(); // default mode. TODO: valid for gaol and... ?
	// [gch] if op==MUL and z=0 we have y=[0,0]
	// and x=[x^-,x0] (or x=[x0,x^+]) which is correct in both
	// case although we could take x entirely in this case.
	//cout << "  result: x=" << x << " y=" << y << endl;
	//cout << "----------------------------------------------------------" << endl;
	assert(xin.is_subset(x));
	assert(yin.is_subset(y));

	return true;
}

inline bool ibwd_leq_add(double z_sup, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return ibwd_cmp_mono_op(false,z_sup,x,y,xin,yin,ADD,true,true);
}

inline bool ibwd_leq_sub(double z_sup, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return ibwd_cmp_mono_op(false, z_sup,x,y,xin,yin,SUB,true,false);
}

// Difference with ibwd_cmp_mono_op: we do not require monotonicity
bool ibwd_leq_mul(double z_sup, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {

	bool inflate=!xin.is_empty();

	//cout << "[mul] x=" << x << " y=" << y << " z_sup=" << z_sup << endl;
	//if (inflate)
	//cout << "  inflating xin=" << xin << " yin=" << yin << endl;

	if (z_sup==POS_INFINITY) return true;

	else if (z_sup==NEG_INFINITY) {
		x.set_empty();
		y.set_empty();
		return false;
	}
	else if (z_sup>0) {
		// xxin and yyin are introduced because the box (xin,yin)
		// may not be included in the current quadrant (here x>0, y>0).
		// Still, we have to take it into account. (xxin,yyin) is the
		// projection of (xin,yin) onto the current quadrant
		Interval xxin,yyin;

		Interval xP = x & Interval::POS_REALS;
		Interval yP = y & Interval::POS_REALS;
		/* volatile? */double xU=x.ub();
		/* volatile? */double yU=y.ub();

		// ------------------------ quadrant x>0 y>0 ----------------------------------
		if(!xP.is_empty() && !yP.is_empty() && xP.ub()>0 && yP.ub()>0) { //(!inflate || yin.lb()>0)) {
			// if xP.ub()==0 or yP.ub()==0, the upper right corner is safe
			// and these cases cannot be handled properly by ibwd_cmp_mono_op
			xxin=inflate? Interval(max(0.0,xin.lb()), max(0.0,xin.ub())) : Interval::EMPTY_SET;
			yyin=inflate? Interval(max(0.0,yin.lb()), max(0.0,yin.ub())) : Interval::EMPTY_SET;
			if(!ibwd_cmp_mono_op(false, z_sup, xP, yP, xxin, yyin, MUL, true, true)) {
				//cout << "[mul] nothing in the positive quadrant --> x and y empty" << endl;
				x.set_empty();
				y.set_empty();
				return false;
			}
			xU = xP.ub();
			yU = yP.ub();
			//cout << "[mul] bounds in the positive quadrant xU=" << xU << " yU=" << yU << endl;
		}

		Interval xN = x & Interval::NEG_REALS;
		Interval yN = y & Interval::NEG_REALS;
		/* volatile? */double xL=x.lb();
		/* volatile? */double yL=y.lb();

		// ------------------------ quadrant x<0 y<0 ----------------------------------
		if(!xN.is_empty() && !yN.is_empty() && xN.lb()<0 && yN.lb()<0) { //(!inflate || yin.lb()<0)) {
			// same remark as above for xN.lb()<0 && yN.lb()<0.
			xxin=inflate? Interval(min(0.0,xin.lb()),min(0.0,xin.ub())) : Interval::EMPTY_SET;
			yyin=inflate? Interval(min(0.0,yin.lb()),min(0.0,yin.lb())) : Interval::EMPTY_SET;
			if(!ibwd_cmp_mono_op(false, z_sup, xN, yN, xxin, yyin, MUL, false, false)) {
				//cout << "[mul] nothing in the negative quadrant --> x and y empty" << endl;
				x.set_empty();
				y.set_empty();
				return false;
			}
			xL = xN.lb();
			yL = yN.lb();
			//cout << "[mul] bounds in the negative quadrant xL=" << xL << " yL=" << yL << endl;
		}

		assert(xL<=xU && yL<=yU);

		x=Interval(xL, xU);
		y=Interval(yL, yU);
		return true;
	}

	else if (z_sup==0) {
		//cout << "z_sup=0 x=" << x <<" xin=" << xin " y=" << y << endl;
		if (x.lb()>0) {
			assert(yin.is_subset(Interval::NEG_REALS));
			return !(y&=Interval::NEG_REALS).is_empty();
		}
		else if (x.ub()<0) {
			assert(yin.is_subset(Interval::POS_REALS));
			return !(y&=Interval::POS_REALS).is_empty();
		} else if (y.lb()>0) {
			assert(xin.is_subset(Interval::NEG_REALS));
			return !(x&=Interval::NEG_REALS).is_empty();
		}
		else if (y.ub()<0) {
			assert(xin.is_subset(Interval::POS_REALS));
			return !(x&=Interval::POS_REALS).is_empty();
		} else if ((x.lb()==0 && x.ub()==0) || (y.lb()==0 && y.ub()==0)) {
			return true;
		} else {
			if (inflate) {
				if ((xin.lb()>=0 && xin.ub()>0) || (yin.ub()<=0 && yin.lb()<0)) {
					assert(xin.is_subset(Interval::POS_REALS));
					assert(yin.is_subset(Interval::NEG_REALS));
					return (!(x&=Interval::POS_REALS).is_empty()) &&
						   (!(y&=Interval::NEG_REALS).is_empty());
				}
				else if ((xin.ub()<=0 && xin.lb()<0) || (yin.lb()>=0 && yin.ub()>0)) {
					assert(xin.is_subset(Interval::NEG_REALS));
					assert(yin.is_subset(Interval::POS_REALS));
					return (!(x&=Interval::NEG_REALS).is_empty()) &&
						   (!(y&=Interval::POS_REALS).is_empty());
				} else if (xin.lb()<0 && xin.ub()>0) {
					// x does not change
					assert(yin==Interval::ZERO);
					return !(y&=Interval::ZERO).is_empty();
				} else if (yin.lb()<0 && yin.ub()>0) { // we have here xin==[0,0]
					assert(xin==Interval::ZERO);
					return !(x&=Interval::ZERO).is_empty();
				}
			}

			// (x,y) strictly contains 0, and either inflate==false or (xin,yin)==(0,0)
			// chose the quadrant where the surface is maximal
			double surf_xpos_yneg = -x.ub()*y.lb();
			double surf_xneg_ypos = -x.lb()*y.ub();

			assert(surf_xpos_yneg>=0);
			assert(surf_xneg_ypos>=0);

			if (surf_xneg_ypos > surf_xpos_yneg) {
				x&=Interval::NEG_REALS;
				y&=Interval::POS_REALS;
			} else {
				x&=Interval::POS_REALS;
				y&=Interval::NEG_REALS;
			}
			return true;
		}

	} else { // z_sup<0
		if (inflate) {
			// in this case, we directly know in which quadrant
			// an inner box has to be found
			if (xin.lb()>0) {
				assert(yin.ub()<=0);
				x &= Interval::POS_REALS;
				y &= Interval::NEG_REALS;
				// note: we know x.ub()>0 && y.lb()<0
				assert(yin.lb()<0);
				return ibwd_cmp_mono_op(false, z_sup, x, y, xin, yin, MUL, false, true);
			} else {
				assert(xin.ub()<=0); // => because xin.ub()>0 => z_sup=0.
				assert(yin.lb()>=0);
				x &= Interval::NEG_REALS;
				y &= Interval::POS_REALS;
				if (z_sup==0) return true;
				// note: we know x.ub()>0 && y.lb()<0
				assert(yin.ub()>0);
				return ibwd_cmp_mono_op(false, z_sup, x, y, xin, yin, MUL, true, false);
			}
		}

		// Disconnected set.
		// We have two possible quadrants: either Q=(x>0,y<0) or Q'=(x<0,y>0).
		// we try to build an inner box in one of these quadrants (chosen randomly)
		// and, if it fails, we try with the other one.

		Interval xsave=x; // for the 2nd quadrant
		Interval ysave=y;

		bool q=(RNG::rand()%2==1); // q==1 : we take first Q.
		x &= q? Interval::POS_REALS : Interval::NEG_REALS;
		y &= q? Interval::NEG_REALS : Interval::POS_REALS;

		if (z_sup==0 || ((q? x.ub()>0 : x.lb()<0) &&
						 (q? y.lb()<0 : y.ub()>0) &&
						 ibwd_cmp_mono_op(false, z_sup, x, y, xin, yin, MUL, !q, q))) {
			return true;
		} else {
			// intersection with the first quadrant did not succeed.
			// we try with the other one.
			x = xsave & (q? Interval::NEG_REALS : Interval::POS_REALS);
			y = ysave & (q? Interval::POS_REALS : Interval::NEG_REALS);
			if (z_sup==0)
				return true;
			else if ((q? x.lb()==0 : x.ub()==0) || (q? y.ub()==0 : y.lb()==0)) {
				x.set_empty();
				y.set_empty();
				return false;
			}
			else
				return ibwd_cmp_mono_op(false, z_sup, x, y, xin, yin, MUL, q, !q);
		}
	}
}

// Difference with ibwd_cmp_mono_op: we do not require monotonicity
bool ibwd_leq_div(double z_sup, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {

	bool inflate=!xin.is_empty();

	//cout << "[div] x=" << x << " y=" << y << " z_sup=" << z_sup << endl;

	if (z_sup==POS_INFINITY) {
		// We cannot simply return true (i.e., (x,y) is inner)
		// because all the points (x,y) with y=0 does not satisfy x/y<+oo
		// since x/y is not a number, except for O/O which is considered to
		// be a real value (0 or 1).

		// We chose one half-plane. TODO: if we are not inflating, we may chose it randomly.
		if (x==Interval::ZERO)                          { }
		else if (inflate && yin.contains(0))            { x&=Interval::ZERO; }
		else if (y.ub()>0 && (!inflate || yin.lb()>0))  { y&=Interval(next_float(0),POS_INFINITY); }
		else                                            { y&=Interval(NEG_INFINITY,previous_float(0)); }
		//cout << "[div] z_sup=+oo --> x=" << x << " y=" << y << endl;
		return true;
	}

	else if (z_sup==NEG_INFINITY) {
		x.set_empty();
		y.set_empty();
		return false;
	}

	else if (z_sup>0) {

		// We have 2 almost-disconnected sets, connected only by the origin.
		// We shall discard the origin point and only consider one of the
		// half-plane.
		// However, we have first to deal with the specific case where we
		// are in inflating mode and the box to be inflated is precisely the origin.
		if (inflate && yin.contains(0)) {
			x&=Interval::ZERO;
			//cout << "[div] 0 in yin --> x=" << x << " y=" << y << endl;
			return true;
		}

		// We consider now that we have 2 possible half-planes: y>0 or y<0.
		// If we are inflating, the half-plane is directly known.
		// Otherwise, we try to build an inner box in one of the half-planes
		// (chosen randomly) and, if it fails, we try with the other one.

		bool r=((RNG::rand()%2)==1); // r==1 : we take first y>0.

		for (int i=0; i<2; i++) {
			if ((i+r)%2==1) {
				// we need to restrict the sign of x as well for the division to be monotonous
				Interval xP = x & Interval::POS_REALS;
				Interval yP = y & Interval::POS_REALS;

				// ------------------------ quadrant x>0 y>0 ----------------------------------
				if(!xP.is_empty() && !yP.is_empty() && (!inflate || yin.lb()>0)) {
					Interval xxin=inflate? Interval(max(0.0,xin.lb()), max(0.0,xin.ub())) : Interval::EMPTY_SET;
					if(ibwd_cmp_mono_op(false, z_sup,xP,yP,xxin,yin,DIV,true,false)) {
						// we reintegrate the part of the quadrant (x<0,y>0)
						x = Interval(x.lb(), xP.ub());
						y = Interval(yP.lb(), y.ub());
						//cout << "[div] quadrant (pos,pos): x=" << x << " y=" << y << endl;
						return true;
					} else assert(!inflate);
				}
			} else {
				// we need to restrict the sign of x as well for the division to be monotonous
				Interval xN = x & Interval::NEG_REALS;
				Interval yN = y & Interval::NEG_REALS;

				// ------------------------ quadrant x<0 y<0 ----------------------------------
				if(!xN.is_empty() && !yN.is_empty() && (!inflate || yin.ub()<0)) {
					Interval xxin=inflate? Interval(min(0.0,xin.lb()),min(0,xin.ub())) : Interval::EMPTY_SET;
					if(ibwd_cmp_mono_op(false, z_sup,xN,yN,xxin,yin,DIV,false,true)) {
						// we reintegrate the part of the quadrant (x>0,y<0)
						x = Interval(xN.lb(), x.ub());
						y = Interval(y.lb(), yN.ub());
						//cout << "[div] quadrant (neg,neg): x=" << x << " y=" << y << endl;
						return true;
					} else assert(!inflate);
				}
			}
		}
		// both quadrants failed.
		x.set_empty();
		y.set_empty();
		return false;
	}
	else { // z_sup<=0

		if (inflate && yin.contains(0)) {
			// this is the only case were we can inflate inside
			// both quadrants (x<0,y>0) and (x>0,y<0), along the line x=0.
			assert(xin==Interval::ZERO);
			if (z_sup==0) {
				x=xin; // 0
			} else {
				assert(yin==Interval::ZERO);
				x=xin; // 0
				y=yin; // 0
			}
			return true;
		}

		int r=RNG::rand()%2; // r==1 : we take first y>0.

		for (int i=0; i<2; i++) {
			if ((i+r)%2==1) {
				Interval xN = x & Interval::NEG_REALS;
				Interval yP = y & Interval::POS_REALS;

				// ------------------------ quadrant x<0 y>0 ----------------------------------
				if(!xN.is_empty() && !yP.is_empty() && (!inflate || yin.lb()>0)) {
					if(z_sup==0 || ibwd_cmp_mono_op(false, z_sup,xN,yP,xin,yin,DIV,true,true)) {
						x = xN;
						y = yP;
						//cout << "[div] quadrant (neg,pos): x=" << x << " y=" << y << endl;
						return true;
					} else assert(!inflate);
				}
			} else {
				Interval xP = x & Interval::POS_REALS;
				Interval yN = y & Interval::NEG_REALS;

				// ------------------------ quadrant x>0 y<0 ----------------------------------
				if(!xP.is_empty() && !yN.is_empty() && (!inflate || yin.ub()<0)) {
					if(z_sup==0 || ibwd_cmp_mono_op(false, z_sup,xP,yN,xin,yin,DIV,false,false)) {
						x = xP;
						y = yN;
						//cout << "[div] quadrant (pos,neg): x=" << x << " y=" << y << endl;
						return true;
					} else assert(!inflate);
				}
			}
		}
		// both quadrants failed.
		x.set_empty();
		y.set_empty();
		return false;
	}
}

inline bool ibwd_geq_add(double z_inf, Interval& x, Interval& y, const Interval &xin, const Interval& yin ) {
	return ibwd_cmp_mono_op(true,z_inf,x,y,xin,yin,ADD,true,true);
}

inline bool ibwd_geq_sub(double z_inf, Interval& x, Interval& y, const Interval &xin, const Interval& yin ) {
	return ibwd_cmp_mono_op(true, z_inf,x,y,xin,yin,SUB,true,false);
}

bool ibwd_geq_mul(double z_inf, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	Interval x2(-x);
	bool res=ibwd_leq_mul(-z_inf,x2,y,-xin,yin);
	x=-x2;
	return res;
}

bool ibwd_geq_div(double z_inf, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	Interval x2(-x);
 	bool res=ibwd_leq_div(-z_inf,x2,y,-xin,yin);

 	// note: may result in spurious negative 0 sign (ex: [-0,1])
	x=-x2;
	return res;
}

} // end namespace

bool ibwd_add(const Interval& z, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return ibwd_leq_add(z.ub(),x,y,xin,yin) && ibwd_geq_add(z.lb(),x,y,xin,yin);
}

bool ibwd_sub(const Interval& z, Interval& x, Interval& y, const Interval& xin, const Interval& yin) {
	return ibwd_leq_sub(z.ub(),x,y,xin,yin) && ibwd_geq_sub(z.lb(),x,y,xin,yin);
}

bool ibwd_mul(const Interval& z, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return ibwd_leq_mul(z.ub(),x,y,xin,yin) && ibwd_geq_mul(z.lb(),x,y,xin,yin);
}

bool ibwd_abs(const Interval& y, Interval& x, const Interval& xin) {
	bool inflate=!xin.is_empty();
	assert(xin.is_subset(x));
	assert(!inflate || abs(xin).is_subset(y));

	double up=y.ub();
	double lo=y.lb();

	if (y.is_empty() || up<0) {
		x.set_empty();
		return false;
	}

	if (lo>0) {
		if (inflate) {
			assert(!xin.contains(0));
			if (xin.lb()>0)
				x &= Interval(lo,up);
			else                      // we cannot have 0 in x since sqr(x) must be included in y
				x &= Interval(-up,-lo);
			return true;
		}
		else {
			Interval xtmp=x;
			bool q=(RNG::rand()%2==1); // q==1 : we first consider x>0.

			x &= (q? Interval(lo,up) : Interval(-up,-lo));
			if (!x.is_empty()) return true;

			x = xtmp & (q? Interval(-up,-lo) : Interval(lo,up));
			return !x.is_empty();
		}
	}
	else {
		x &= Interval(-up,up);
		return !x.is_empty();
	}
}

bool ibwd_div(const Interval& z, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return ibwd_leq_div(z.ub(),x,y,xin,yin) && ibwd_geq_div(z.lb(),x,y,xin,yin);
}


bool ibwd_max(const Interval& z, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {

	assert((xin.is_empty() && yin.is_empty()) || (!xin.is_empty() && !yin.is_empty()));

	if (z.is_empty()) {
		x.set_empty();
		y.set_empty();
	}

	if (x.is_empty() || y.is_empty()) return false;

	// the first part is the same as bwd_max:
	/* ---- Disjoint intervals ---- */
	if (y.lb()>x.ub() || z.lb()>x.ub()) {
		/* then, max(x,y) is necessarily y */
		if ((y &= z).is_empty()) {
			assert(xin.is_empty() && yin.is_empty());
			x.set_empty();
			return false;
		}
		else return true;
	} else if (x.lb()>y.ub() || z.lb()>y.ub()) {
		if ((x &= z).is_empty()) {
			assert(xin.is_empty() && yin.is_empty());
			y.set_empty();
			return false;
		}
		else return true;
	}
	/*------------------------------*/

	if (z.ub()<x.lb() || z.ub()<y.lb()) {
		assert(xin.is_empty() && yin.is_empty());
		x.set_empty();
		y.set_empty();
		return false; // inconsistency
	}

	/* At this point, x, y and z all mutually intersect. */
	double u1 = x.ub()>z.ub() ? z.ub() : x.ub();
	double u2 = y.ub()>z.ub() ? z.ub() : y.ub();

	assert(xin.is_empty() || xin.lb()<=u1);
	assert(yin.is_empty() || yin.lb()<=u2);

	if (x.lb() >= z.lb() || y.lb() >= z.lb()) {
		x = Interval(x.lb(), u1);
		y = Interval(y.lb(), u2);
		return true;
	}

	// the second part is specific to inner projection:
	// We have to chose which interval is contracted to the lower bound of z

	bool x_contracted=true; // is the lower bound of x to be contracted?

	if (!xin.is_empty() && xin.lb()<z.lb()) x_contracted=false;
	else if (!yin.is_empty() && yin.lb()<z.lb()) x_contracted=true;
	else if (x.lb()<y.lb()) x_contracted=false; // try to get the inner box of maximal size
	else x_contracted=true;

	if (x_contracted) {
		x = Interval(z.lb(), u1); // lower bound of x contracted
		y = Interval(y.lb(), u2);
	} else {
		x = Interval(x.lb(), u1);
		y = Interval(z.lb(), u2); // lower bound of y contracted
	}
	return true;
}


bool ibwd_min(const Interval& z, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {

	Interval mx=-x;
	Interval my=-y;
	Interval mxin=-xin;
	Interval myin=-yin;

	if (!ibwd_max(-z,mx,my,mxin,myin)) {
		x.set_empty();
		y.set_empty();
		return false;
	}

	x=-mx;
	y=-my;
	return true;
}

// [gch]
bool ibwd_sqr(const Interval& y, Interval& x, const Interval& xin) {
	return ibwd_pow(y,x,2,xin);
}

bool ibwd_pow(const Interval& y, Interval& x, int p, const Interval &xin) {
	//   //cout << "[sqr] xin=" << xin << " x=" << x << " y=" << y << endl;
	// Interval xini(X);
	bool inflate=!xin.is_empty();
	assert(xin.is_subset(x));
	assert(!inflate || (p==2 && sqr(xin).is_subset(y)) || (p!=2 && pow(xin,p).is_subset(y)));

	if (pow(x,p).is_subset(y)) return true;

	/* volatile */double lo;
	if (y.lb()==NEG_INFINITY)
		lo= p%2==0? POS_INFINITY : NEG_INFINITY;
	else
		lo= p==2? UP(sqrt,y.lb()) : UP_root(y.lb(), p);

	double up;
	if (y.ub()==POS_INFINITY)
		up= POS_INFINITY;
	else
		up= p==2? LO(sqrt,y.ub()) : LO_root(y.ub(), p);

	if (p % 2 ==0) { // even exponant
		if (up<0) up=0;                       // may happen because of rounding

		if (y.lb()>0) {
			if (lo<up) {                      // may not hold because of rounding
				if (inflate) {
					assert(!xin.contains(0));
					if (xin.lb()>0)
						x &= Interval(lo,up);
					else                      // we cannot have 0 in x since sqr(x) must be included in y
						x &= Interval(-up,-lo);
					// it may happen at this point that xin is not anymore included
					// in x (but still overlapping) because sqrt can be more
					// pessimistic than sqr
					x |= xin;
					return true;
				}
				else {
					// First alternative: choose randomly a side
					// ============================================================
					Interval xtmp=x;
					bool q=(RNG::rand()%2==1); // q==1 : we first consider x>0.

					x &= (q? Interval(lo,up) : Interval(-up,-lo));
					if (!x.is_empty()) return true;

					x = xtmp & (q? Interval(-up,-lo) : Interval(lo,up));
					return !x.is_empty();
					// ============================================================


					// second alternative: choose the side with maximal interval
					// ============================================================
//					Interval xtmp =x & Interval(lo,up);
//					x &= Interval(-up,-lo);
//					if (xtmp.diam()>x.diam()) x=xtmp;
//					return !x.is_empty();
					// ============================================================
				}
			}
			else {
				if (inflate) { x = xin; return true; }
				else { x.set_empty(); return false; }
			}
		} else {
			x &= Interval(-up,up);
			if (inflate) { x |= xin; return true; }                        // see comment above
			else return !x.is_empty();
		}
	} else { // odd exponant

		x &= Interval(lo,up);
		return !x.is_empty();
	}
	//assert(sqr(x).is_subset(y));        // note that sqr(x) might not be included anymore in y
	                                      // if sqr was more pessimistic than sqrt
	//  assert(xini.is_subset(x));
	//cout << "[sqr] result: x=" << x << endl;
}

#define PERIOD_COS(i) (i%2==0? iadd(period_0,imul((double) i,Interval::PI)) : isub(imul((double) i+1,Interval::PI),period_0))
#define PERIOD_SIN(i) (i%2==0? iadd(period_0,imul((double) i,Interval::PI)) : isub(imul((double) i,Interval::PI),period_0))
#define PERIOD_TAN(i) (iadd(period_0, imul((double) i,Interval::PI)))

#define COS 0
#define SIN 1
#define TAN 2

static bool ibwd_trigo(const Interval& y, Interval& x, const Interval& xin, int ftype) {

	bool inflate=!xin.is_empty();
	assert(xin.is_subset(x));

	Interval period_0, nb_period;

	switch (ftype) {
	case COS :
		if (y==Interval(-1,1)) return true;
		period_0 = iacos(y); break;
	case SIN :
		if (y==Interval(-1,1)) return true;
		period_0 = iasin(y); break;
	case TAN :
		period_0 = iatan(y); break;
	default :
		assert(false); break;
	}

	if (period_0.is_empty()) {
		// can happen even if inflate==true
		// because of inner rounding (to be checked?)
		if (inflate) x=xin;
		else x.set_empty();
		return inflate;
	}

	switch (ftype) {
	case COS :
		nb_period = (inflate? xin : x) / Interval::PI; break;
	case SIN :
		nb_period = ((inflate? xin : x) + Interval::HALF_PI) / Interval::PI; break;
	case TAN :
		nb_period = ((inflate? xin : x) + Interval::HALF_PI) / Interval::PI; break;
	}

	if (nb_period.lb() < std::numeric_limits<long>::min() ||
			nb_period.ub() > std::numeric_limits<long>::max()) {
		if (inflate) x=xin;
		else x.set_empty();
		return inflate;
	}

	long p1 = ((long) nb_period.lb())-1;
	long p2 = ((long) nb_period.ub())+1;

 	long i1 = p1;

	switch(ftype) {
	case COS :
		// should find in at most 2 turns.. but consider rounding !
		while (i1<=p2 && ((inflate? xin : x) & PERIOD_COS(i1)).is_empty()) i1++;
		break;
	case SIN :
		while (i1<=p2 && ((inflate? xin : x) & PERIOD_SIN(i1)).is_empty()) i1++;
		break;
	case TAN :
		while (i1<=p2 && ((inflate? xin : x) & PERIOD_TAN(i1)).is_empty()) i1++;
		break;
	}

	if (i1==p2+1) {
		// can happen even if inflate==true
		// since PERIOD_XXX can be the empty set
		// because of inner rounding
		if (inflate) x=xin;
		else x.set_empty();
		return inflate;
	}

	long i2 = p2;

	if (i1<p2) {
		switch(ftype) {
		case COS :
			// should find in at most 2 turns.. but consider rounding !
			while (i2>=p1 && ((inflate? xin : x) & PERIOD_COS(i2)).is_empty()) i2--;
			break;
		case SIN :
			while (i2>=p1 && ((inflate? xin : x) & PERIOD_SIN(i2)).is_empty()) i2--;
			break;
		case TAN :
			while (i2>=p1 && ((inflate? xin : x) & PERIOD_TAN(i2)).is_empty()) i2--;
			break;
		}
	}
	assert(i2>=p1 && i2>=i1);

	if (inflate) {
		switch(ftype) {
		case COS :
			x &= (PERIOD_COS(i1) | PERIOD_COS(i2));
			break;
		case SIN :
			x &= (PERIOD_SIN(i1) | PERIOD_SIN(i2));
			break;
		case TAN :
			// impossible to span more than one period
			// even if y=(-oo,+oo)
			assert(i1==i2);
			x &= PERIOD_TAN(i1);
			break;
		}
	}
	else {
		// choose randomly the period on which we project
		long i;
		if (i1==i2) i = i1; // no choice
		else i = i1+ (RNG::rand() % (i2-i1+1));

		switch(ftype) {
		case COS :
			if (y.ub()>=1.0) // the projection spans two periods
				x &= (PERIOD_COS(i) | (i%2==0? PERIOD_COS(i-1) : PERIOD_COS(i+1)));
			else if (y.lb()<=-1.0)
				x &= (PERIOD_COS(i) | (i%2==0? PERIOD_COS(i+1) : PERIOD_COS(i-1)));
			else // the case y==[-1,1] is already considered at the beginning
				x &= PERIOD_COS(i);
			break;
		case SIN :
			if (y.ub()>=1.0) // the projection spans two periods
				x &= (PERIOD_SIN(i) | (i%2==0? PERIOD_SIN(i+1) : PERIOD_SIN(i-1)));
			else if (y.lb()<=-1.0)
				x &= (PERIOD_SIN(i) | (i%2==0? PERIOD_SIN(i-1) : PERIOD_SIN(i+1)));
			else // the case y==[-1,1] is already considered at the beginning
				x &= PERIOD_SIN(i);
			break;
		case TAN :
			// impossible to span more than one period
			x &= PERIOD_TAN(i);
			break;
		}
	}

	return true;
}

bool ibwd_cos(const Interval& y, Interval& x, const Interval& xin) {
	return ibwd_trigo(y,x,xin,COS);
}

bool ibwd_sin(const Interval& y, Interval& x, const Interval& xin) {
	return ibwd_trigo(y,x,xin,SIN);
}

bool ibwd_tan(const Interval& y, Interval& x, const Interval& xin) {
	return ibwd_trigo(y,x,xin,TAN);
}

/*---------------------------------------------------------------------------*/

Interval iadd(const Interval& x, const Interval& y) {
	// note if the upper bound < lower bound,  the interval
	// will be automatically set to the empty interval
	return Interval(UP2(operator+,x.lb(),y.lb()),LO2(operator+,x.ub(),y.ub()));
}

Interval isub(const Interval& x, const Interval& y) {
	return Interval(UP2(operator-,x.lb(),y.ub()),LO2(operator-,x.ub(),y.lb()));
}

Interval imul(const Interval& x, const Interval& y) {

	if (x.is_empty() || y.is_empty())
			return Interval::EMPTY_SET;

	if (x==Interval::ZERO || y==Interval::ZERO)
		return Interval::ZERO;

	double lx=x.lb();
	double ux=x.ub();
	double ly=y.lb();
	double uy=y.ub();

	double l,u;

	if (lx>=0) {
		if (ly>=0)      { l =                                                       UP2(operator*, lx,ly);
		                  u = ux==POS_INFINITY || uy==POS_INFINITY ? POS_INFINITY : LO2(operator*, ux,uy); }
		else if (uy<=0) { l = ux==POS_INFINITY || ly==NEG_INFINITY ? NEG_INFINITY : UP2(operator*, ux,ly);
		                  u =                                                       LO2(operator*, lx,uy); }
		else            { l = ux==POS_INFINITY || ly==NEG_INFINITY ? NEG_INFINITY : UP2(operator*, ux,ly);
		                  u = ux==POS_INFINITY || uy==POS_INFINITY ? POS_INFINITY : LO2(operator*, ux,uy); }
	} else if (ux<=0) {
		return -imul(-x,y);
	} else {
		if (ly>=0)      { l = lx==NEG_INFINITY || uy==POS_INFINITY ? NEG_INFINITY : UP2(operator*, lx,uy);
						  u = ux==POS_INFINITY || uy==POS_INFINITY ? POS_INFINITY : LO2(operator*, ux,uy); }
		else if (uy<=0) { l = ux==POS_INFINITY || ly==NEG_INFINITY ? NEG_INFINITY : UP2(operator*, ux,ly);
						  u = lx==NEG_INFINITY || ly==NEG_INFINITY ? POS_INFINITY : LO2(operator*, lx,ly); }
		else {
			if (x.is_unbounded() || y.is_unbounded()) {
				l = NEG_INFINITY;
				u = POS_INFINITY;
			} else {
				l = std::min(UP2(operator*,lx,uy), UP2(operator*,ux,ly));
				u = std::max(LO2(operator*,ux,uy), LO2(operator*,lx,ly));
			}
		}
	}
	return Interval(l,u);
}

Interval idiv(const Interval& x, const Interval& y) {
	if (x.is_empty() || y.is_empty())
		return Interval::EMPTY_SET;

	double lx=x.lb();
	double ux=x.ub();
	double ly=y.lb();
	double uy=y.ub();

	if (ly==0 && uy==0)
		return Interval::EMPTY_SET;
	else if (lx==0 && ux==0)
		return  Interval::ZERO;
	else if (ly>0 || uy<0) {
		if (uy<0)
			if (ux<0)
				return Interval(UP2(operator/,ux,uy),LO2(operator/,lx,ly));
			else if (lx<0)
				return Interval(UP2(operator/,ux,uy),LO2(operator/,lx,uy));
			else
				return Interval(UP2(operator/,ux,ly),LO2(operator/,lx,uy));
		else // ly>0
			if (ux<0)
				return Interval(UP2(operator/,lx,uy),LO2(operator/,ux,ly));
			else if (lx<0)
				return Interval(UP2(operator/,lx,ly),LO2(operator/,ux,ly));
			else
				return Interval(UP2(operator/,lx,ly),LO2(operator/,ux,uy));
	}
	else if (ux<=0 && uy==0)
		return Interval(UP2(operator/,ux,ly), POS_INFINITY);
	else if (ux<=0 && ly<0 && uy>0)
		return Interval(NEG_INFINITY,LO2(operator/,ux,uy)); // we could also return Interval(UP2(operator/,ux,ly),POS_INFINITY);
	else if (ux<=0 && ly==0)
		return Interval(NEG_INFINITY, LO2(operator/,ux,uy));
	else if (lx>=0 && uy==0)
		return Interval(NEG_INFINITY, LO2(operator/,lx,ly));
				else if (lx>=0 && ly<0 && uy>0)
					return Interval(NEG_INFINITY,LO2(operator/,lx,ly)); // idem with Interval(UP2(operator/,lx,uy),POS_INFINITY);
				else if (lx>=0 && ly==0)
					return Interval(UP2(operator/,lx,uy), POS_INFINITY);
				else
					return Interval::ALL_REALS; // lx<0<ux && ly<=0<=uy


}

Interval isqr(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	double l,u;
	if (x.lb()==NEG_INFINITY) {
		l = x.ub()>=0 ? 0 : UP(sqr,x.ub());
		u = POS_INFINITY;
	}
	else if (x.ub()==POS_INFINITY) {
		l = x.lb()<=0 ? 0 : UP(sqr,x.lb());
		u = POS_INFINITY;
	}
	else if (x.ub()<-x.lb()) {
		l = x.ub()>=0 ? 0 : UP(sqr,x.ub());
		u = LO(sqr,x.lb());
	}
	else {
		l = x.lb()<=0 ? 0 : UP(sqr,x.lb());
		u=LO(sqr,x.ub());
	}
	return l>u? Interval::EMPTY_SET : Interval(l,u);
}

Interval ilog(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	double inf = x.lb()<=0 ?            NEG_INFINITY : UP(log,x.lb());
	double sup = x.ub()==POS_INFINITY ? POS_INFINITY : LO(log,x.ub());
	return inf>sup ? Interval::EMPTY_SET : Interval(inf,sup);
}

Interval iexp(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	double inf = x.lb()==NEG_INFINITY ? 0            : UP(exp,x.lb());
	double sup = x.ub()==POS_INFINITY ? POS_INFINITY : LO(exp,x.ub());
	return inf>sup ? Interval::EMPTY_SET : Interval(inf,sup);
}

Interval iacos(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	double inf = x.ub()>=1 ?  0                 : UP(acos,x.ub()); // condition added just in case x.ub()==+oo.
	double sup = x.lb()<=-1 ? Interval::PI.lb() : LO(acos,x.lb()); // idem (in case x.lb()==-oo)
	return inf>sup ? Interval::EMPTY_SET : Interval(inf,sup);
}

Interval iasin(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	double inf = x.lb()<=-1 ? -Interval::HALF_PI.lb() : UP(asin,x.lb()); // idem
	double sup = x.ub()>=1 ?   Interval::HALF_PI.lb() : LO(asin,x.ub()); // idem
	return inf>sup ? Interval::EMPTY_SET : Interval(inf,sup);
}

Interval iatan(const Interval& x) {
	if (x.is_empty()) return Interval::EMPTY_SET;
	double inf = x.lb()==NEG_INFINITY ? -Interval::HALF_PI.lb() : UP(atan,x.lb());
	double sup = x.ub()==POS_INFINITY ?  Interval::HALF_PI.lb() : LO(atan,x.ub());
	return inf>sup ? Interval::EMPTY_SET : Interval(inf,sup);
}


} // end namespace ibex
