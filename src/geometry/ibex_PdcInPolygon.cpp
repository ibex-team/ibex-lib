//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcInPolygon.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#include "ibex_PdcInPolygon.h"

using namespace std;

namespace ibex {

PdcInPolygon::PdcInPolygon(vector<double> &_ax, vector<double> &_ay, vector<double> &_bx, vector<double> &_by) : Pdc(2),
    		ax(_ax),
    		ay(_ay),
    		bx(_bx),
    		by(_by),
    		multiple(_ax.size()),
    		constant(_ax.size()),
    		inverse(true) {

	precalc_values();
}

namespace {


double argument(double mx, double my, double xa, double ya, double xb, double yb) {
	double ma_x = xa - mx;
	double ma_y = ya - my;
	double mb_x = xb - mx;
	double mb_y = yb - my;

	double norm_ma = hypot(ma_x, ma_y);
	double norm_mb = hypot(mb_x, mb_y);

	double cos_theta = (ma_x*mb_x + ma_y*mb_y) / (norm_ma*norm_mb);
	double sin_theta = (ma_x*mb_y - ma_y*mb_x) / (norm_ma*norm_mb);

	return ::atan2(sin_theta,cos_theta); // remove "::" (should be intervalized)
}

} // end anonymous namespace



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

void PdcInPolygon::precalc_values() {

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

int PdcInPolygon::pnpoly(double x, double y) {

	int i, j, c = 0;
	int nvert = ay.size();
	for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if ( ((ay[i]>y) != (ay[j]>y)) &&
                (x < (ax[j]-ax[i]) * (y-ay[i]) / (ay[j]-ay[i]) + ax[i]) )
			c = !c;
	}
	return c;
}

//===================================================================
void PdcInPolygon::inv() {
	//    std::cout << inverse << std::endl;
	inverse = (inverse == true) ? false: true;
}

BoolInterval PdcInPolygon::test(const IntervalVector& x) {

	// TODO: *** should be intervalized and return MAYBE in some cases ***

	double mx = x[0].mid(); // no! should be intervalized
	double my = x[1].mid(); // no! should be intervalized

	//double cos_theta = 0;
	double theta = 0;
	for(uint i = 0; i < ax.size(); i++){
		theta += argument(mx,my,ax[i],ay[i],bx[i],by[i]);
	}

	if(theta > 1) return inverse? NO : YES;
	return inverse? YES : NO;
}

} // namespace ibex
