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

void contract_and_draw( Ctc& c, IntervalVector& X, const string color) {
	IntervalVector X0(X);       // get a copy
	try {
		c.contract(X);
	} catch(EmptyBoxException&) {
		vibes::drawBox(X0[0].lb(),X0[0].ub(),X0[1].lb(),X0[1].ub(),color);
		return;
	}
	if (X==X0) return;     // nothing contracted.
	IntervalVector* rest;
	int n=X0.diff(X,rest); // calculate the set difference
	for (int i=0; i<n; i++) {     // display the boxes
		vibes::drawBox(rest[i][0].lb(),rest[i][0].ub(), rest[i][1].lb(),rest[i][1].ub(),color);
	}
	delete[] rest;
	return;
}


int main() {
	{
		std::string figureName = "SIVIA_0";
		vibes::beginDrawing();           // <== Initializes the VIBES "connection"
		vibes::newFigure(figureName);       // <== Create a new VIBes figure

		double epsilon = 1.e-2;
		// Example #4
		Variable p(2), t, ii,jj,x(2);

		Function traj (p,t,ii, Return (10*cos(t+p[0]*ii) + 3*cos(p[1]*(t+p[0]*ii)),
				                       15*sin(t+p[0]*ii) + 3*sin(p[1]*(t+p[0]*ii))) );
		Function norm (x, sqrt(sqr(x[0]) +sqr(x[1])));

		Function dist_robot(p,t,ii,jj, norm(traj(p,t,ii) -traj(p,t,jj)));

		// we want the distance between the robots stay larger than 0.2.
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
		IntervalVector init_box(2);
		init_box[0]=Interval(-10,10);
		init_box[1]=Interval(-10,10);


		// Build the way boxes will be bisected.
		// "LargestFirst" means that the dimension bisected
		// is always the largest one.
		LargestFirst lf;
		list<IntervalVector> s;
		s.push_back(init_box);
		IntervalVector box(2);
		cout<<"start"<<endl;
		int i=0;
		while (!s.empty()) {
			box=s.front();
			s.pop_front();
			contract_and_draw(outside,box,"[b]cyan");
			if (box.is_empty())  continue;

			contract_and_draw(inside,box,"[r]magenta");
			if (box.is_empty())  continue;

			if (std::max(box[0].diam(),box[1].diam())<epsilon) {
				vibes::drawBox(box[0].lb(),box[0].ub(),box[1].lb(),box[1].ub(),"[y]yellow");
			} else {
				pair<IntervalVector,IntervalVector> boxes=lf.bisect(box);
				s.push_back(boxes.first);
				s.push_back(boxes.second);
			}
		}
	}
	 vibes::endDrawing();      // <== closes the VIBES "connection"
	return 0;
}

