#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <set>
using namespace std;
using namespace ibex;

Interval matrixtrace (IntervalMatrix& M){
  Interval inter(0,0);
  if (M.nb_rows()==M.nb_cols())
    for (int k =0; k< M.nb_rows(); k++)
      inter+=M[k][k];
  return inter;
}


 int main(int argc, char** argv) {


  
	double b1=939.5973154362416;
	double b3=600.0;
	double b5=b1;
	double b6=399;
	double a1=1/b1;
        double a3=- b3/b1;
	double a5=1/b5;
	double a6=-b6/b5;

        cout << a1 << endl;
        cout << a3 << endl;
        cout << a5 << endl;
        cout << a6 << endl;


	Matrix k1 (3,3);
	k1[0][0]=b1;
	k1[0][1]=0;
	k1[0][2]=b3;
	k1[1][0]=0;
	k1[1][1]=b5;
	k1[1][2]=b6;
	k1[2][0]=0;
	k1[2][1]=0;
	k1[2][2]=1;


        Matrix F (3,3);
	
	
	F[0][0]=-2.20246e-06 ;
	F[0][1]=7.06071e-05 ;
	F[0][2]=-0.0315415;
	F[1][0]=-2.3367e-05;
	F[1][1]=-8.85794e-07;
	F[1][2]=0.12567;
	F[2][0]=0.0149924;
	F[2][1]=-0.127773;
	F[2][2]=-1.86073;
	

	



        
	Matrix G (3,3);
	
	G= k1.transpose()*F*k1;

	
       	cout << "essential sol ransac " << endl;
	cout << G[0][0] << "  " << G[0][1] <<  "  " << G[0][2] << endl;
	cout << G[1][0] << "  " << G[1][1] <<  "  " << G[1][2] << endl;
	cout << G[2][0] << "  " << G[2][1] <<  "  " << G[2][2] << endl;
	

	double enorm=0;
	for (int i=0;i<3;i++)
	  for(int j=0;j<3;j++)
	    enorm+=G[i][j]*G[i][j];
	G= sqrt((1/enorm))*G;


 
	
	cout << "essential sol ransac normalise" << endl;
	cout << G[0][0] << "  " << G[0][1] <<  "  " << G[0][2] << endl;
	cout << G[1][0] << "  " << G[1][1] <<  "  " << G[1][2] << endl;
	cout << G[2][0] << "  " << G[2][1] <<  "  " << G[2][2] << endl;
	
 }
