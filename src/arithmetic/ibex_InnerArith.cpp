//============================================================================
//                                  I B E X                                   
// File        : ibex_InnerProj.cpp
// Author      : Gilles Chabert, Ignacio Araya
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 2, 2012
// Last Update : Jul 2, 2012
//============================================================================

#include "ibex_InnerArith.h"

#include <stdlib.h>
#include <cassert>

namespace ibex {

namespace {

enum { ADD, SUB, MUL, DIV};

string op_str(int i) {
  switch(i) {
    case ADD: return " + ";
    case SUB: return " - ";
    case MUL: return " * ";
    default : return " / ";
  }
}

double max(const double& x, const double& y) {
	return x>y? x : y;
}

double min(const double& x, const double& y) {
	return x<y? x : y;
}

double UP(Interval (*F)(const Interval&), const double& _real_) {
	return F(Interval(_real_,_real_)).ub();
}

double LO(Interval (*F)(const Interval&), const double& _real_) {
	return F(Interval(_real_,_real_)).lb();
}

double UP_root(const double& _real_, int expon) {
	return root(Interval(_real_,_real_), expon).ub();
}

double LO_root(const double& _real_, int expon) {
	return root(Interval(_real_,_real_), expon).lb();
}

Interval eval(const Interval& x, const Interval& y, int op) {
  switch(op) {
    case ADD: return x+y;
    case SUB: return x-y;
    case MUL: return x*y;
    default: return x/y;
  }
}

double projx(double z, double y, int op, bool round_up) {
  (round_up)? fpu_round_up() : fpu_round_down();
  switch(op) {
    case ADD: return z-y;
    case SUB: return z+y;
    case MUL:
    	assert(z!=0); // z==0 should not appear
    	return (y==0)? POS_INFINITY:z/y;
    default: return z*y;
  }
  //fpu_round_near(); // unreachable
}

double projy(double z, double x, int op, bool round_up) {
  (round_up)? fpu_round_up() : fpu_round_down();
  switch(op) {
    case ADD: return z-x;
    case SUB: return x-z;
    case MUL:
    	assert(z!=0); // z==0 should not appear
    	assert(x!=0); // x==0 should not appear
    	return z/x;
    default: return (z==0)? POS_INFINITY:x/z;
  }
  //fpu_round_near(); // unreachable
}


/**
 * Contract x and y so that the resulting box is inner w.r.t.
 *                      (x op y)>=z_inf
 * where op is a monotonous function wrt x and y.
 *
 * If xin and yin are non-empty, the returned box (x,y) is guaranteed to be
 * a superset of (xin,yin). Otherwise, xin and yin are ignored.
 *
 * Requirements:
 * 1- the function op(x,y) is monotonous wrt x and y in [x] x [y].
 *    In particular, if op==MUL, [x]x[y] must be included in a single quadrant.
 * 2- [xin]x[yin] is contained in [x]x[y]
 * 3- and [op]([xin],[yin]) is contained in [z]
 *
 * \param inc_var1 : true<=>op is increasing wrt x (false<=>decreasing)
 * \param inc_var2 : true<=>op is increasing wrt y (false<=>decreasing)

 * \pre xin.is_empty()<=>yin.is_empty()
 */
bool iproj_geq_mono_op(double z_inf, Interval& x, Interval& y, const Interval& xin, const Interval& yin, int op, bool inc_var1, bool inc_var2) {
	double xmin, xmax, x0,y0;
	double y1,y2;
	bool inflate=!xin.is_empty();

	assert(xin.is_subset(x));
	assert(yin.is_subset(y));
	//assert(!inflate || eval(xin,yin,op).lb()>=z_inf); // does this condition should really hold?
	//pb: this function is used for <= with add/sub
	//cout << "----------------------------------------------------------" << endl;
	//cout << "  geq_mono_op " << op_str(op) << endl;
	//cout << "  x=" << x << " y=" << y << " z_inf=" << z_inf << endl;
	//cout << "  inc_var1=" << inc_var1 << " inc_var2=" << inc_var2 << endl;
	if (inflate) //cout << "  inflating xin=" << xin << " yin=" << yin << endl;
	if (x.is_empty() || y.is_empty()) {
		//cout << "  result: x=" << x << " y=" << y << endl;
		//cout << "----------------------------------------------------------" << endl;
		return false;
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

	xmin=projx(z_inf, y1, op, inc_var1);
	// remark: to avoid the following test, we cannot
	// return -oo in projx if round_up==false
	// because we want sometimes to get +oo "rounded by below"
	// (ex: op==ADD, x,y>0, z_sup=+oo inc_var and inc_var are both false (leq_add))
	if (op==MUL && xmin==POS_INFINITY) xmin=NEG_INFINITY;
	xmax=projx(z_inf, y2, op, inc_var1); //true->ROUND_UP, false->ROUND_DOWN

	//cout << "  xmin=" << xmin << " xmax=" << xmax << endl;

	if (xmax==POS_INFINITY) {
		xmax=x.ub();
		//cout << "    xmax=" << xmax << endl;
	}

	if (xmin==NEG_INFINITY) {
		xmin=x.lb();
		//cout << "    xmin=" << xmin << endl;
	}


	if ((inc_var1 && xmin > x.ub()) || (!inc_var1 && xmax < x.lb())) {
		assert(!inflate);
		x.set_empty();
		y.set_empty();
		//cout << "  result: x=" << x << " y=" << y << endl;
		//cout << "----------------------------------------------------------" << endl;
		return false;
	} else if((inc_var1 && xmax < x.lb()) || (!inc_var1 && xmin > x.ub())) {
		// all the box is inner
		x0=(inc_var1)? x.lb():x.ub();
		//cout << "  x0 =" << x0 << endl;
	} else {
		if (inflate) {
			if (inc_var1) { if (xmax>xin.lb()) xmax=xin.lb(); }
			else          { if (xmin<xin.ub()) xmin=xin.ub(); }
		}
		//cout << "  xmin=" << xmin << " xmax=" << xmax << endl;
		Interval xx= x & Interval(xmin,xmax);
		x0= xx.lb() + (double)rand()/(double)RAND_MAX*xx.diam();
		//cout << "  x0 (random) =" << x0 << endl;
		if (!xx.contains(x0)) x0= (x0 < xx.lb())? xx.lb():xx.ub();
	}

	y0=projy(z_inf,x0,op,inc_var2);
	//cout << "  y0=" << y0 << endl;

	if (y0!=POS_INFINITY) {
		if(y0>y.ub()) y0=y.ub();
		else if(y0<y.lb()) y0=y.lb();

		y = (inc_var2)? Interval(y0,y.ub()): Interval(y.lb(),y0);
	}

	x = (inc_var1)? Interval(x0,x.ub()):Interval(x.lb(),x0);
	// [gch] if op==MUL and z_sup=0 we have y=[0,0]
	// and x=[x^-,x0] (or x=[x0,x^+]) which is correct in both
	// case although we could take x entirely in this case.
	//cout << "  result: x=" << x << " y=" << y << endl;
	//cout << "----------------------------------------------------------" << endl;
	assert(xin.is_subset(x));
	assert(yin.is_subset(y));

	return true;
}

bool iproj_leq_mono_op(double z_sup, Interval& x, Interval& y, const Interval& xin, const Interval& yin, int op, bool inc_var1, bool inc_var2) {
	return iproj_geq_mono_op(z_sup,x,y,xin,yin,op,!inc_var1,!inc_var2);
}

bool iproj_leq_add(double z_sup, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return iproj_leq_mono_op(z_sup,x,y,xin,yin,ADD,true,true);
}

bool iproj_leq_sub(double z_sup, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return iproj_leq_mono_op(z_sup,x,y,xin,yin,SUB,true,false);
}

// Difference with iproj_leq_mono_op: we do not require monotonicity
bool iproj_leq_mul(double z_sup, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {

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

	else if (z_sup==0) {
		if (xin==Interval::ZERO)
			return !(y&=Interval::ZERO).is_empty();
		else {
			assert(yin==Interval::ZERO);
			return !(x&=Interval::ZERO).is_empty();
		}
	}
	else if (z_sup>0) {
		// xxin and yyin are introduced because the box (xin,yin)
		// may not be included in the current quadrant (here x>0, y>0).
		// Still, we have to take it into account. (xxin,yyin) is the
		// projection of (xin,yin) onto the current quadrant
		Interval xxin,yyin;

		Interval xP = x & Interval::POS_REALS;
		Interval yP = y & Interval::POS_REALS;
		double xU=x.ub();
		double yU=x.ub();

		// ------------------------ quadrant x>0 y>0 ----------------------------------
		if(!xP.is_empty() && !yP.is_empty()) { //y.ub()>=0 && (!inflate || yin.lb()>0)) {
			xxin=inflate? Interval(max(0.0,xin.lb()), max(0.0,xin.ub())) : Interval::EMPTY_SET;
			yyin=inflate? Interval(max(0.0,yin.lb()), max(0.0,yin.ub())) : Interval::EMPTY_SET;
			if(!iproj_leq_mono_op(z_sup, xP, yP, xxin, yyin, MUL, true, true)) {
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
		double xL=x.lb();
		double yL=y.lb();

		// ------------------------ quadrant x<0 y<0 ----------------------------------
		if(!xN.is_empty() && !yN.is_empty()) { //y.ub()>=0 && (!inflate || yin.lb()>0)) {
			xxin=inflate? Interval(min(0.0,xin.lb()),min(0.0,xin.ub())) : Interval::EMPTY_SET;
			yyin=inflate? Interval(min(0.0,yin.lb()),min(0.0,yin.lb())) : Interval::EMPTY_SET;
			if(!iproj_leq_mono_op(z_sup, xN, yN, xxin, yyin, MUL, false, false)) {
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

	else { // z_sup<0

		if (inflate) {
			// in this case, we directly know in which quadrant
			// an inner box has to be found
			if (xin.lb()>0)
				return iproj_leq_mono_op(z_sup,x, y, xin, yin, MUL, false, true);
			else
				return iproj_leq_mono_op(z_sup, x, y, xin, yin, MUL, true, false);
		}

		// Disconnected set.
		// We have two possible quadrants: either Q=(x>0,y<0) or Q'=(x<0,y>0).
		// we try to build an inner box in one of these quadrants (chosen randomly)
		// and, if it fails, we try with the other one.

		Interval xsave=x; // for the 2nd quadrant
		Interval ysave=y;

		bool q=(rand()%2==1); // q==1 : we take first Q.
		x &= q? Interval::POS_REALS : Interval::NEG_REALS;
		y &= q? Interval::NEG_REALS : Interval::POS_REALS;

		if (iproj_leq_mono_op(z_sup, x, y, xin, yin, MUL, !q, q)) {
			return true;
		} else {
			// intersection with the first quadrant did not succeed.
			// we try with the other one.
			x = xsave & (q? Interval::NEG_REALS : Interval::POS_REALS);
			y = ysave & (q? Interval::POS_REALS : Interval::NEG_REALS);
			return iproj_leq_mono_op(z_sup, x, y, xin, yin, MUL, q, !q);
		}
	}
}

// Difference with iproj_leq_mono_op: we do not require monotonicity
bool iproj_leq_div(double z_sup, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {

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

		bool r=rand()%2; // r==1 : we take first y>0.

		for (int i=0; i<2; i++) {
			if ((i+r)%2==1) {
				// we need to restrict the sign of x as well for the division to be monotonous
				Interval xP = x & Interval::POS_REALS;
				Interval yP = y & Interval::POS_REALS;

				// ------------------------ quadrant x>0 y>0 ----------------------------------
				if(!xP.is_empty() && !yP.is_empty() && (!inflate || yin.lb()>0)) {
					Interval xxin=inflate? Interval(max(0.0,xin.lb()), max(0.0,xin.ub())) : Interval::EMPTY_SET;
					if(iproj_leq_mono_op(z_sup,xP,yP,xxin,yin,DIV,true,false)) {
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
					if(iproj_leq_mono_op(z_sup,xN,yN,xxin,yin,DIV,false,true)) {
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

		int r=rand()%2; // r==1 : we take first y>0.

		for (int i=0; i<2; i++) {
			if ((i+r)%2==1) {
				Interval xN = x & Interval::NEG_REALS;
				Interval yP = y & Interval::POS_REALS;

				// ------------------------ quadrant x<0 y>0 ----------------------------------
				if(!xN.is_empty() && !yP.is_empty() && (!inflate || yin.lb()>0)) {
					if(z_sup==0 || iproj_leq_mono_op(z_sup,xN,yP,xin,yin,DIV,true,true)) {
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
					if(z_sup==0 || iproj_leq_mono_op(z_sup,xP,yN,xin,yin,DIV,false,false)) {
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

bool iproj_geq_add(double z_inf, Interval& x, Interval& y, const Interval &xin, const Interval& yin ) {
	return iproj_geq_mono_op(z_inf,x,y,xin,yin,ADD,true,true);
}

bool iproj_geq_sub(double z_inf, Interval& x, Interval& y, const Interval &xin, const Interval& yin ) {
	return iproj_geq_mono_op(z_inf,x,y,xin,yin,SUB,true,false);
}

bool iproj_geq_mul(double z_inf, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	Interval x2(-x);
	bool res=iproj_leq_mul(-z_inf,x2,y,-xin,yin);
	x=-x2;
	return res;
}

bool iproj_geq_div(double z_inf, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	Interval x2(-x);
 	bool res=iproj_leq_div(-z_inf,x2,y,-xin,yin);
	x=-x2;
	return res;
}

} // end namespace

bool iproj_add(const Interval& z, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return iproj_leq_add(z.ub(),x,y,xin,yin) && iproj_geq_add(z.lb(),x,y,xin,yin);
}

bool iproj_sub(const Interval& z, Interval& x, Interval& y, const Interval& xin, const Interval& yin) {
	return iproj_leq_sub(z.ub(),x,y,xin,yin) && iproj_geq_sub(z.lb(),x,y,xin,yin);
}

bool iproj_mul(const Interval& z, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return iproj_leq_mul(z.ub(),x,y,xin,yin) && iproj_geq_mul(z.lb(),x,y,xin,yin);
}

bool iproj_div(const Interval& z, Interval& x, Interval& y, const Interval &xin, const Interval& yin) {
	return iproj_leq_div(z.ub(),x,y,xin,yin) && iproj_geq_div(z.lb(),x,y,xin,yin);
}

// [gch]
bool iproj_sqr(const Interval& y, Interval& x, const Interval& xin) {
	return iproj_pow(y,x,2,xin);
}

bool iproj_pow(const Interval& y, Interval& x, int p, const Interval &xin) {
	//   cout << "sqr x=" << xin << " x=" << x << " y=" << y << endl;
	// Interval xini(X);
	bool inflate=!xin.is_empty();
	assert(xin.is_subset(x));
	assert(!inflate || (p==2 && sqr(xin).is_subset(y)) || (p!=2 && pow(xin,p).is_subset(y)));

	double lo=p==2? UP(sqrt,y.lb()) : UP_root(y.lb(), p);
	double up=p==2? LO(sqrt,y.ub()) : LO_root(y.ub(), p);

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
					Interval xtmp=x;
					bool q=(rand()%2==1); // q==1 : we first consider x>0.

					x &= (q? Interval(lo,up) : Interval(-up,-lo));
					if (!x.is_empty()) return true;

					x = xtmp & (q? Interval(-up,-lo) : Interval(lo,up));
					return !x.is_empty();
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
	//cout << "new x=" << x << endl;
}



/*---------------------------------------------------------------------------*/

Interval isqr(const Interval& x) {
	double l,u;
	if (x.lb()==NEG_INFINITY) {
		l = x.ub()>=0 ? 0 : UP(sqr,x.ub());
		u = NEG_INFINITY;
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
