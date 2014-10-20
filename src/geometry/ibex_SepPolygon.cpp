//============================================================================
//                                  I B E X
// File        : Separator for the constraint : Point inside a polygon
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Apr 25, 2012
//============================================================================

#include "ibex_SepPolygon.h"
#include "ibex_Ctc.h"
#include "ibex_CtcSegment.h"
#include "ibex_Array.h"
#include "ibex_BoolInterval.h"

#include <cmath>

using namespace ibex;

namespace {

double argument(double mx, double my, double xa, double ya, double xb, double yb) {
	double ma_x = xa - mx;
	double ma_y = ya - my;
	double mb_x = xb - mx;
	double mb_y = yb - my;

	double norm_ma = hypot(ma_x, ma_y);
	double norm_mb = hypot(mb_x, mb_y);

	//    ma_x /= norm_ma;
	//    ma_y /= norm_ma;

	//    mb_x /= norm_mb;
	//    mb_y /= norm_mb;

	double cos_theta = (ma_x*mb_x + ma_y*mb_y) / (norm_ma*norm_mb);
	double sin_theta = (ma_x*mb_y - ma_y*mb_x) / (norm_ma*norm_mb);
	//double mod = (sin_theta >= 0 ) ? 1 : -1;
	return atan2(sin_theta,cos_theta);//mod *acos(cos_theta);
}


// TODO: Benoit, check this.
Array<Ctc> segment_ctc_list(vector<double>& ax, vector<double>& ay, vector<double>& bx, vector<double>& by) {
	Array<Ctc> l(ax.size());

	for(uint i=0; i<ax.size(); i++) {
		l.set_ref(i, *new CtcSegment(ax[i],ay[i],bx[i],by[i]));
	}
	return l;
}

} // end anonymous namespace

namespace ibex {

SepPolygon::SepPolygon(vector<double> &_ax, vector<double> &_ay, vector<double> &_bx, vector<double> &_by) :
    		Sep(),
    		ax(_ax),
    		ay(_ay),
    		bx(_bx),
    		by(_by),
    		multiple(_ax.size()),
    		constant(_ax.size()),
    		//C(_ax,_ay, _bx, _by),
    		cseg(segment_ctc_list(_ax,_ay, _bx, _by)),
    		inverse(true) {

	precalc_values();
}

SepPolygon::~SepPolygon() {
	for(uint i=0; i<cseg.list.size(); i++) {
		delete &cseg.list[i];
	}
}


//  Globals which should be set before calling these functions:
//
//  int    polySides  =  how many corners the polygon has
//  float  ax[]    =  horizontal coordinates of corners
//  float  ay[]    =  vertical coordinates of corners
//  float  x, y       =  point to be tested
//
//  The following global arrays should be allocated before calling these functions:
//
//  float  constant[] = storage for precalculated constants (same size as ax)
//  float  multiple[] = storage for precalculated multipliers (same size as ax)
//
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//
//  USAGE:
//  Call precalc_values() to initialize the constant[] and multiple[] arrays,
//  then call pointInPolygon(x, y) to determine if the point is in the polygon.
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.

void SepPolygon::precalc_values() {

	int   i, j=ax.size()-1;

	for(i=0; i<ax.size(); i++) {
		if(ay[j]==ay[i]) {
			constant[i]=ax[i];
			multiple[i]=0;
		} else {
			constant[i]=ax[i]-(ay[i]*ax[j])/(ay[j]-ay[i])+(ay[i]*ax[i])/(ay[j]-ay[i]);
			multiple[i]=(ax[j]-ax[i])/(ay[j]-ay[i]);
		}
		j=i;
	}
}

void SepPolygon::inv() {
	//    std::cout << inverse << std::endl;
	inverse = (inverse == true) ? false: true;
}

int SepPolygon::pnpoly(double testx, double testy) {

	int i, j, c = 0;
	int nvert = ay.size();
	for (i = 0, j = nvert-1; i < nvert; j = i++) {
		if ( ((ay[i]>testy) != (ay[j]>testy)) &&
				(testx < (ax[j]-ax[i]) * (testy-ay[i]) / (ay[j]-ay[i]) + ax[i]) )
			c = !c;
	}
	return c;
}

//===================================================================
bool SepPolygon::check(IntervalVector& X) {
	double mx = X[0].mid();
	double my = X[1].mid();

	//double cos_theta = 0;
	double theta = 0;
	for(uint i = 0; i < ax.size(); i++){
		theta += argument(mx,my,ax[i],ay[i],bx[i],by[i]);
	}

	if(theta > 1) return true;
	return false;
}


void SepPolygon::separate(IntervalVector& Xin, IntervalVector& Xout) {
	IntervalVector box = Xin & Xout;
	IntervalVector box0 = box;
	try {

		cseg.contract(box);
	} catch(EmptyBoxException& e) {
		//        throw e;
	}
	Xin = box; Xout = box;
	if (box==box0) return;     // nothing contracted.
	IntervalVector* rest;
	int n=box0.diff(box,rest); // calculate the set difference
	for(uint i = 0; i < n; i++){
		if(pnpoly(rest[i][0].mid(),rest[i][1].mid()) == inverse) {
			Xout |= rest[i];
		} else {
			Xin |= rest[i];
		}
	}
	delete[] rest;
	//    Xin = box; Xout = box;
}

};
