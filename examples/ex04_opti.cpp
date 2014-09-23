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
		// Example #4
		Variable p(2), t, ii,jj,x(2);

		Function traj (p,t,ii, Return (10*cos(t+p[0]*ii) + 3*cos(p[1]*(t+p[0]*ii)),
				                       15*sin(t+p[0]*ii) + 3*sin(p[1]*(t+p[0]*ii))) );
				                       
		Function norm (x, sqrt(sqr(x[0]) +sqr(x[1])));

		Function dist_robot(p,t,ii,jj, norm(traj(p,t,ii) -traj(p,t,jj)));

		// we want the distance between the robots stay larger than 2.
		double secu =2;
		int n=3;
		vector<NumConstraint*> array_out;
		for (int i=0; i<n; i++) {
			for(int j=i+1; j<n; j++) {
				array_out.push_back(new NumConstraint(p,t, dist_robot(p,t,Interval(i),Interval(j))>=secu));


			}
		}
		CtcHC4 outside1(array_out);
		BitSet mask(BitSet::all(3));
		mask.remove(2);

		//Ctc3BCid outside2(outside1);
		CtcForAll outside(outside1,mask,IntervalVector(1,Interval(0,1)),1.e-3);

		vector<Ctc*> array_in;
		for (int i=0; i<n; i++) {
			for(int j=i+1; j<n; j++) {
				NumConstraint *c =new NumConstraint(p,t, dist_robot(p,t,Interval(i),Interval(j))<=secu);
				array_in.push_back(new CtcFwdBwd(*c));
			}
		}
		CtcUnion inside1(array_in);
		//Ctc3BCid inside2(inside1);
		CtcExist inside(inside1,mask,IntervalVector(1,Interval(0,1)),1.e-3);

		// Build the initial box.
		IntervalVector init_box(2,Interval(-10,10));

		// objective function
		Variable x1,x2;
		Function f_cost1(x1,x2, (sin(x1)*pow((sin(sqr(x1)/3.1416)),20)) + (sin(x2)*pow(sin(2*sqr(x2)/3.1416),20)));
		Function f_cost(x1,x2, f_cost1(x1+2,x2+2));

		LargestFirst  bsc(epsilon);
		OptimCtc o(inside,outside,f_cost,bsc,epsilon,gaol_prec,gaol_prec);
		// the trace
			o.trace=1;

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

