//============================================================================
//                                  I B E X
// File        : exo04.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================


#include "ibex.h"
#include "vibes.h"

using namespace std;
using namespace ibex;



int main() {
	{
		std::string figureName = "SIVIA_0";
		vibes::beginDrawing();           // <== Initializes the VIBES "connection"
		vibes::newFigure(figureName);       // <== Create a new VIBes figure

		double epsilon = 1.e-8;
		double gaol_prec= 1.e-6;
		double epsilon_time = 1.e-6;
		Interval init_time(0,1);
		// number of plane
		int n=4;
		
		// initial position and speed
		Matrix x_0(n,2); // initial position of each plane
		Matrix d_0(n,2); // initial speed vector of each plane

		x_0[0][0] = 0;
		x_0[0][1] = 0;
		x_0[1][0] = 1;
		x_0[1][1] = 0;
		x_0[2][0] = 0;
		x_0[2][1] = 1;
		x_0[3][0] = 1;
		x_0[3][1] = 1;

		d_0[0][0] = 1;
		d_0[0][1] = 1;
		d_0[1][0] = -1;
		d_0[1][1] = 1;
		d_0[2][0] = 1.3;
		d_0[2][1] = -1;
		d_0[3][0] = -1;
		d_0[3][1] = -1.3;

		
		// Symbolic part
		Variable t,v, d(2), x(2), V(n);
		
		Function traj_avion(v,t,d,x, (x+(1+v)*t*d));
				                       
		Function norm_2 (x, sqrt(sqr(x[0]) +sqr(x[1])));


		// we want the distance between the robots stay larger than 2.
		double secu =0.01;
		vector<NumConstraint*> array_out;
		for (int i=0; i<n; i++) {
			for(int j=i+1; j<n; j++) {
				array_out.push_back(new NumConstraint(V,t,  norm_2(traj_avion(V[i],t, d_0[i],x_0[i]) -traj_avion(V[j],t, d_0[j],x_0[j]))>=secu));
			}
		}

		// CtcHC4 = (CtcCompo + CtcFixPoint)
		CtcHC4 outside1(array_out);
		
		// Create a mask to indicate which variable are projected
		BitSet mask(BitSet::all(n+1));
		mask.remove(n);  // the last variable "t" will be projected
		// The initial domain of "t" is [0,1]
		CtcForAll outside(outside1,mask,IntervalVector(1,init_time),epsilon_time);


		// Now we create the contractor for the complementary domaine
		vector<Ctc*> array_in;
		for (int i=0; i<n; i++) {
			for(int j=i+1; j<n; j++) {
				NumConstraint *c= new NumConstraint(V,t,  norm_2(traj_avion(V[i],t, d_0[i],x_0[i]) -traj_avion(V[j],t, d_0[j],x_0[j]))<=secu);
				array_in.push_back(new CtcFwdBwd(*c));
			}
		}
		CtcUnion inside1(array_in);
		CtcExist inside(inside1,mask,IntervalVector(1,init_time),epsilon_time);

		// Build the initial box of V
		IntervalVector init_box(n,Interval(-0.1,0.1));

		// objective function
		const ExprNode* e=&(sqr(V[0]));
		for (int i=1; i<n; i++)
			e = & (*e + sqr(V[i]));
		Function norm_n(V,*e);
		Function f_cost(V, sqrt(norm_n(V)));

		// create a stategy to bisect a element
		LargestFirst  bsc(epsilon);
		
		OptimCtc o(outside,inside,f_cost,bsc,epsilon,gaol_prec,gaol_prec);
		// the trace
			o.trace=-10;

			// the allowed time for search
			o.timeout=100;

			// the search itself
			o.optimize(init_box);

			// printing the results
			o.report();
	}
	 vibes::endDrawing();      // <== closes the VIBES "connection"
	return 0;
}

