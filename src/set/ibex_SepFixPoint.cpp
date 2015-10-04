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


SepFixPoint::SepFixPoint(Sep& sep, double ratio) : 
    Sep(sep.nb_var), 
    sep(sep),
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



void SepFixPoint::setCinFlags(IntervalVector& x_in, IntervalVector& x0){
    if( !impact_cin ){
        if ( ! (x_in == x0)){
            impact_cin = true;
            n_in = x0.diff(x_in,first_cin_boxes); // calculate the set difference
        }
    }
}

void SepFixPoint::setCoutFlags(IntervalVector& x_out, IntervalVector& x0){
    if(!impact_cout){
        if ( !(x_out == x0)){
            impact_cout = true;
            n_out = x0.diff(x_out,first_cout_boxes); // calculate the set difference
        }
    }
}

/**
 * To reconstruct x_in and x_out after two situations can appear
 *
 * SITUATION 1
 * Only and inner (or outer) contraction happens.
 * In this case x_in = x and x_out = x0
 *
 *  ++++++++++++++++++++++++++++++++++++
 * |                   |               |
 * |         x         |    x_in       |
 * |                   |               |
 * |-----------------------------------|
 * |             x_in                  |
 * |                                   |
 *  ++++++++++++++++++++++++++++++++++++
 *
 * SITUATION 2
 * Both inner and outer contraction happen
 * in this case, n_out = n_in = 1
 *
 *  +++++++++++++++++++++++++++++++++++
 * |            |           |          |
 * |     x_in   |    x      |  x_out   |
 * |            |           |          |
 *  +++++++++++++++++++++++++++++++++++
 *
 */
bool SepFixPoint::reconstrut(IntervalVector &x_in, IntervalVector& x_out, IntervalVector& x_old){
    IntervalVector x = x_in & x_out;
    IntervalVector *rest;

    if (x == x_old) return true; // no contraction
    if (impact_cin == true && impact_cout == false){
        // Only an inner contraction
        x_out = x_old;
        x_in = x;
    } else if (impact_cin == false && impact_cout == true){
        // Only an outer contraction
        x_in = x_old;
        x_out = x;
    } else if (impact_cin == true && impact_cout == true){

        // Inner and outer contraction.
        // We are in the situation 2.
        // make the difference if x_old \ x
        // the number of boxes of the difference must be 2
        // and n_in + n_out == n

        int n = x_old.diff(x, rest);
        assert(n_in  == 1);
        assert(n_out == 1);
        assert(n     == 2);
        // iterate over boxes in rest and affect them in x_in or x_out
        for (int i = 0; i < n; i++){
            for(int j = 0; j < n_in; j++){
                // identify the box in rest which was removed by cin
                if(first_cin_boxes[j].intersects(rest[i])){
                    x_out |= rest[i];
                    break;
                }
            }
            for(int j = 0; j < n_out; j++){
                // identify the box in rest which wa removed by cout
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
        setCinFlags(x_in, x_old);   // check if x_in  has been contracted
        setCoutFlags(x_out, x_old); // check if x_out has been contracted
        x = x_in & x_out;
        if ( x.is_empty() /*|| x.max_diam() < prec*/)
            break;
    } while (( x_old.rel_distance(x_in)>ratio || x_old.rel_distance(x_out)>ratio ));
    x_in = x;
    x_out = x;
    reconstrut(x_in, x_out, x_old0);
}

} // namespace ibex

