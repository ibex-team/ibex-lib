//============================================================================
//                                  I B E X
// File        : test_Shekel5.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : oct 22, 2013
//============================================================================



#include "ibex.h"

using namespace std;
using namespace ibex;

int main() {
	double a[5][4]= { {4,4,4,4}, {1,1,1,1}, {8,8,8,8}, {6,6,6,6}, { 3,7,3,7}};
	double c[5] = {0.1, 0.2, 0.2, 0.4, 0.4};

	double x1[4] = {4,4,4,4};
	IntervalVector x2(4,Interval(3.9,4.1));

	cout<< "-----------------------------------------------------------" << endl;
	cout << " CPU-time for 1.e6 evaluation of the Shekel-5 function at the point x1=(4,4,4,4)" <<endl;
	{
		double time=0;
		Timer::start();

		double f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z = z+ ::pow((x1[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Double precision: " << time << " s" << endl;
	}
	{
		double time=0;
		Timer::start();
		IntervalVector x(4,Interval(4));
		Interval f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z = z+pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Interval Arithmetic: " << time << " s" << endl;
	}
	{
		double time=0;
		Timer::start();
		Affine2Main<AF_No> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_No>(4,l+1,Interval(4));
		}

		Affine2Main<AF_No> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 No Rounding: " << time << " s" << endl;
	}
	{
		double time=0;
		Timer::start();
		Affine2Main<AF_sAF> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_sAF>(4,l+1,Interval(4));
		}

		Affine2Main<AF_sAF> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 sAF: " << time << " s" << endl;
	}
	{
		double time=0;
		Timer::start();
		Affine2Main<AF_iAF> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_iAF>(4,l+1,Interval(4));
		}

		Affine2Main<AF_iAF> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 iAF: " << time << " s" << endl;
	}

	{
		double time=0;
		Timer::start();
		Affine2Main<AF_fAF1> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_fAF1>(4,l+1,Interval(4));
		}

		Affine2Main<AF_fAF1> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 fAF version 1: " << time << " s" << endl;
	}

	{
		double time=0;
		Timer::start();
		Affine2Main<AF_fAF2> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_fAF2>(4,l+1,Interval(4));
		}

		Affine2Main<AF_fAF2> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 fAF version 2: " << time << " s" << endl;
	}

	{
		double time=0;
		Timer::start();
		Affine2Main<AF_fAF2_fma> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_fAF2_fma>(4,l+1,Interval(4));
		}

		Affine2Main<AF_fAF2_fma> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 fAF version 2 with fma option: " << time << " s" << endl;
	}

	cout<< "-----------------------------------------------------------" << endl;
	cout << " CPU-time for 1.e6 evaluation of the Shekel-5 function at the Interval x1=([3.9,4.1],[3.9,4.1],[3.9,4.1],[3.9,4.1])" <<endl;

	{
		double time=0;
		Timer::start();
		IntervalVector x(4,Interval(3.9,4.1));
		Interval f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z = z+pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Interval Arithmetic: " << time << " s, eval = "<< f << endl;
	}
	{
		double time=0;
		Timer::start();
		Affine2Main<AF_No> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_No>(4,l+1,Interval(3.9,4.1));
		}

		Affine2Main<AF_No> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 No Rounding: " << time << " s, eval = "<< f << endl;
	}
	{
		double time=0;
		Timer::start();
		Affine2Main<AF_sAF> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_sAF>(4,l+1,Interval(3.9,4.1));
		}

		Affine2Main<AF_sAF> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 sAF: " << time << " s, eval = "<< f << endl;
	}
	{
		double time=0;
		Timer::start();
		Affine2Main<AF_iAF> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_iAF>(4,l+1,Interval(3.9,4.1));
		}

		Affine2Main<AF_iAF> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 iAF: " << time << " s, eval = "<< f << endl;
	}

	{
		double time=0;
		Timer::start();
		Affine2Main<AF_fAF1> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_fAF1>(4,l+1,Interval(3.9,4.1));
		}

		Affine2Main<AF_fAF1> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 fAF version 1: " << time << " s, eval = "<< f << endl;
	}

	{
		double time=0;
		Timer::start();
		Affine2Main<AF_fAF2> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_fAF2>(4,l+1,Interval(3.9,4.1));
		}

		Affine2Main<AF_fAF2> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 fAF version 2: " << time << " s, eval = "<< f << endl;
	}

	{
		double time=0;
		Timer::start();
		Affine2Main<AF_fAF2_fma> x[4];
		for (int l=0; l<4;l++){
			x[l]= Affine2Main<AF_fAF2_fma>(4,l+1,Interval(3.9,4.1));
		}

		Affine2Main<AF_fAF2_fma> f, z;
		for (int k =0; k<1.e6; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z += pow((x[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		Timer::stop();
		time+= Timer::VIRTUAL_TIMELAPSE();
		cout<< "Affine2 fAF version 2 with fma option: " << time << " s, eval = "<< f << endl;
	}


	cout<< "-----------------------------------------------------------" << endl;

	return 0;

}
