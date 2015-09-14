//============================================================================
//                                  I B E X                                   
// File        : ibex_SepFixPoint.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================
#include "ibex_SepFixPoint.h"

using namespace std;

namespace ibex {

/*! Default composition ratio. */
const double SepFixPoint::default_ratio = 0.1;


SepFixPoint::SepFixPoint(Sep& sep, double ratio) : sep(sep),
    ratio(ratio), impact_cin(false), impact_cout(false),
    first_cin_boxes(NULL), first_cout_boxes(NULL)
{
    clearFlags();
}

SepFixPoint::~SepFixPoint(){

}

void SepFixPoint::clearFlags(){
    n_in = 0;
    n_out = 0;
    if (first_cin_boxes != NULL) delete[] first_cin_boxes;
    if (first_cout_boxes != NULL) delete[] first_cout_boxes;
    first_cin_boxes = NULL;
    first_cout_boxes = NULL;
    impact_cin = false;
    impact_cout = false;
}



bool SepFixPoint::setCinFlags(IntervalVector& x_in, IntervalVector& x0){
    if( !impact_cin ){
        if ( ! (x_in == x0)){
            impact_cin = true;
            n_in = x0.diff(x_in,first_cin_boxes); // calculate the set difference
        }
    }
}

bool SepFixPoint::setCoutFlags(IntervalVector& x_out, IntervalVector& x0){
    if(!impact_cout){
        if ( !(x_out == x0)){
            impact_cout = true;
            n_out = x0.diff(x_out,first_cout_boxes); // calculate the set difference
        }
    }
}

/**
 * @brief SepFixPoint::reconstrut
 * @param x_in
 * @param x_out
 * @param x_old
 * @return
 */
bool SepFixPoint::reconstrut(IntervalVector &x_in, IntervalVector& x_out, IntervalVector& x_old){
    IntervalVector x = x_in & x_out;
    IntervalVector *rest;

    if (x == x_old) return true;
    if (impact_cin == true && impact_cout == false){
        x_out = x_old;
        x_in = x;
    } else if (impact_cin == false && impact_cout == true){
        x_in = x_old;
        x_out = x;
    } else if (impact_cin == true && impact_cout == true){
        int n = x_old.diff(x, rest);
        assert(n_in  == 1);
        assert(n_out == 1);
        assert(n     == 2);
        for (int i = 0; i < n; i++){
            for(int j = 0; j < n_in; j++){
                if(first_cin_boxes[j].intersects(rest[i])){
                    x_out |= rest[i];
                    break;
                }
            }
            for(int j = 0; j < n_out; j++){
                if(first_cout_boxes[j].intersects(rest[i])){
                    x_in |= rest[i];
                    break;
                }
            }
        }
        delete[] rest;
        return false;
    } else {
        assert(false);
    }
    return true;
}


void SepFixPoint::separate(IntervalVector& x_in, IntervalVector& x_out){
    
    IntervalVector x = x_in & x_out;
    IntervalVector x_old0(x); // Initial boxes
    IntervalVector x_old(x); // tmporary box use during the fix point.
    clearFlags(); // reset flags
    do {
        x_old = x;
        sep.separate(x_in, x_out);
        x = x_in & x_out;
        if ( x.is_empty() /*|| x.max_diam() < prec*/)
            break;
    } while (( x_old.rel_distance(x_in)>ratio || x_old.rel_distance(x_out)>ratio ));
    x_in = x;
    x_out = x;
    reconstrut(x_in, x_out, x_old0);
}

} // namespace ibex

