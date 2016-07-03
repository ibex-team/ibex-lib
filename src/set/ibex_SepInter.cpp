//============================================================================
//                                  I B E X
// File        : Intersection of separators
// Author      : Benoit Desrochers
// Copyright   : ENSTA Bretagne (France)
// License     : See the LICENSE file
// Created     : Mar 25, 2014
// Last Update : Mar 25, 2014
//============================================================================

#include "ibex_SepInter.h"
#include <cassert>

namespace ibex {

SepInter::SepInter(const Array<Sep>& list) : Sep(list[0].nb_var), list(list) {
}

SepInter::SepInter(Sep& s1, Sep& s2) : Sep(s1.nb_var), list(Array<Sep>(s1,s2)) {

}

SepInter::SepInter(Sep& s1, Sep& s2, Sep& s3) : Sep(s1.nb_var), list(Array<Sep>(s1,s2,s3)) {

}

SepInter::SepInter(Sep &s1, Sep &s2, Sep &s3, Sep &s4) : Sep(s1.nb_var), list(Array<Sep>(s1,s2,s3,s4))
{

}

//void SepInter::separate(IntervalVector &Xin, IntervalVector &Xout){

//    std::vector<IntervalVector> LXin(2, IntervalVector(2)), LXout(2,IntervalVector(2));

//    for (int i=0; i<list.size(); i++) {
//        LXin[i] = Xin; LXout[i] = Xout;
//        try{
//            list[i].separate(LXin[i],LXout[i]);
//        }catch(EmptyBoxException&){}
//    }

//    Xin = IntervalVector::empty(Xin.size());
//    Xout = IntervalVector(Xout.size(),Interval::ALL_REALS);
//    for (int i=0; i<list.size(); i++){
//        Xin |= LXin[i];
//        Xout &= LXout[i];
//    }

//}

void SepInter::separate(IntervalVector &x_in, IntervalVector &x_out){
	assert(x_in==x_out);
    IntervalVector result_x_in(IntervalVector::empty(x_out.size()));
    IntervalVector savebox(x_in);

    for (int i=0; i<list.size(); i++) {
    	if (i>0) x_in=savebox;
    	x_in &= x_out;
    	list[i].separate(x_in,x_out);
    	result_x_in |= x_in;
    }
    x_in = result_x_in;

}

} // end namespace ibex
