//============================================================================
//                                  I B E X
// File        : test_Shekel5.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : oct 22, 2013
//============================================================================



#include "ibex.h"

#ifndef _IBEX_WITH_AFFINE_EXTENDED_
#error "You need the plugin Affine Extended to run this example."
#endif

using namespace std;
using namespace ibex;

int main() {
	Timer timer;
	int n= 1.e8;
	double a[5][4]= { {4,4,4,4}, {1,1,1,1}, {8,8,8,8}, {6,6,6,6}, { 3,7,3,7}};
	double c[5] = {0.1, 0.2, 0.2, 0.4, 0.4};

	double x1[4] = {4,4,4,4};
	IntervalVector x2(4,Interval(3.9,4.1));
	cout.precision(15);

	cout<< "-----------------------------------------------------------" << endl;
	cout << " CPU-time for "<<n<<" evaluations of the Shekel-5 function at the point x1=(4,4,4,4)" <<endl;
	{
		timer.restart();
		double f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z = z+ ::pow((x1[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Double precision: " << timer.get_time() << " s" << endl;
	}
	{
		timer.restart();
		IntervalVector x(4,Interval(4));
		Interval f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z = z+pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Interval Arithmetic: " << timer.get_time() << " s" << endl;
	}
	{
		timer.restart();
		AffineMain<AF_No> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_No>(4,l+1,Interval(4));
		}

		AffineMain<AF_No> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine No Rounding: " << timer.get_time() << " s" << endl;
	}
	{
		timer.restart();
		AffineMain<AF_sAF> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_sAF>(4,l+1,Interval(4));
		}

		AffineMain<AF_sAF> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine sAF: " << timer.get_time() << " s" << endl;
	}
	{
		timer.restart();
		AffineMain<AF_iAF> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_iAF>(4,l+1,Interval(4));
		}

		AffineMain<AF_iAF> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine iAF: " << timer.get_time() << " s" << endl;
	}

	{
		timer.restart();
		AffineMain<AF_fAF1> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_fAF1>(4,l+1,Interval(4));
		}

		AffineMain<AF_fAF1> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine fAF version 1: " << timer.get_time() << " s" << endl;
	}

	{
		timer.restart();
		AffineMain<AF_fAF2> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_fAF2>(4,l+1,Interval(4));
		}

		AffineMain<AF_fAF2> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine fAF version 2: " << timer.get_time() << " s" << endl;
	}

	{
		timer.restart();
		AffineMain<AF_fAF2_fma> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_fAF2_fma>(4,l+1,Interval(4));
		}

		AffineMain<AF_fAF2_fma> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine fAF version 2 with fma option: " << timer.get_time() << " s" << endl;
	}

	cout<< "-----------------------------------------------------------" << endl;
	cout << " CPU-time for "<<n<<" evaluations of the Shekel-5 function at the Interval x2=([3.9,4.1],[3.9,4.1],[3.9,4.1],[3.9,4.1])" <<endl;

	{
		timer.restart();
		IntervalVector x(4,Interval(3.9,4.1));
		Interval f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z = z+pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Interval Arithmetic: " << timer.get_time() << " s, eval = "<< f << endl;
	}
	{
		timer.restart();
		AffineMain<AF_No> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_No>(4,l+1,Interval(3.9,4.1));
		}

		AffineMain<AF_No> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine No Rounding: " << timer.get_time() << " s, eval = "<< f << endl;
	}
	{
		timer.restart();
		AffineMain<AF_sAF> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_sAF>(4,l+1,Interval(3.9,4.1));
		}

		AffineMain<AF_sAF> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine sAF: " << timer.get_time() << " s, eval = "<< f << endl;
	}
	{
		timer.restart();
		AffineMain<AF_iAF> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_iAF>(4,l+1,Interval(3.9,4.1));
		}

		AffineMain<AF_iAF> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine iAF: " << timer.get_time() << " s, eval = "<< f << endl;
	}

	{
		timer.restart();
		AffineMain<AF_fAF1> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_fAF1>(4,l+1,Interval(3.9,4.1));
		}

		AffineMain<AF_fAF1> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine fAF version 1: " << timer.get_time() << " s, eval = "<< f << endl;
	}

	{
		timer.restart();
		AffineMain<AF_fAF2> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_fAF2>(4,l+1,Interval(3.9,4.1));
		}

		AffineMain<AF_fAF2> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine fAF version 2: " << timer.get_time() << " s, eval = "<< f << endl;
	}

	{
		timer.restart();
		AffineMain<AF_fAF2_fma> x[4];
		for (int l=0; l<4;l++){
			x[l]= AffineMain<AF_fAF2_fma>(4,l+1,Interval(3.9,4.1));
		}

		AffineMain<AF_fAF2_fma> f, z;
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		timer.stop();
		cout<< "Affine fAF version 2 with fma option: " << timer.get_time() << " s, eval = "<< f << endl;
	}


	cout<< "-----------------------------------------------------------" << endl;

	return 0;

}
