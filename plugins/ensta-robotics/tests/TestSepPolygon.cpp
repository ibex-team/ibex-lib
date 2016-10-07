//============================================================================
//                                  I B E X                                   
// File        : TestSepPolygon.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 02, 2015
//============================================================================

#include "TestSepPolygon.h"

using namespace std;

namespace ibex {

void TestSepPolygon::setup(){

    murs_xa.clear(); murs_xa2.clear();
    murs_xb.clear(); murs_xb2.clear();
    murs_ya.clear(); murs_ya2.clear();
    murs_yb.clear(); murs_yb2.clear();

    // Polygone convex
    murs_xa.push_back(6);  murs_ya.push_back(-6);   murs_xb.push_back(7);  murs_yb.push_back(9);
    murs_xa.push_back(7);  murs_ya.push_back(9);   murs_xb.push_back(0);  murs_yb.push_back(5);
    murs_xa.push_back(0);  murs_ya.push_back(5);    murs_xb.push_back(-9); murs_yb.push_back(8);
    murs_xa.push_back(-9); murs_ya.push_back(8);    murs_xb.push_back(-8); murs_yb.push_back(-9);
    murs_xa.push_back(-8); murs_ya.push_back(-9);   murs_xb.push_back(6);  murs_yb.push_back(-6);

    // Objet au milieu
    murs_xa2.push_back(-2); murs_ya2.push_back(3);   murs_xb2.push_back(3.5);  murs_yb2.push_back(2);
    murs_xa2.push_back(3.5); murs_ya2.push_back(2);   murs_xb2.push_back(3);  murs_yb2.push_back(-4);
    murs_xa2.push_back(3); murs_ya2.push_back(-4);   murs_xb2.push_back(-3);  murs_yb2.push_back(-3);
    murs_xa2.push_back(-3); murs_ya2.push_back(-3);   murs_xb2.push_back(-2);  murs_yb2.push_back(3);
}

void TestSepPolygon::test_SepPolygon_01(){

    SepPolygon S1(murs_xa, murs_ya, murs_xb, murs_yb);

    IntervalVector box(2);
    box[0]=Interval(-10,10);
    box[1]=Interval(-10,10);

}


void TestSepPolygon::test_SepPolygon_02(){
    
    murs_xa.insert(murs_xa.end(), murs_xa2.begin(), murs_xa2.end());
    murs_ya.insert(murs_ya.end(), murs_ya2.begin(), murs_ya2.end());
    murs_xb.insert(murs_xb.end(), murs_xb2.begin(), murs_xb2.end());
    murs_yb.insert(murs_yb.end(), murs_yb2.begin(), murs_yb2.end());

    SepPolygon S1(murs_xa, murs_ya, murs_xb, murs_yb);
}

void TestSepPolygon::test_SepPolygon_03(){

     SepPolygon S1(murs_xa, murs_ya, murs_xb, murs_yb);
     SepPolygon S2(murs_xa2, murs_ya2, murs_xb2, murs_yb2);
     SepNot S3(S2);
     SepInter S(S1, S3);

}

} // end namespace

