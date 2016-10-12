/* ============================================================================
 * I B E X - OptimMinMax Tests
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Oct 2, 2016
 * ---------------------------------------------------------------------------- */

#include "TestOptimMinMax.h"
#include "ibex_OptimMinMax.h"
#include "ibex_SystemFactory.h"
#include "ibex_CtcIdentity.h"

using namespace std;

namespace ibex {



//******** messine GOW conf*********
void TestOptimMinMax::ex_messine() {
        double x_prec = 1e-6;
        double y_prec = 1e-4;
        double stop_prec = 1e-3;

        Variable x(2),y(2);
        IntervalVector x_ini(2,Interval(-10,10));
        IntervalVector y_ini(2,Interval(-0.1,0.1));
        Function func(x,y,(pow((x[0]+y[0]-2),6)+0.2)*log(1+pow(x[0]+y[0],2)) + (pow((x[1]+y[1]-2),6)+0.2)*log(1+pow(x[1]+y[1],2)));

        SystemFactory x_fac;
        x_fac.add_var(x, x_ini);
        NormalizedSystem x_sys(x_fac);
        CtcIdentity x_ctc(x_ini.size());

        SystemFactory xy_fac;
        xy_fac.add_var(x, x_ini);
        xy_fac.add_var(y, y_ini);
        xy_fac.add_goal(func);
        NormalizedSystem xy_sys(xy_fac);
        CtcIdentity xy_ctc(x_ini.size()+y_ini.size());


        OptimMinMax oo(x_sys, xy_sys,x_ctc,xy_ctc,x_prec, y_prec,stop_prec);
        Optim::Status res = oo.optimize(x_ini);
        oo.trace =2;
        oo.report();

}
/*
//  *********** Problems from "A differential evolution approach for solving constrained min–max optimization problems",Segundo, 2012

void TestOptimMinMax::ex_segundo1() {

        Variable x(1),y(1);
        IntervalVector x_ini(1,Interval(-3.14,3.14));
        IntervalVector y_ini(1,Interval(-3.14,3.14));
        double x_prec(0.00001),y_prec(0.00001),stop_prec(0.01);
        Function func(x,y,pow(cos(y)+cos(2*y+x),2));

        SystemFactory fac_x,fac_y;
        fac_x.add_goal(func);


        Function c1f(x,y,y-x*(x+6.28) );
        NumConstraint c1(c1f,LEQ);
        Function c2f(x,y,y-x*(x-6.28));
        NumConstraint c2(c2f,LEQ);

        vector<Ctc*> array_ctc,array_ctc_inv;
        array_ctc.push_back(new CtcFwdBwd(NumConstraint(c1f,LEQ)));
        array_ctc.push_back(new CtcFwdBwd(NumConstraint(c2f,LEQ)));
        CtcCompo ctc_xy(array_ctc);

        fac_y.add_var(x);
        fac_y.add_var(y);
        fac_y.add_ctr(c1);
        fac_y.add_ctr(c2);

        NormalizedSystem sys_x(fac_x);
        NormalizedSystem sys_y(fac_y);

        minimax_solver solver(0,&ctc_xy,&sys_x,&sys_y);
        solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);
}

void TestOptimMinMax::ex_segundo2() {

        Variable x(1),y(1);
        IntervalVector x_ini(1,Interval(0,6));
        IntervalVector y_ini(1,Interval(2,8));
        double x_prec(0.001),y_prec(0.001),stop_prec(0.01);
        Function func(x,y,pow(x,2)+pow(y,2)+2*x*y-20*x-20*y+100);

        SystemFactory fac_x,fac_y;
        fac_x.add_goal(func);

        Function c1f(x,y,-pow(x-5,2)-pow(y-3,2)+4);
        NumConstraint c1(c1f,LEQ);
        Function c2f(x,y,pow(x-5,2)+pow(y-3,2)-16);
        NumConstraint c2(c2f,LEQ);

        vector<Ctc*> array_ctc;
        array_ctc.push_back(new CtcFwdBwd(NumConstraint(c1f,LEQ)));
        array_ctc.push_back(new CtcFwdBwd(NumConstraint(c2f,LEQ)));
        CtcCompo ctc_xy(array_ctc);

        fac_y.add_var(x);
        fac_y.add_var(y);
        fac_y.add_ctr(c1);
        fac_y.add_ctr(c2);

        NormalizedSystem sys_x(fac_x);
        NormalizedSystem sys_y(fac_y);

        minimax_solver solver(0,&ctc_xy,&sys_x,&sys_y);
        solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);

}

void TestOptimMinMax::ex_segundo3() {

    Variable x(1),y(1);
    IntervalVector x_ini(1,Interval(-5,5));
    IntervalVector y_ini(1,Interval(-5,5));
    double x_prec(0.0001),y_prec(0.0001),stop_prec(0.01);
    Function func(x,y,pow(sin(x),2)-x*cos(y)+2*sin(x)-pow(cos(y),2)+y-1 );

    SystemFactory fac_x,fac_y;
    fac_x.add_goal(func);
    IntervalVector box(2);
    box[0] = (-0.499064, -0.499064);
    box[1] = (4.9, 5); //-1.63713, -1.63644

    Function c1f(x,y,pow(x,2)+pow(y,2)-25);
    NumConstraint c1(c1f,GEQ);
    CtcFwdBwd ctc_xy(c1);

    fac_y.add_var(x);
    fac_y.add_var(y);
    fac_y.add_ctr(c1);

    NormalizedSystem sys_x(fac_x);
    NormalizedSystem sys_y(fac_y);

    minimax_solver solver(0,&ctc_xy,&sys_x,&sys_y);
    solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);
}

void TestOptimMinMax::ex_segundo4() {

        Variable x(2),y(2);
        IntervalVector x_ini(2,Interval(0,10));
        IntervalVector y_ini(2,Interval(0,10));
        double x_prec(0.1),y_prec(0.1),stop_prec(0.01);
        Function func(x,y, -((x[0]+y[0])*(x[1]+y[1]))/(1+x[0]*y[0]+x[1]*y[1]) );

        SystemFactory fac_x,fac_y;
        fac_x.add_goal(func);

        Function c1f(x,y,pow(x[0],2)+pow(x[1],2)-100);
        Function c2f(x,y,y[0]-x[0]);
        Function c3f(x,y,y[1]-x[1]);
        NumConstraint c1(c1f,LEQ);
        NumConstraint c2(c2f,LEQ);
        NumConstraint c3(c3f,LEQ);

        vector<Ctc*> array_ctc;
        array_ctc.push_back(new CtcFwdBwd(c1));
        array_ctc.push_back(new CtcFwdBwd(c2));
        array_ctc.push_back(new CtcFwdBwd(c3));
        CtcCompo ctc_xy(array_ctc);

        fac_y.add_var(x);
        fac_y.add_var(y);
        fac_y.add_ctr(c1);
        fac_y.add_ctr(c2);
        fac_y.add_ctr(c3);

        NormalizedSystem sys_x(fac_x);
        NormalizedSystem sys_y(fac_y);

        minimax_solver solver(0,&ctc_xy,&sys_x,&sys_y);
        solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);
}

//  *********** CDC submission example ***************

void TestOptimMinMax::ex_cdc() {
    Variable x(3),y(1),kp,ki,kd,u;
    IntervalVector x_ini(3,Interval(-10,10));
    IntervalVector y_ini(1,Interval(-3,3));
    double x_prec(0.0001),y_prec(0.0001),stop_prec(0.1);

    Function Twz1("functions/cdc_ex/Twz1.txt");
    Function Twz2("functions/cdc_ex/Twz2.txt");
    Function Twz3("functions/cdc_ex/Twz3.txt");

    Function max12(x,y,ibex::max(Twz1(x[0],x[1],x[2],y),Twz2(x[0],x[1],x[2],y)));
    Function goal(x,y,ibex::max(max12(x,y),Twz3(x[0],x[1],x[2],y)));

    SystemFactory fac_x;
    fac_x.add_var(x);
    fac_x.add_goal(goal);

    Function c1f(x,1.9833333333333333333+(0.58333333333333333337)*x[0]+x[2]-(0.41666666666666666666)*x[1]);
    Function c2f(x,(1.9833333333333333333+(2.5666666666666666666)*x[0]+x[2]*x[1]+(0.58333333333333333337)*pow(x[0],2)+x[2]-(0.41666666666666666666)*pow(x[1],2)-(0.83333333333333333337)*x[1]+x[0]*x[2]+(0.16666666666666666671)*x[0]*x[1])*1/(1.9833333333333333333+(0.58333333333333333337)*x[0]+x[2]-(0.41666666666666666666)*x[1]));
    Function c3f(x,x[1]);
    NumConstraint c1(c1f,GEQ);
    NumConstraint c2(c2f,GEQ);
    NumConstraint c3(c3f,GEQ);

    vector<Ctc*> array_ctc;
    array_ctc.push_back(new CtcFwdBwd(c1));
    array_ctc.push_back(new CtcFwdBwd(c2));
    array_ctc.push_back(new CtcFwdBwd(c3));
    CtcCompo ctc_x(array_ctc);


    fac_x.add_ctr(c1);
    fac_x.add_ctr(c2);
    fac_x.add_ctr(c3);

    NormalizedSystem sys_x(fac_x);

    minimax_solver solver(&ctc_x,0,&sys_x,0);
    solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);


}

//  *********** Ciscrea directly linearized example ***************

void TestOptimMinMax::ex_ciscrea() {
    Variable x(4),y(1);
    IntervalVector x_ini(4,Interval(0,3));
    IntervalVector y_ini(1,Interval(-3,3));
    double x_prec(0.01),y_prec(0.01),stop_prec(0.1);

    Function Twz1("functions/ciscrea_ex/Twz1.txt");
    Function Twz2("functions/ciscrea_ex/Twz2.txt");
    Function Twz3("functions/ciscrea_ex/Twz3.txt");

    Function max12(x,y,ibex::max(Twz1(x[0],x[1],x[2],x[3],y),Twz2(x[0],x[1],x[2],x[3],y)));
    Function goal(x,y,ibex::max(max12(x,y),Twz3(x[0],x[1],x[2],x[3],y)));

    SystemFactory fac_x;
    fac_x.add_var(x);
    fac_x.add_goal(goal);

    Function c1f(x,(0.4958)*x[3]);
    Function c2f(x,0.4958+(0.54537)*x[3]);
    Function c3f(x,(0.270394446-(0.4958)*x[1]*pow(x[3],2)+(0.54537)*x[2]*x[3]+(0.4958)*x[2]+(0.54537)*x[0]*pow(x[3],2)+(0.29742843690000000001)*x[3])*1/(0.4958+(0.54537)*x[3]));
    Function c4f(x,1/(0.270394446-(0.4958)*x[1]*pow(x[3],2)+(0.54537)*x[2]*x[3]+(0.4958)*x[2]+(0.54537)*x[0]*pow(x[3],2)+(0.29742843690000000001)*x[3])*((0.270394446)*x[0]+(0.4958)*x[0]*x[2]+(0.4958)*x[2]*x[1]*x[3]+(0.54537)*pow(x[0],2)*pow(x[3],2)+(0.54537)*x[2]*x[1]*pow(x[3],2)-(0.270394446)*x[1]*x[3]+(0.54537)*x[0]*x[1]*pow(x[3],3)-(0.24581763999999999999)*x[1]+(0.54537)*x[0]*x[2]*x[3]-(0.4958)*x[0]*x[1]*pow(x[3],2)+(0.29742843690000000001)*x[0]*x[3]-(0.4958)*pow(x[1],2)*pow(x[3],3)));
    Function c5f(x,x[1]);
    NumConstraint c1(c1f,GEQ);
    NumConstraint c2(c2f,GEQ);
    NumConstraint c3(c3f,GEQ);
    NumConstraint c4(c4f,GEQ);
    NumConstraint c5(c5f,GEQ);

    vector<Ctc*> array_ctc;
    array_ctc.push_back(new CtcFwdBwd(c1));
    array_ctc.push_back(new CtcFwdBwd(c2));
    array_ctc.push_back(new CtcFwdBwd(c3));
    array_ctc.push_back(new CtcFwdBwd(c4));
    array_ctc.push_back(new CtcFwdBwd(c5));
    CtcCompo ctc_x(array_ctc);


    fac_x.add_ctr(c1);
    fac_x.add_ctr(c2);
    fac_x.add_ctr(c3);
    fac_x.add_ctr(c4);
    fac_x.add_ctr(c5);

    NormalizedSystem sys_x(fac_x);

    minimax_solver solver(&ctc_x,0,&sys_x,0);
    solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);
}

//  *********** Ciscrea robust direct linearized example ***************

void TestOptimMinMax::ex_ciscrea_robust() {
    Variable x(4),y(2);
    IntervalVector x_ini(4,Interval(0,3));
    IntervalVector y_ini(2,Interval(-3,3));
    y_ini[1] = Interval(0.5,2);
    double x_prec(0.01),y_prec(0.01),stop_prec(0.1);

    Function Twz1("functions/ciscrea_robust_ex/Twz1_rob.txt");
    Function Twz2("functions/ciscrea_robust_ex/Twz2_rob.txt");
    Function Twz3("functions/ciscrea_robust_ex/Twz3_rob.txt");

    cout<<"functions loaded"<<endl;

    Function max12(x,y,ibex::max(Twz1(x[0],x[1],x[2],x[3],y[0],y[1]),Twz2(x[0],x[1],x[2],x[3],y[0],y[1])));
    Function goal(x,y,ibex::max(max12(x,y),Twz3(x[0],x[1],x[2],x[3],y[0],y[1])));
    cout<<"goal ok"<<endl;

    SystemFactory fac_x,fac_y;
    fac_x.add_var(x);
    fac_x.add_goal(goal);


    Function routh("functions/ciscrea_robust_ex/routh.txt");


    Function c1f(x,y,routh(x[0],x[1],x[2],x[3],y[0])[0]);
    Function c2f(x,y,routh(x[0],x[1],x[2],x[3],y[0])[1]);
    Function c3f(x,y,routh(x[0],x[1],x[2],x[3],y[0])[2]);
    Function c4f(x,y,routh(x[0],x[1],x[2],x[3],y[0])[3]);
    Function c5f(x,y,routh(x[0],x[1],x[2],x[3],y[0])[4]);
    NumConstraint c1(c1f,GEQ);
    NumConstraint c2(c2f,GEQ);
    NumConstraint c3(c3f,GEQ);
    NumConstraint c4(c4f,GEQ);
    NumConstraint c5(c5f,GEQ);

    vector<Ctc*> array_ctc;
    array_ctc.push_back(new CtcFwdBwd(c1));
    array_ctc.push_back(new CtcFwdBwd(c2));
    array_ctc.push_back(new CtcFwdBwd(c3));
    array_ctc.push_back(new CtcFwdBwd(c4));
    array_ctc.push_back(new CtcFwdBwd(c5));
    CtcCompo ctc_xy(array_ctc);

    fac_y.add_var(x);
    fac_y.add_var(y);
    fac_y.add_ctr(c1);
    fac_y.add_ctr(c2);
    fac_y.add_ctr(c3);
    fac_y.add_ctr(c4);
    fac_y.add_ctr(c5);


    NormalizedSystem sys_x(fac_x),sys_y(fac_y);

    minimax_solver solver(0,&ctc_xy,&sys_x,&sys_y);
    solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);
}

//  *********** Hirokazu symbolic example ***************

void TestOptimMinMax::ex_hirokazu() {
    Variable x(2),y(1);
    IntervalVector x_ini(2,Interval(-10,10));
    IntervalVector y_ini(1,Interval(-10,0));
    double x_prec(0.001),y_prec(0.0001),stop_prec(0.01);

    Function Twz1("functions/Hirokazu_ex/Twz1.txt");

    Function goal(x,y,ibex::sqrt(Twz1(x[0],x[1],y)));

    SystemFactory fac_x;
    fac_x.add_var(x);
    fac_x.add_goal(goal);

    Function routh("functions/Hirokazu_ex/routh.txt");

    Function c1f(x,routh(x[0],x[1])[2]);
    Function c2f(x,routh(x[0],x[1])[3]);
    NumConstraint c1(c1f,GEQ);
    NumConstraint c2(c2f,GEQ);

    vector<Ctc*> array_ctc;
    array_ctc.push_back(new CtcFwdBwd(c1));
    array_ctc.push_back(new CtcFwdBwd(c2));
    CtcCompo ctc_x(array_ctc);


    fac_x.add_ctr(c1);
    fac_x.add_ctr(c2);

    NormalizedSystem sys_x(fac_x);

    minimax_solver solver(&ctc_x,0,&sys_x,0);
    solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);
}

//  *********** Fujisaki symbolic example ***************

void TestOptimMinMax::ex_fujisaki() {
    Variable x(6),y(1);
    IntervalVector x_ini(6,Interval(-5,5));
    IntervalVector y_ini(1,Interval(-3,3));
    double x_prec(0.001),y_prec(0.0001),stop_prec(0.01);

    Function Twz1("functions/Fujisaki_ex/Twz1.txt");

    Function goal(x,y,ibex::sqrt(Twz1(x[0],x[1],x[2],x[3],x[4],x[5],y)));

    SystemFactory fac_x;
    fac_x.add_var(x);
    fac_x.add_goal(goal);

    Function routh("functions/Fujisaki_ex/routh.txt");

    Function c1f(x,routh(x[0],x[1],x[2],x[3],x[4],x[5])[0]);
    Function c2f(x,routh(x[0],x[1],x[2],x[3],x[4],x[5])[1]);
    Function c3f(x,routh(x[0],x[1],x[2],x[3],x[4],x[5])[2]);
    Function c4f(x,routh(x[0],x[1],x[2],x[3],x[4],x[5])[3]);
    Function c5f(x,routh(x[0],x[1],x[2],x[3],x[4],x[5])[4]);
    Function c6f(x,routh(x[0],x[1],x[2],x[3],x[4],x[5])[5]);
    NumConstraint c1(c1f,GEQ);
    NumConstraint c2(c2f,GEQ);
    NumConstraint c3(c3f,GEQ);
    NumConstraint c4(c4f,GEQ);
    NumConstraint c5(c5f,GEQ);
    NumConstraint c6(c6f,GEQ);

    vector<Ctc*> array_ctc;
    array_ctc.push_back(new CtcFwdBwd(c1));
    array_ctc.push_back(new CtcFwdBwd(c2));
    array_ctc.push_back(new CtcFwdBwd(c3));
    array_ctc.push_back(new CtcFwdBwd(c4));
    array_ctc.push_back(new CtcFwdBwd(c5));
    array_ctc.push_back(new CtcFwdBwd(c6));
    CtcCompo ctc_x(array_ctc);


    fac_x.add_ctr(c1);
    fac_x.add_ctr(c2);
    fac_x.add_ctr(c3);
    fac_x.add_ctr(c4);
    fac_x.add_ctr(c5);
    fac_x.add_ctr(c6);

    NormalizedSystem sys_x(fac_x);

    minimax_solver solver(&ctc_x,0,&sys_x,0);
    solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);
}

//  *********** satellite pid filt example ***************


void TestOptimMinMax::ex_satellite() {
    Variable x(4),y(1);
    IntervalVector x_ini(4,Interval(-10,10));
    x_ini[3] = Interval(0,5);
    IntervalVector y_ini(1,Interval(-3,3));
    double x_prec(0.01),y_prec(0.01),stop_prec(0.01);

    Function Tw1z1("functions/satellite_ex/Tw1z1.txt");
    Function Tw1z2("functions/satellite_ex/Tw1z2.txt");
    Function Tw2z1("functions/satellite_ex/Tw2z1.txt");
    Function Tw2z2("functions/satellite_ex/Tw2z2.txt");

    Function Twz1(x,y,Tw1z1(x[0],x[1],x[2],x[3],y)+Tw2z1(x[0],x[1],x[2],x[3],y));
    Function Twz2(x,y,Tw1z2(x[0],x[1],x[2],x[3],y)+Tw2z2(x[0],x[1],x[2],x[3],y));

    Function goal(x,y,ibex::max(Twz1(x,y),Twz2(x,y)));

    SystemFactory fac_x;
    fac_x.add_var(x);
    fac_x.add_goal(goal);

    Function routh("functions/satellite_ex/routh.txt");

    Function c1f(x,routh(x[0],x[1],x[2],x[3])[0]);
    Function c2f(x,routh(x[0],x[1],x[2],x[3])[1]);
    Function c3f(x,routh(x[0],x[1],x[2],x[3])[2]);
    Function c4f(x,routh(x[0],x[1],x[2],x[3])[3]);
    Function c5f(x,routh(x[0],x[1],x[2],x[3])[4]);
    Function c6f(x,routh(x[0],x[1],x[2],x[3])[5]);
    Function c7f(x,routh(x[0],x[1],x[2],x[3])[6]);
    NumConstraint c1(c1f,GEQ);
    NumConstraint c2(c2f,GEQ);
    NumConstraint c3(c3f,GEQ);
    NumConstraint c4(c4f,GEQ);
    NumConstraint c5(c5f,GEQ);
    NumConstraint c6(c6f,GEQ);
    NumConstraint c7(c7f,GEQ);

    vector<Ctc*> array_ctc;
    array_ctc.push_back(new CtcFwdBwd(c1));
    array_ctc.push_back(new CtcFwdBwd(c2));
    array_ctc.push_back(new CtcFwdBwd(c3));
    array_ctc.push_back(new CtcFwdBwd(c4));
    array_ctc.push_back(new CtcFwdBwd(c5));
    array_ctc.push_back(new CtcFwdBwd(c6));
    array_ctc.push_back(new CtcFwdBwd(c7));
    CtcCompo ctc_x(array_ctc);


    fac_x.add_ctr(c1);
    fac_x.add_ctr(c2);
    fac_x.add_ctr(c3);
    fac_x.add_ctr(c4);
    fac_x.add_ctr(c5);
    fac_x.add_ctr(c6);
    fac_x.add_ctr(c7);

    NormalizedSystem sys_x(fac_x);

    minimax_solver solver(&ctc_x,0,&sys_x,0);
    solver.solve(x_ini,y_ini,x_prec,y_prec,stop_prec);
}

*/

} // end namespace
