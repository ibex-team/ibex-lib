//============================================================================
//                                  I B E X                                   
// File        : Ponts30.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 10, 2012
// Last Update : Apr 10, 2012
//============================================================================

#include "Ponts30.h"

namespace ibex {


void Ponts30::build_box() {

	int i=0;

	int A_x,A_y; A_x= i++; A_y= i++;
	int B_x,B_y; B_x= i++; B_y= i++;
	int C_x,C_y; C_x= i++; C_y= i++;
	int D_x,D_y; D_x= i++; D_y= i++;
	int E_x,E_y; E_x= i++; E_y= i++;
	int F_x,F_y; F_x= i++; F_y= i++;
	int G_x,G_y; G_x= i++; G_y= i++;
	int H_x,H_y; H_x= i++; H_y= i++;
	int I_x,I_y; I_x= i++; I_y= i++;
	int J_x,J_y; J_x= i++; J_y= i++;
	int K_x,K_y; K_x= i++; K_y= i++;
	int L_x,L_y; L_x= i++; L_y= i++;
	int M_x,M_y; M_x= i++; M_y= i++;
	int N_x,N_y; N_x= i++; N_y= i++;
	int O_x,O_y; O_x= i++; O_y= i++;

	init_box[O_y]=Interval(-46.516999999999996, 53.483000000000004);
	init_box[O_x]=Interval(-53.574999999999996, 46.425000000000004);
	init_box[N_y]=Interval(-46.759999999999998, 53.240000000000002);
	init_box[N_x]=Interval(-53.517999999999994, 46.482000000000006);
	init_box[M_y]=Interval(-46.589999999999996, 53.410000000000004);
	init_box[M_x]=Interval(-53.335999999999999, 46.664000000000001);
	init_box[L_y]=Interval(-46.602999999999994, 53.397000000000006);
	init_box[L_x]=Interval(-53.085999999999999, 46.914000000000001);
	init_box[K_y]=Interval(-46.832999999999998, 53.167000000000002);
	init_box[K_x]=Interval(-53.278999999999996, 46.721000000000004);
	init_box[J_y]=Interval(-47.003999999999998, 52.996000000000002);
	init_box[J_x]=Interval(-53.461999999999996, 46.538000000000004);
	init_box[I_y]=Interval(-49.599999999999994, 50.400000000000006);
	init_box[I_x]=Interval(-51.958999999999996, 48.041000000000004);
	init_box[H_y]=Interval(-47.596999999999994, 52.403000000000006);
	init_box[H_x]=Interval(-48.497999999999998, 51.502000000000002);
	init_box[G_y]=Interval(-46.552999999999997, 53.447000000000003);
	init_box[G_x]=Interval(-52.840999999999994, 47.159000000000006);
	init_box[F_y]=Interval(-46.789999999999999, 53.210000000000001);
	init_box[F_x]=Interval(-52.919999999999995, 47.080000000000005);
	init_box[E_y]=Interval(-46.976999999999997, 53.023000000000003);
	init_box[E_x]=Interval(-52.753999999999998, 47.246000000000002);
	init_box[D_y]=Interval(-46.739999999999995, 53.260000000000005);
	init_box[D_x]=Interval(-52.674999999999997, 47.325000000000003);
	init_box[C_y]=Interval(-46.503, 53.497);
	init_box[C_x]=Interval(-52.595999999999997, 47.404000000000003);
	init_box[B_y]=Interval(0,55);
	init_box[B_x]=Interval(-50,50);
	init_box[A_y]=Interval(-50,50);
	init_box[A_x]=Interval(-50,50);

}

void Ponts30::build_equ() {

	const ExprSymbol& A_x=f.add_symbol("A_x");
	const ExprSymbol& A_y=f.add_symbol("A_y");
	const ExprSymbol& B_x=f.add_symbol("B_x");
	const ExprSymbol& B_y=f.add_symbol("B_y");
	const ExprSymbol& C_x=f.add_symbol("C_x");
	const ExprSymbol& C_y=f.add_symbol("C_y");
	const ExprSymbol& D_x=f.add_symbol("D_x");
	const ExprSymbol& D_y=f.add_symbol("D_y");
	const ExprSymbol& E_x=f.add_symbol("E_x");
	const ExprSymbol& E_y=f.add_symbol("E_y");
	const ExprSymbol& F_x=f.add_symbol("F_x");
	const ExprSymbol& F_y=f.add_symbol("F_y");
	const ExprSymbol& G_x=f.add_symbol("G_x");
	const ExprSymbol& G_y=f.add_symbol("G_y");
	const ExprSymbol& H_x=f.add_symbol("H_x");
	const ExprSymbol& H_y=f.add_symbol("H_y");
	const ExprSymbol& I_x=f.add_symbol("I_x");
	const ExprSymbol& I_y=f.add_symbol("I_y");
	const ExprSymbol& J_x=f.add_symbol("J_x");
	const ExprSymbol& J_y=f.add_symbol("J_y");
	const ExprSymbol& K_x=f.add_symbol("K_x");
	const ExprSymbol& K_y=f.add_symbol("K_y");
	const ExprSymbol& L_x=f.add_symbol("L_x");
	const ExprSymbol& L_y=f.add_symbol("L_y");
	const ExprSymbol& M_x=f.add_symbol("M_x");
	const ExprSymbol& M_y=f.add_symbol("M_y");
	const ExprSymbol& N_x=f.add_symbol("N_x");
	const ExprSymbol& N_y=f.add_symbol("N_y");
	const ExprSymbol& O_x=f.add_symbol("O_x");
	const ExprSymbol& O_y=f.add_symbol("O_y");

	const ExprNode* equ[30];
	int i=0;

	equ[i++]=&(	sqr((N_x - O_x) + sqr(N_y - O_y)) - 0.0625 );
	equ[i++]=&(	sqr((M_x - O_x) + sqr(M_y - O_y)) - 0.0625 );
	equ[i++]=&(	sqr((M_x - N_x) + sqr(M_y - N_y)) - 0.0625 );
	equ[i++]=&(	sqr((J_x - N_x) + sqr(J_y - N_y)) - 0.0625 );
	equ[i++]=&(	sqr((L_x - M_x) + sqr(L_y - M_y)) - 0.0625 );
	equ[i++]=&(	sqr((K_x - M_x) + sqr(K_y - M_y)) - 0.0625 );
	equ[i++]=&(	sqr((G_x - L_x) + sqr(G_y - L_y)) - 0.0625 );
	equ[i++]=&(	sqr((K_x - L_x) + sqr(K_y - L_y)) - 0.089999999999999997 );
	equ[i++]=&(	sqr((J_x - K_x) + sqr(J_y - K_y)) - 0.0625 );
	equ[i++]=&(	sqr((K_x - N_x) + sqr(K_y - N_y)) - 0.0625 );
	equ[i++]=&(	sqr((I_x - J_x) + sqr(I_y - J_y)) - 9 );
	equ[i++]=&(	sqr((H_x - J_x) + sqr(H_y - J_y)) - 25 );
	equ[i++]=&(	sqr((B_x - I_x) + sqr(B_y - I_y)) - 25 );
	equ[i++]=&(	sqr((A_x - I_x) + sqr(A_y - I_y)) - 4 );
	equ[i++]=&(	sqr((B_x - H_x) + sqr(B_y - H_y)) - 9 );
	equ[i++]=&(	sqr((H_x - I_x) + sqr(H_y - I_y)) - 16 );
	equ[i++]=&(	sqr((F_x - G_x) + sqr(F_y - G_y)) - 0.0625 );
	equ[i++]=&(	sqr((C_x - G_x) + sqr(C_y - G_y)) - 0.0625 );
	equ[i++]=&(	sqr((F_x - L_x) + sqr(F_y - L_y)) - 0.0625 );
	equ[i++]=&(	sqr((D_x - F_x) + sqr(D_y - F_y)) - 0.0625 );
	equ[i++]=&(	sqr((D_x - E_x) + sqr(D_y - E_y)) - 0.0625 );
	equ[i++]=&(	sqr((E_x - F_x) + sqr(E_y - F_y)) - 0.0625 );
	equ[i++]=&(	sqr((D_x - G_x) + sqr(D_y - G_y)) - 0.0625 );
	equ[i++]=&(	sqr((C_x - D_x) + sqr(C_y - D_y)) - 0.0625 );
	equ[i++]=&(	sqr((C_x - H_x) + sqr(C_y - H_y)) - 18.003049000000004 );
	equ[i++]=&(	sqr((B_x - C_x) + sqr(B_y - C_y)) - 9 );
	equ[i++]=&(	sqr((A_x - B_x) + sqr(A_y - B_y)) - 25 );
	equ[i++]=&(	B_x - 0 );
	equ[i++]=&(	A_y - 0 );
	equ[i++]=&(	A_x - 0 );

	f.set_expr(ExprVector::new_(equ,30,false));

}

Ponts30::Ponts30() : init_box(30) {
	build_equ();
	build_box();
}


} // end namespace ibex
