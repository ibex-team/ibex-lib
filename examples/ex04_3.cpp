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
		if (X==X0) return;     // nothing contracted.
		IntervalVector* rest;
		int n=X0.diff(X,rest); // calculate the set difference
		for (int i=0; i<n; i++) {     // display the boxes
			vibes::drawBox(rest[i][0].lb(),rest[i][0].ub(), rest[i][1].lb(),rest[i][1].ub(),color);
		}
		delete[] rest;
	} catch(EmptyBoxException&) {
		vibes::drawBox(X0[0].lb(),X0[0].ub(),X0[1].lb(),X0[1].ub(),color);
	}
}


int main() {
	{

		std::string figureName = "SIVIA_3";
		vibes::beginDrawing();           // <== Initializes the VIBES "connection"
		vibes::newFigure(figureName);       // <== Create a new VIBes figure


		double epsilon = 1.e-1;
		// Example #4
		Variable p(2), t, ii, i2(2),x(2);

		Function traj (p,t,ii, Return (10*cos(t+p[0]*ii) + 3*cos(p[1]*(t+p[0]*ii)),
				                       15*cos(t+p[0]*ii) + 3*cos(p[1]*(t+p[0]*ii))) );
		Function norm (x, sqrt(sqr(x[0]) +sqr(x[1])));

		Function dist_robot(p,t,i2, norm(traj(p,t,i2[0]) -traj(p,t,i2[1])));

		// we want the distance between the robots stay larger than 0.2.
		double secu =1;
		int n=4;
		Vector iv(2);
		SystemFactory fac;
		fac.add_var(p);
		fac.add_var(t);
		for (int i=0; i<n; i++) {
			for(int j=0; j<n; j++) {
				if (i!=j) {
					iv[0] =i; iv[1]=j;
					fac.add_ctr(*new NumConstraint(p,t, dist_robot(p,t,iv)>=secu));
				}

			}
		}
		System sys(fac);
		LinearRelaxAffine2 lin(sys);
		CtcPolytopeHull ctc1(lin);
		CtcHC4 outside1(sys.ctrs);
		CtcCompo outside2(outside1,ctc1);
		Ctc3BCid outside3(2,outside1);

		BitSet mask(BitSet::all(3));
		mask.remove(2);

		CtcForAll outside(outside3,mask,IntervalVector(1,Interval(0,1)),1.e-3);

		vector<Ctc*> array_in;
		for (int i=0; i<n; i++) {
			for(int j=0; j<n; j++) {
				if (i!=j) {
					iv[0] =i; iv[1]=j;
					NumConstraint *c =new NumConstraint(p,t, dist_robot(p,t,iv)<=secu);
					array_in.push_back(new CtcFwdBwd(*c));
				}

			}
		}
		CtcUnion inside1(array_in);
		Ctc3BCid inside2(inside1);
		CtcExist inside(inside2,mask,IntervalVector(1,Interval(0,1)),1.e-3);

		// Build the initial box.
		IntervalVector init_box(2);
		init_box[0]=Interval(-10,10);
		init_box[1]=Interval(-10,10);


		// Build the way boxes will be bisected.
		// "LargestFirst" means that the dimension bisected
		// is always the largest one.
		LargestFirst lf;
		stack<IntervalVector> s;
		s.push(init_box);
		IntervalVector box(2);
		cout<<"start"<<endl;
		int i=0;
		while (!s.empty()) {
			box=s.top();
			s.pop();
			contract_and_draw(outside,box,"b");
			if (box.is_empty()) { continue; }

			contract_and_draw(inside,box,"r");
			if (box.is_empty()) { continue; }

			if (std::max(box[0].diam(),box[1].diam())<epsilon) {
				vibes::drawBox(box[0].lb(),box[0].ub(),box[1].lb(),box[1].ub(),"y");
			} else {
				pair<IntervalVector,IntervalVector> boxes=lf.bisect(box);
				s.push(boxes.first);
				s.push(boxes.second);
			}
		}
	}
	vibes::endDrawing();      // <== closes the VIBES "connection"
	return 0;
}

